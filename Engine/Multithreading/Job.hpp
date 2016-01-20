//==============================================================================================================
//Job.hpp
//by Albert Chen Nov-3-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Job__
#define _included_Job__

//===========================================================================================================

enum Priority{
	PRIORITY_LOW,
	PRIORITY_MEDIUM,
	PRIORITY_HIGH,
	NUM_PRIORITIES
};

typedef void(JobCompleteCallbackFunc)(void* args);

struct JobCompleteCallback{

	//vars
	JobCompleteCallbackFunc* function;
	void* args;

	JobCompleteCallback(JobCompleteCallbackFunc* func, void* data) :
		function(func),
		args(data)
	{

	}

	//methods
	void CallJobCompleteFunction(){
		if (function){
			(*function)(args);
		}
	}

};

//===========================================================================================================

struct Job{

	//vars
	Priority m_priority;
	JobCompleteCallback* m_jobCompleteCallback;

	//methods
	Job(){
		//do nothing
	}
	virtual ~Job(){
		//do nothing
	}

	Job(const Priority& newPriority, JobCompleteCallbackFunc* jobCallBackFunc, void* data):
		m_priority(newPriority)
	{
		SetJobCompleteCallBack(jobCallBackFunc, data);
	}

	void SetJobCompleteCallBack(JobCompleteCallbackFunc* jobCallBackFunc, void* data){
		if (jobCallBackFunc){
			m_jobCompleteCallback = new JobCompleteCallback(jobCallBackFunc, data);
		}
		else{
			m_jobCompleteCallback = NULL;
		}
	}

	void SetJobCompleteCallBack(JobCompleteCallback* jobCompleteCallBack){
		m_jobCompleteCallback = jobCompleteCallBack;
	}

	virtual void Execute(){
		//expand this in the subclasses
	}
	
	void ExecuteJobCompleteCallBack(){
		if (m_jobCompleteCallback){
			m_jobCompleteCallback->CallJobCompleteFunction();
		}
	}


};



//===========================================================================================================

#endif //__includedJob__

