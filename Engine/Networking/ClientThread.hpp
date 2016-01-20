//==============================================================================================================
//ClientThread.hpp
//by Albert Chen Jan-16-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ClientThread__
#define _included_ClientThread__

//===========================================================================================================

#include "ConnectionSocket.hpp"
#include "Engine/Multithreading/JobManager.hpp"

//===========================================================================================================

struct ClientThread : DedicatedJobThread {
	//vars
	ClientSocket m_clientSocket;
	Connection m_connection;

	std::string m_currentMessage = "";

	//methods
	ClientThread(const std::string& clientName, std::string& hostName, const std::string& service, int addrFamily = AF_INET, int addrInfoFlags = 0);
	
	ClientThread();
	~ClientThread();

	void SetClientConnection(std::string& hostName, const std::string& service, int addrFamily = AF_INET, int addrInfoFlags = 0);

	void Execute();

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

///----------------------------------------------------------------------------------------------------------
///constructors
inline ClientThread::ClientThread() {
	//do nothing
}

inline ClientThread::~ClientThread() {
	//do nothing
}

//===========================================================================================================


//===========================================================================================================

#endif //__includedClientThread__

#pragma once
