//==============================================================================================================
//ServerThread.hpp
//by Albert Chen Jan-16-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ServerThread__
#define _included_ServerThread__

//===========================================================================================================

#include "ConnectionSocket.hpp"
#include "Engine/Multithreading/JobManager.hpp"

//===========================================================================================================

struct ClientHandleThread;
typedef std::vector<ClientHandleThread*>ClientHandleThreads;

struct ServerThread;
typedef std::vector<ServerThread*>ServerThreads;
typedef std::vector<ServerThread*>::iterator ServerThreadsIterator;

//===========================================================================================================

struct Server {
	
	ServerThreads m_serverHandles;

	Server(const size_t& maxConnections);
	
	void CloseSocket();

};

//===========================================================================================================

struct ServerThread : DedicatedJobThread{

	//vars
	ServerSocket m_serverSocket;

	ClientHandleThreads m_clientHandles;
	
	bool m_isRunning = true;

	Connection m_connection;

	//methods
	ServerThread(const std::string& hostName, const std::string& service, const size_t& maxConnections, int addrFamily = AF_INET, int addrInfoFlags = AI_PASSIVE);

	ServerThread();
	~ServerThread();
	
	void SetServerConnection(const std::string& hostName, const std::string& service, int addrFamily = AF_INET, int addrInfoFlags = AI_PASSIVE);

	virtual void Execute();

	void CloseSocket();

	void CloseAllClientHandles();

};

typedef std::vector<ServerThread*>ServerThreads;
typedef std::vector<ServerThread*>::iterator ServerThreadsIterator;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline

///----------------------------------------------------------------------------------------------------------
///constructors

inline ServerThread::ServerThread() {
	//do nothing
}

inline ServerThread::~ServerThread() {
	//do nothing

}

//===========================================================================================================

struct ClientHandleThread : ServerThread {
	//vars
	SOCKET m_otherSocket;
	ServerThread* m_serverHandle;

	ClientHandleThread(const std::string& hostName, const std::string& service, const SOCKET& handleSocket, int addrFamily = AF_INET, int addrInfoFlags = AI_PASSIVE);

	void Execute();

	void CloseSocket(){ 
		m_serverSocket.SendBufferDataToOtherSocket(m_otherSocket, "quit");

		closesocket(m_otherSocket); 
	}

};

typedef std::vector<ClientHandleThread*>ClientHandleThreads;
typedef std::vector<ClientHandleThread*>::iterator ClientHandleThreadsIterator;
//===========================================================================================================

#endif //__includedServerThread__

