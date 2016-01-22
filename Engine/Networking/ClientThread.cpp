//==============================================================================================================
//ClientThread.cpp
//by Albert Chen Jan-16-2016.
//==============================================================================================================

#include "ClientThread.hpp"
#include "Engine/Console/DevConsole.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

ClientThread::ClientThread(const std::string& clientName, std::string& hostName, const std::string& service, int addrFamily, int addrInfoFlags) {
	type = JOB_TYPE_NET_CLIENT;

	SetClientConnection(hostName, service, addrFamily, addrInfoFlags);

	m_clientSocket.m_name = clientName;
	m_clientSocket.m_socketID = ConnectToHostSocket(m_connection);

}

//-----------------------------------------------------------------------------------------------------------

void ClientThread::SetClientConnection(std::string& hostName, const std::string& service, int addrFamily , int addrInfoFlags ){
	
	m_connection = GetConnection(hostName, service, addrFamily, addrInfoFlags);
	
}


void ClientThread::Execute(){

	while (manager->IsRunning()) {

		char messageBuffer[1024];
		std::cout << "\n[" << m_clientSocket.m_name << "]: ";
		std::cin.getline(messageBuffer, 1024);

		if (strcmp(messageBuffer, "") != 0) {
			m_currentMessage = std::string(messageBuffer);
		}

		if (m_currentMessage != "") {

			m_clientSocket.SendBufferDataToOtherSocket(m_clientSocket.m_socketID, m_currentMessage.c_str());
			//process command
			RunCommandFromBufferText(m_currentMessage); //doesn't print right on server

			m_currentMessage = "";

			char* fromServer = m_clientSocket.RecieveBufferDataFromOtherSocket(m_clientSocket.m_socketID);
			//somehow get this to be called in a command
			if (!fromServer || CompareCStr(fromServer, "quit") == 0) {
				m_clientSocket.CloseSocket();
				manager->m_isRunning = false;
				return;
			}
		}

		
	}

}

//===========================================================================================================

