//==============================================================================================================
//JobManager.cpp
//by Albert Chen Nov-3-2015.
//==============================================================================================================

#include "JobManager.hpp"
#include "Engine\Console\DevConsole.hpp"

//===========================================================================================================

JobManager* theJobManager = NULL;

//===========================================================================================================

CONSOLE_COMMAND(generateFiles){
	std::string argText;
	if (COMMAND_HAS_ARGS){

		int fileCount = ARG_TOKEN_I(0);
		int kbPerFile = ARG_TOKEN_I(1);

		//unsigned char* buffer = new unsigned char[kbPerFile * 1000];
		Job* newJob;
		for (int i = 0; i < fileCount; i++){

			std::string genFileName = "generated_" + IntToString(kbPerFile) + "kb_file_" + IntToString(i + 1) + ".dat";

			if (theJobManager){
				newJob = new GenerateFileJob(genFileName, kbPerFile);
				theJobManager->AddPendingJob(newJob, PRIORITY_HIGH, NULL);
			}

		}

		argText += "Generated " + IntToString(fileCount) + " files of " + IntToString(kbPerFile) + " each!\nThe Application will now Quit.";

		int outParam = ConsoleGenericMessageBox(argText, "ConsoleCommand: Generate Files", MB_OK);
		if (outParam == IDOK){
			
			//quit game
			IF_INPUT_SYSTEM_EXISTS
				theInputSystem->SetSystemQuit();

		}

	}

}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///job thread methods

void JobThread::ProcessAll(){
	while (manager->RunJob()) {
		//ConsolePrintString(GetName() + " runs a Job!");
	}
}

//-----------------------------------------------------------------------------------------------------------

void JobThread::Execute(){
	
	while (manager->IsRunning()){
		//ConsolePrintString(GetName() + "is Executing: ");
		ProcessAll();
		//Wait();
	}

	ProcessAll();
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///dedicated job thread methods

void DedicatedJobThread::ProcessAll(){
	while (manager->RunJob(type)){
		//ConsolePrintString(GetName() + " runs a Job!");
	}
}

//-----------------------------------------------------------------------------------------------------------

void DedicatedJobThread::Execute(){

	while (manager->IsRunning()){
		//ConsolePrintString(GetName() + "is Executing: ");
		ProcessAll();
		//Wait();
	}

	ProcessAll();
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

JobManager::JobManager(){

	m_isRunning = false;

	if (!theJobManager){
		theJobManager = this;
	}

	REGISTER_CONSOLE_COMMAND(generateFiles, "Generates <fileCount> files of <KBPerFile> each.");

}

JobManager::~JobManager(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

void JobManager::StartUp(int numThreads){
	m_isRunning = true;

	//split up threads evenly among categories sometime?
	int systemCores = SystemGetCoreCount();
	int systemConcurrentThreadCount = SystemGetHardwareConcurrentThreads();

	ConsolePrintf("\n===Job Manager Start Up===\nCores: %d | Thread Limit: %d |\n", systemCores, systemConcurrentThreadCount);

	AddThreads(numThreads);
	//4 dedicated threads
// 	AddDedicatedThreads(JOB_TYPE_FILEIO, 2);
// 
// 	AddDedicatedThreads(JOB_TYPE_RENDERING, 1);
// 	AddDedicatedThreads(JOB_TYPE_INPUT, 1);
// 	AddDedicatedThreads(JOB_TYPE_SOUND, 1);

	m_activeJobCount = 0;

}

//-----------------------------------------------------------------------------------------------------------

void JobManager::ShutDown(){
	m_isRunning = false;
	
	while (!m_threads.empty()) {
		Thread* thread = m_threads.back();
		if (thread){
			thread->Join();
			delete thread;
			thread = NULL;
		}
		
		m_threads.pop_back();
	}

	// finish out finished jobs
	Update();
}

///----------------------------------------------------------------------------------------------------------
///upkeep

void JobManager::Update(double maxTimeMs){
	UNUSED(maxTimeMs);
	PROFILE_SECTION();
	
	if (m_threads.empty()) {
		RunJob();
	}

	Job* finishedJob;
	while (m_finishedJobs.Dequeue(&finishedJob)) {
		if (finishedJob){
			finishedJob->ExecuteJobCompleteCallBack();

			delete finishedJob;
		}
	}

}

///----------------------------------------------------------------------------------------------------------
///job management

void JobManager::AddPendingJob(Job *job, const Priority& priority, JobCompleteCallback* jobCompleteCallbackFunc, const JobType& jobType){
	if (job){
		job->m_jobCompleteCallback = jobCompleteCallbackFunc;

		m_pendingJobs[jobType][priority].Enqueue(job);
	}

	IncrementActiveJobCount();

}

//-----------------------------------------------------------------------------------------------------------

void JobManager::AddPendingJob(const Priority& priority, JobCompleteCallbackFunc* jobCallBackFunc, void* data, const JobType& jobType ){
	Job* job = new Job(priority, jobCallBackFunc, data);

	if (job){
		m_pendingJobs[jobType][priority].Enqueue(job);
	}

	IncrementActiveJobCount();
}

//-----------------------------------------------------------------------------------------------------------

bool JobManager::RunJob(const JobType& jobType ){
	Job *job;

	//auto &queue = m_pendingJobs[jobType]; //do something to not have to dereference pending jobs
	for (int p = NUM_PRIORITIES-1; p >= 0; --p){
		if (!m_pendingJobs[jobType][p].Empty()){

			if (m_pendingJobs[jobType][p].Dequeue(&job)) {

				job->Execute();
				
				if (job->m_jobCompleteCallback != NULL){
					m_finishedJobs.Enqueue(job);
				}
				else{
					delete job;
					job = NULL;
				}

				DecrementActiveJobCount();

				return true;

			}
		}
	}
	

	return false;
}

///----------------------------------------------------------------------------------------------------------
///thread management
void JobManager::AddThreads(int numThreads){

	for (int i = 0; i < numThreads; i++){
		m_threads.push_back(new JobThread(this));
		m_threads.back()->Start("Job Thread " + IntToString(i));
	}

}

void JobManager::AddDedicatedThreads(const JobType& jobType, int numThreads){
	for (int i = 0; i < numThreads; i++){
		m_threads.push_back(new DedicatedJobThread(jobType, this));
		m_threads.back()->Start("Dedicated Job Thread "+JobTypeToString(jobType) + IntToString(i));
	}
}

//-----------------------------------------------------------------------------------------------------------


//===========================================================================================================

