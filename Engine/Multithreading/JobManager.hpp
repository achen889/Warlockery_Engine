//==============================================================================================================
//JobManager.hpp
//by Albert Chen Nov-3-2015.
//==============================================================================================================

#pragma once

#ifndef _included_JobManager__
#define _included_JobManager__

#include "ThreadSafeQueue.hpp"
#include "Job.hpp"
#include "Thread.hpp"
//#include "Engine/Core/Utilities.hpp"
#include "Engine/Core/BinaryUtils.hpp"

//===========================================================================================================

enum JobType{
	JOB_TYPE_GENERAL,
	JOB_TYPE_RENDERING,
	JOB_TYPE_FILEIO,
	JOB_TYPE_INPUT,
	JOB_TYPE_SOUND,
	JOB_TYPE_EVENT,
	NUM_JOB_TYPES
};

inline std::string JobTypeToString(const JobType& jobType){
	switch (jobType){
	case JOB_TYPE_GENERAL:
		return "GENERAL";
	case JOB_TYPE_RENDERING:
		return "RENDERING";
	case JOB_TYPE_FILEIO:
		return "FILE IO";
	case JOB_TYPE_INPUT:
		return "INPUT";
	case JOB_TYPE_SOUND:
		return "SOUND";
	}
	return "INVALID";
}

//===========================================================================================================

class JobManager;

struct JobThread : Thread{
	//vars
	JobManager* manager;

	//methods
	JobThread(JobManager* myManager):
		manager(myManager)
	{

	}
	JobThread(){

	}
	~JobThread(){
		//do nothing
	}

	virtual void ProcessAll();
	virtual void Execute();

};

//===========================================================================================================

struct DedicatedJobThread : JobThread{
	//vars
	JobManager* manager;
	JobType type;


	//methods
	DedicatedJobThread(const JobType& myType, JobManager* myManager) :
		manager(myManager),
		type(myType)
	{

	}
	DedicatedJobThread(){

	}
	~DedicatedJobThread(){
		//do nothing
	}

	void ProcessAll();
	void Execute();


};

//===========================================================================================================

class JobManager{
public:
	 JobManager();
	~JobManager();

	//init/close
	void StartUp(int numThreads);
	void ShutDown();

	//accessors
	const unsigned int GetActiveJobCount() const { return m_activeJobCount; }
	void SetActiveJobCount(const unsigned int& activeJobCount = 0){ m_activeJobCount = activeJobCount;  }
	bool IsRunning() const { return m_isRunning; }
	void IncrementActiveJobCount();
	void DecrementActiveJobCount();

	//upkeep
	void Update(double maxTimeMs = 10.0);

	//job management
	void AddPendingJob(Job *job, const Priority& priority, 
		JobCompleteCallback* jobCompleteCallbackFunc = NULL, const JobType& jobType = JOB_TYPE_GENERAL);

	void AddPendingJob(const Priority& priority, JobCompleteCallbackFunc* jobCallBackFunc, 
		void* data, const JobType& jobType = JOB_TYPE_GENERAL);

	bool RunJob(const JobType& jobType = JOB_TYPE_GENERAL);

	//thread management
	void AddThreads(int numThreads);
	void AddDedicatedThreads(const JobType& jobType, int numThreads);

	//friend
	friend unsigned int SystemGetCoreCount();

	friend std::string GetThreadingDataString();

	Threads m_threads;
	bool m_isRunning;
private:
	
	unsigned int m_activeJobCount;
	
	ThreadSafeQueue<Job*> m_pendingJobs[NUM_JOB_TYPES][NUM_PRIORITIES];
	ThreadSafeQueue<Job*> m_finishedJobs;
	CriticalSection m_activeJobCountCritSec;
};

