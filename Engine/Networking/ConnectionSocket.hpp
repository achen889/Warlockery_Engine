//==============================================================================================================
//ConnectionSocket.hpp
//by Albert Chen Jan-16-2016.
//==============================================================================================================

#pragma once

#ifndef _included_ConnectionSocket__
#define _included_ConnectionSocket__

//===========================================================================================================

#include "Engine/Networking/NetworkCommon.hpp"


//===========================================================================================================

struct Connection{
	addrinfo* addrInfo;
	std::string service;
	bool didSucceed;

	bool CreateSocket(addrinfo* iter, SOCKET& hostSock);

	bool DidSucceed() { return didSucceed; }

	friend Connection GetConnection(const std::string& host_name, const std::string& service, int addrFamily = AF_INET, int addrInfoFlags = AI_PASSIVE);
	
};

//===========================================================================================================

struct SocketInfo {

	SOCKET m_socketID;
	std::string m_name;
	
	void SetSocketInfo(const SOCKET& SocketID, const std::string& name) {
		m_socketID = SocketID;
		m_name = name;
	}

	void CloseSocket();
};

typedef std::vector<SocketInfo> SocketIDs;

inline void SocketInfo::CloseSocket() {

	int success = closesocket(m_socketID);
	m_socketID = INVALID_SOCKET;

	std::cout << "\n[" << m_name << "]: disconnected...";
}

//===========================================================================================================

struct ConnectionSocket{

	//methods
	ConnectionSocket();
	~ConnectionSocket();
	
	bool BindSocketToHost(std::string& hostName, const std::string& service, int addrFamily = AF_INET, int addrInfoFlags = AI_PASSIVE);
	bool ConnectSocketToHost(std::string& host_name, const std::string& service, int addrFamily = AF_INET);

	void SendBufferDataToOtherSocket(SOCKET otherSocket, const char* buffer);
	char* RecieveBufferDataFromOtherSocket(SOCKET otherSocket);
	
	void CloseSocket();

	//vars
	SOCKET m_socketID; //host socket ID regardless of server or client
	std::string m_name;

	//friend methods
	
	friend SOCKET BindSocketWithConnection(Connection conn);
	friend SOCKET ConnectToHostSocket(Connection conn);
};

inline void ConnectionSocket::CloseSocket() {

	closesocket(m_socketID);
	m_socketID = INVALID_SOCKET;

	std::cout << "\n[" << m_name << "]: disconnected...";


}

std::string AllocLocalHostName();
std::string GetLocalHostName();

//===========================================================================================================

struct ServerSocket : ConnectionSocket{

	//vars
	size_t m_maxConnections;

	//methods
	SOCKET AcceptOtherAddress(sockaddr* otherAddr, int otherAddrLen);

	void Listen();
	SOCKET Accept();

	void ServerLoop();

};

//===========================================================================================================

struct ClientSocket : ConnectionSocket {

	void ClientLoop(const std::string& message);

};

typedef std::vector<ClientSocket*> ClientSockets;

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers methods

std::string WindowsErrorAsString(DWORD error_id);
void* GetInAddress(sockaddr *sa);

//===========================================================================================================

#endif //__includedConnectionSocket__

