//==============================================================================================================
//NetworkSystem.hpp
//by Albert Chen Jan-14-2016.
//==============================================================================================================

#pragma once

#ifndef _included_NetworkSystem__
#define _included_NetworkSystem__

//===========================================================================================================

#include "Engine/Core/Utilities.hpp"

#include "Engine/Networking/NetworkSession.hpp"

//===========================================================================================================

//===========================================================================================================

class NetworkSystem;
extern NetworkSystem* theNetworkSystem;

class NetworkSystem {
public:

	NetworkSystem();
	~NetworkSystem() {

		DestroySession(NetworkSystem::s_gameSession);

	}

	bool StartUp();
	void ShutDown();

	void Update();
	
	NetworkSession* CreateSession();
	void DestroySession(NetworkSession* session);

	static bool StartHost(NetworkSession*& hostSession, short port);

	UDPSocket* CreateUDPSocket(NetPacketQueue* queue, short port);

	void FreeSocket(UDPSocket* socket);

	static NetworkSession* s_gameSession;

};

///----------------------------------------------------------------------------------------------------------
///inline methods

inline UDPSocket* NetworkSystem::CreateUDPSocket(NetPacketQueue* queue, short port){
	UDPSocket* sock = new UDPSocket(queue, port); //mem leak here

	return sock;
}

//be sure to call join on the socket before freeing the memory, otherwise thread issues
inline void NetworkSystem::FreeSocket(UDPSocket* socket){
	if (socket){
		socket->Join();
	}
	socket->Stop();

	//then delete socket
	delete socket;
	socket = NULL;
}

//-----------------------------------------------------------------------------------------------------------

inline NetworkSession* NetworkSystem::CreateSession() {

	NetworkSession* session = new NetworkSession(); //mem leak here

	return session;

}

//-----------------------------------------------------------------------------------------------------------

inline void NetworkSystem::DestroySession(NetworkSession* session) {

	delete session;

	session = NULL;

}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

std::string WindowsErrorAsString(DWORD error_id);

void* GetInAddress(sockaddr *sa);

std::string AllocLocalHostName();

//===========================================================================================================

#endif //__includedNetworkSystem__

