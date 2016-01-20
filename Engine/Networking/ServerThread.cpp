//==============================================================================================================
//ServerThread.cpp
//by Albert Chen Jan-16-2016.
//==============================================================================================================

#include "ServerThread.hpp"
#include "Engine/Console/DevConsole.hpp"

//===========================================================================================================

Server::Server(const size_t& maxConnections) {
	static int serverThreadID = 0;

	std::string zergHost = GetLocalHostName();
	std::string service = "1234";
	//spawn host thread
	ServerThread* serverThread;
	serverThread = new ServerThread(zergHost, service, maxConnections);
	serverThread->manager = theJobManager;
	m_serverHandles.push_back(serverThread);
	theJobManager->m_threads.push_back(serverThread);
	theJobManager->m_threads.back()->Start("Server Thread " + IntToString(serverThreadID));
	serverThreadID++;

	zergHost = "localhost";
	serverThread = new ServerThread(zergHost, service, maxConnections);
	serverThread->manager = theJobManager;
	m_serverHandles.push_back(serverThread);
	theJobManager->m_threads.push_back(serverThread);
	theJobManager->m_threads.back()->Start("Server Thread " + IntToString(serverThreadID));
	serverThreadID++;
}

//-----------------------------------------------------------------------------------------------------------

void Server::CloseSocket() {
	for (ServerThreadsIterator it = m_serverHandles.begin(); it != m_serverHandles.end(); ++it) {
		ServerThread& thread = *(*it);
		thread.CloseSocket();
	}
}

//===========================================================================================================

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

ServerThread::ServerThread(const std::string& hostName, const std::string& service, const size_t& maxConnections, int addrFamily , int addrInfoFlags) {
	
	type = JOB_TYPE_NET_SERVER;

	SetServerConnection(hostName, service, addrFamily, addrInfoFlags);
	
	m_serverSocket.m_name = hostName;
	m_serverSocket.m_socketID = BindSocketWithConnection(m_connection);
	m_serverSocket.m_maxConnections = maxConnections;

}

//-----------------------------------------------------------------------------------------------------------

///----------------------------------------------------------------------------------------------------------
///construction helpers

void ServerThread::SetServerConnection(const std::string& hostName, const std::string& service, int addrFamily, int addrInfoFlags) {

	m_connection = GetConnection(hostName, service, addrFamily, addrInfoFlags);

}

//-----------------------------------------------------------------------------------------------------------

void ServerThread::Execute() {

	static int clientHandleThreadID = 0;
	m_serverSocket.Listen();
	
	while (manager->IsRunning()) {

		if (!m_isRunning) {

			theJobManager->m_isRunning = false;

			break;
		}
		//the last server thread is deadlocked inside this function...is running for the last server is still true
		SOCKET otherSocket = INVALID_SOCKET;
		otherSocket = m_serverSocket.Accept();
		
		if (otherSocket != INVALID_SOCKET) {
			printf("[%s] Accepted [%d]\n", m_serverSocket.m_name.c_str(), otherSocket);
			//spin up a connection thread for this other
			ClientHandleThread* clientHandleThread = new ClientHandleThread(m_serverSocket.m_name, m_connection.service, otherSocket);
			clientHandleThread->manager = theJobManager;
			clientHandleThread->m_serverHandle = this;

			m_clientHandles.push_back(clientHandleThread);

			theJobManager->m_threads.push_back(clientHandleThread);
			theJobManager->m_threads.back()->Start("Client Handle Thread " + m_serverSocket.m_name + " " + IntToString(clientHandleThreadID));
			
			clientHandleThreadID++;
		}
	}


}

//-----------------------------------------------------------------------------------------------------------

void ServerThread::CloseSocket(){
	CloseAllClientHandles();

	m_serverSocket.CloseSocket();
	
	printf("\n[%s] quits.", m_serverSocket.m_name.c_str());

	m_isRunning = false;

}

//-----------------------------------------------------------------------------------------------------------

void ServerThread::CloseAllClientHandles() {
	for (ClientHandleThreadsIterator it = m_clientHandles.begin(); it != m_clientHandles.end(); ){
		 ClientHandleThread& thread = *(*it);

		thread.CloseSocket();

		it = m_clientHandles.erase(it);
		
		
	}
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///friend methods



//===========================================================================================================

ClientHandleThread::ClientHandleThread(const std::string& hostName, const std::string& service, const SOCKET& handleSocket, int addrFamily, int addrInfoFlags)
	//: ServerThread(hostName, service, addrFamily, addrInfoFlags)
{
	type = JOB_TYPE_NET_SERVER;

	SetServerConnection(hostName, service, addrFamily, addrInfoFlags);

	m_serverSocket.m_name = hostName;
	m_otherSocket = handleSocket;
}

//-----------------------------------------------------------------------------------------------------------

void ClientHandleThread::Execute() {
	while (manager->IsRunning()) {

		char* bufferData = m_serverSocket.RecieveBufferDataFromOtherSocket(m_otherSocket);
		
		if (bufferData) {
			//RunCommandFromBufferText(std::string(bufferData));
			if (CompareCStr(bufferData, "serverquit") == 0) {

				m_serverSocket.SendBufferDataToOtherSocket(m_otherSocket, "quit");

				if (m_serverHandle) {
					m_serverHandle->CloseSocket();
					return;
				}
			}

			m_serverSocket.SendBufferDataToOtherSocket(m_otherSocket, bufferData);
		}

	}
	
}

//===========================================================================================================
