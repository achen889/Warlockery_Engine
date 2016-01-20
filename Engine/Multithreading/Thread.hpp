//==============================================================================================================
//Thread.hpp
//by Albert Chen Nov-3-2015.
//==============================================================================================================

#pragma once

#ifndef _included_Thread__
#define _included_Thread__

#include <thread>
#include <chrono>
#include <vector>
#include <string>

//===========================================================================================================

struct Thread{
	//vars
	std::thread t;
	std::string name;

	//methods
	Thread();
	~Thread();
	virtual void Start(const std::string& newName){
		t = std::thread::thread(&Thread::Execute, this);
		name = newName;

		ConsolePrintString("Spawning: "+newName+"\n");

	}

	virtual void Execute(){
		//must override in stuff
	}

	//cleanup
	void Detach(){
		t.detach();
	}

	void Join(){
		t.join();
	}

	//helpers
	std::string GetName(){
		return name;
	}

	friend unsigned int SystemGetHardwareConcurrentThreads();

protected:

	void Sleep(const long& sleepTimeMs){
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
	}

	//wait is basically yield
	void Wait(){
		std::this_thread::yield();
	}

};

typedef std::vector<Thread*> Threads;
typedef std::vector<Thread*>::iterator ThreadsIterator;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods

inline Thread::Thread(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

inline Thread::~Thread(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

inline unsigned int SystemGetHardwareConcurrentThreads(){
	unsigned int numHardwareConcurrentThreads = std::thread::hardware_concurrency();

	return numHardwareConcurrentThreads;
}

//===========================================================================================================

#endif //__includedThread__

