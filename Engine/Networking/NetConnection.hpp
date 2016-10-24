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
#include "Engine\Core\Timer.hpp"
#include <queue>
#include "Engine/Core/BitArray.hpp"
#include "ReliableTracker.hpp"

//===========================================================================================================

//static const uint16_t MAX_ACK_ID = 4096;

class NetworkSession;

struct NetPacket;

typedef std::vector<NetPacket*> NetPackets;

static const double CONN_TIME_FOR_DISCONNECT = 30.0;
static const double CONN_HEARTBEAT_TICK = 2.5;
static const uint16_t CONN_RELIABLE_WINDOW_SIZE = 4096;
static const uint16_t CONN_MAX_RELIABLE_ID = 0xffff;
static const double CONN_NET_MESSAGE_RESEND_TIME = 0.1;
static const double CONN_TRACKER_MAX_AGE = 2.5;

enum ConnState {
	CONN_STATE_CONNECTED,
	CONN_STATE_DISCONNECTED,
	NUMBER_OF_CONN_STATES
};

//===========================================================================================================

class NetConnection {
public:
	//methods

	NetConnection(const NetAddress& conn_addr);

	~NetConnection();

	//upkeep
	void Update();
	void CleanupTrackers();
	void UpdateSendPacket();
	//upkeep helpers
	void UpdateSendPackets();

	void SendPacket(NetPacket* packet);

	void UpdatePacketsToSend(NetPackets& netPacketsToSend);

	void SetOwningSession(NetworkSession* owner) { m_owningSession = owner; }

	NetAddress& GetAddress() { return m_address; }

	//void SendPacket(NetPacket* packet);
	void SendNetMessage(NetMessage &msg);

	Byte GetConnIndex() { return m_connIndex; }

	void CheckForDisconnect();

	bool IsConnected() {
		if (m_state == CONN_STATE_CONNECTED) {
			return true;
		}
		return false;
	}

	void SetState(const ConnState& state) { m_state = state; }
	void IncrementAckID() { m_nextAckID++; }
	void IncrementReliableID() { m_nextReliableID++; }
	bool CanUseReliableID(uint16_t reliableID);

	ReliableTracker* FindAndRemoveTracker(uint16_t ack);
	void TrackAck(uint16_t ack) { 
		m_trackedAcks.push_back(ack);
		//m_trackers.push_back(new ReliableTracker(ack, )
	}

	void SetConnIndex(const Byte& idx) { m_connIndex = idx; }
	bool IsConnIndexValid() { return (m_connIndex != 0xff); }

	NetworkSession* GetOwningSession() {
		if (m_owningSession) {
			return m_owningSession;
		}
		return NULL;
	}

	void SetName(const std::string& name) { m_name = name; }

	std::string GetName() {
		if (m_name != "") {
			return m_name;
		}
		return m_address.ToString() + "_" + IntToString(m_connIndex);
	} //spits out the conn name or it's common map key

	//public vars
	NetMessages m_unsentUnreliables; //(initially empty)

	std::vector<ReliableTracker*> m_trackers;

	std::queue<NetMessage*> m_unsentReliables; //(initially empty)
	std::queue<NetMessage*> m_sentReliables;

	std::priority_queue<NetMessage>m_incomingOrderedReliables;

	std::vector<uint16_t> m_trackedAcks;

	TBitArray<CONN_MAX_RELIABLE_ID> m_sentReliableIDs;
	TBitArray<CONN_MAX_RELIABLE_ID> m_recievedReliableIDs;

	double m_timeRecievedPacket; //time since last recv packet

	double m_lastTimeRecievedPacket;

	uint16_t m_nextOutgoingOrderID = 0; //sending in order
	uint16_t m_nextIncomingOrderID = 0; //recv in order

private:
	//vars
	NetAddress m_address;
	uint16_t m_nextAckID = 0;
	ConnState m_state;
	uint16_t m_nextReliableID = 0;


	Byte m_connIndex = 0xff;

	std::string m_name = ""; //usually blank

	//float m_lastSentTime;

	NetworkSession* m_owningSession;

	Timer m_heartbeatTimer;
	
	uint16_t m_oldestRecvReliableID = 0;

};

typedef std::map<Byte, std::string> NetConnectionNameRegistry;
typedef std::map<Byte, std::string>::iterator NetConnectionNameRegistryIterator;

typedef std::map<std::string, NetConnection*> NetConnectionMap;
typedef std::map<std::string, NetConnection*>::iterator NetConnectionMapIterator;
typedef std::pair<std::string, NetConnection*> NetConnectionMapEntry;

//===========================================================================================================

#endif //__includedNetConnection__

