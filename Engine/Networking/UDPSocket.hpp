//==============================================================================================================
//UDPSocket.hpp
//by Albert Chen Jan-25-2016.
//==============================================================================================================

#pragma once

#ifndef _included_UDPSocket__
#define _included_UDPSocket__

//===========================================================================================================

#include "Engine/Core/Utilities.hpp"

#include "Engine/Networking/NetAddress.hpp"

//===========================================================================================================

//===========================================================================================================

struct UDPSocketThread;

struct NetPacketQueue;

struct UDPSocket{
	
	//constructors
	UDPSocket();
	~UDPSocket();
	UDPSocket(NetPacketQueue* queue, short port);

	//methods
	bool IsRunning(){ return m_isRunning; }
	void Stop(){ m_isRunning = false; }
	void Join();
	NetPacketQueue* GetNetPacketQueue();
	
	//vars
	bool m_isRunning;
	UDPSocketThread* m_serviceThread;

	NetAddress m_netAddress;

};

typedef std::vector<UDPSocket*> UDPSockets;
typedef std::vector<UDPSocket*>::iterator UDPSocketsIterator;

//===========================================================================================================

#endif //__includedUDPSocket__