extern JobManager* theJobManager;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline void JobManager::IncrementActiveJobCount(){
	CRITICAL_SECTION_SCOPE(m_activeJobCountCritSec);
	m_activeJobCount++;
}
inline void JobManager::DecrementActiveJobCount(){
	CRITICAL_SECTION_SCOPE(m_activeJobCountCritSec);
	m_activeJobCount--;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods

inline unsigned int SystemGetCoreCount(){
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	return (unsigned int)sysinfo.dwNumberOfProcessors;
}

//-----------------------------------------------------------------------------------------------------------

inline std::string GetThreadingDataString(){
	unsigned int processorCount = SystemGetCoreCount();
	unsigned int concurrentThreadCount = SystemGetHardwareConcurrentThreads();

	std::string threadDataString = "Multithreading Data";
	threadDataString += "\nSystem Cores: " + IntToString(processorCount);
	threadDataString += "\nHardware Concurrent Threads: " + IntToString(concurrentThreadCount);

	return threadDataString;
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///default custom jobs

///----------------------------------------------------------------------------------------------------------
///this one lets you load a binary file
struct LoadBinaryFileJob : Job{

	//vars
	std::string filePath;
	unsigned char*& outBuffer;
	size_t outBufferSize;

	//methods
	LoadBinaryFileJob(const std::string& newfilePath, unsigned char*& outBufferPassed):
		outBuffer(outBufferPassed)
	{
		filePath = newfilePath;
	}

	void Execute(){
		outBuffer = LoadBinaryFileToNewBuffer(filePath, outBufferSize);

		ConsoleGenericMessageBox((char*)outBuffer, "Load File Job Complete");
	}
protected:
	//Assignment Operator
	const LoadBinaryFileJob& operator=(const LoadBinaryFileJob& jobToAssign){
		*this = jobToAssign;

		return *this;
	}
	
};

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///this one lets you reverse a arbitrary buffer
struct ReverseBufferJob : Job{

	//vars
	unsigned char*& outBuffer;

	//methods
	ReverseBufferJob(unsigned char*& outBufferPassed) :
		outBuffer(outBufferPassed)
	{
		
	}

	void Execute(){
		outBuffer = ReverseBuffer(outBuffer);

		ConsoleGenericMessageBox((char*)outBuffer, "ReverseBuffer Job Complete");
	}

	unsigned char* ReverseBuffer(unsigned char* outBufferPassed){
		
		int bufferLength = GetUCStrLength(outBufferPassed);
		unsigned char* reversedBuffer = new unsigned char[bufferLength];
		//int halfOfBufferLength = (int)((float)bufferLength * 0.5f);

		for (int i = 0; i < bufferLength ; i++){
			reversedBuffer[bufferLength-i-1] = outBufferPassed[i];
		}

		return reversedBuffer;
	}

protected:
	//Assignment Operator
	const ReverseBufferJob& operator=(const ReverseBufferJob& jobToAssign){
		*this = jobToAssign;

		return *this;
	}

};

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///this one lets you save a buffer to a file

struct SaveBinaryFileJob : Job{
	//vars
	std::string filePath;
	unsigned char* outBuffer;
	size_t outBufferSize;

	//methods
	SaveBinaryFileJob(const std::string& newfilePath, unsigned char* outBufferPassed) :
		outBuffer(outBufferPassed),
		filePath(newfilePath)
	{
		
	}

	void Execute(){

		BinaryBufferBuilder saveJobBuilder(filePath, outBuffer);

		saveJobBuilder.WriteBufferToFile();

	}
protected:
	//Assignment Operator
	const SaveBinaryFileJob& operator=(const SaveBinaryFileJob& jobToAssign){
		*this = jobToAssign;

		return *this;
	}

};

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///this one hashes the buffer

struct HashBufferJob : Job{

	//vars
	unsigned char* buffer;
	unsigned int& hashVal;
	//methods
	HashBufferJob(unsigned char* bufferToHash, unsigned int& newHashVal) :
		buffer(bufferToHash),
		hashVal(newHashVal)
	{
		hashVal = 0;
		//ConsoleGenericMessageBox("Adding Gen File Job\nName: "+filePath+"\nKB: "+IntToString(kbPerFile));
	}

	void Execute(){
		//do a hash algorithm then watch the magic
		//each char is 0 - 255
		//arbitrary size buffer
		//if (buffer){
			int bufferLength = GetUCStrLength(buffer);
			for (int i = 0; i < bufferLength; i++){
				//for (int j = 0; j < buffer[j]; j++){
				hashVal += buffer[i] * 31;
				hashVal -= 7;
				hashVal %= 255;
				//}
			}
			std::string testBuffer = (char*)buffer;
			ConsolePrintString("\nBuffer: " + testBuffer + " Hash Value: " + IntToString(hashVal));
			delete buffer;
		//}
	}
protected:
	//Assignment Operator
	const HashBufferJob& operator=(const HashBufferJob& jobToAssign){
		*this = jobToAssign;

		return *this;
	}

};

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///this one generates files of arbitrary size with random data
struct GenerateFileJob : Job{

	//vars
	std::string filePath;
	int kbFileSize;

	//methods
	GenerateFileJob(const std::string& newfilePath, const int& kbPerFile) :
		filePath(newfilePath),
		kbFileSize(kbPerFile * 1000)
	{
		//ConsoleGenericMessageBox("Adding Gen File Job\nName: "+filePath+"\nKB: "+IntToString(kbPerFile));
	}

	void Execute(){
		
		unsigned char* buffer = new unsigned char[kbFileSize];
		
		for (int j = 0; j < kbFileSize; j++){
			buffer[j] = GetRandomByte();
		}

		WriteExistingUCharBufferToFile(buffer, kbFileSize, +"Data/hash_" /*+ IntToString(hashVal)*/ + filePath);
		delete buffer;

		ConsolePrintString(filePath + " file wrote to disk!\n");
	}
protected:
	//Assignment Operator
	const GenerateFileJob& operator=(const GenerateFileJob& jobToAssign){
		*this = jobToAssign;

		return *this;
	}

};

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///this one fires an event on a thread, subscribers will still be called on the main thread
struct FireEventJob : Job {

	//vars
	std::string event_name;
	NamedProperties& event_params;

	//methods
	FireEventJob(const std::string& name, NamedProperties& params) :
		event_name(name),
		event_params(params)
	{
		
	}

	void Execute() {
		FireEvent(event_name, event_params);
	}
protected:
	//Assignment Operator
	const FireEventJob& operator=(const FireEventJob& jobToAssign) {
		*this = jobToAssign;

		return *this;
	}

};

//===========================================================================================================

//untested, we'll see if it works later

void FireEventOnThread(std::string event_name, NamedProperties& event_params);

inline void FireEventOnThread(std::string event_name, NamedProperties& event_params) {

	if (theJobManager) {
		Job* myFireEventJob = new FireEventJob(event_name, event_params);
		
		theJobManager->AddPendingJob(myFireEventJob, PRIORITY_HIGH);
	}
}


//===========================================================================================================



//===========================================================================================================



#endif //__includedJobManager__

