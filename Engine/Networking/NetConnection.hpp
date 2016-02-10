//==============================================================================================================
//NetConnection.hpp
//by Albert Chen Feb-1-2016.
//==============================================================================================================

#pragma once

#ifndef _included_NetConnection__
#define _included_NetConnection__

#include "NetAddress.hpp"
#include "NetMessage.hpp"
#include <map>

//===========================================================================================================

//static const uint16_t MAX_ACK_ID = 4096;

class NetworkSession;

struct NetPacket;

typedef std::vector<NetPacket*> NetPackets;


//===========================================================================================================

class NetConnection {
public:
	//methods

	NetConnection(const NetAddress& conn_addr);

	//upkeep
	void Update();

	void UpdatePacketsToSend(NetPackets& netPacketsToSend);

	void SetOwningSession(NetworkSession* owner) { m_owningSession = owner; }

	NetAddress& GetAddress() { return m_address; }

	//void SendPacket(NetPacket* packet);
	void SendNetMessage(NetMessage &msg);

	NetMessages m_outgoingMessages; //(initially empty)

private:
	//vars
	NetAddress m_address;
	uint16_t m_nextAckId = 0;
	
	float m_lastSentTime;

	NetworkSession* m_owningSession;

};

typedef std::map<std::string, NetConnection*> NetConnectionMap;
typedef std::map<std::string, NetConnection*>::iterator NetConnectionMapIterator;
typedef std::pair<std::string, NetConnection*> NetConnectionMapEntry;

//===========================================================================================================

#endif //__includedNetConnection__

