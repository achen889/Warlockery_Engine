//==============================================================================================================
//NetworkSession.hpp
//by Albert Chen Jan-25-2016.
//==============================================================================================================

#pragma once


#ifndef _included_NetworkSession__
#define _included_NetworkSession__

//===========================================================================================================

#include "UDPSocket.hpp"

#include "NetPacketQueue.hpp"
#include "NetConnection.hpp"

//===========================================================================================================

struct NetAddress;
struct NetSession;

struct NetSender{
	NetAddress* addr;
	NetConnection* conn;
	NetSession* session;
};

class NetworkSession{
public:
	NetworkSession(){
		m_listening = false;
	}
	~NetworkSession() {
		//m_netConnectionMap.clear();
	}
	
	void ShutDown();

	bool CreateSocket(short port);
	void StartHost(const std::string& connPortService, const Byte& numConnections = 8);

	void Listen(bool isListening) { m_listening = isListening; }
	std::string GetAddressStringForHostName(int addrFamily = 0);

	//upkeep
	void Update();
	//update helpers
	void UpdateIncomingTraffic();
	void UpdateNetPacketQueueOnSocket(UDPSocket* sock);
	void UpdateIncomingPacketOnSocket(NetPacket* sockPacket);

	void ExecuteMessageCallbacksFromSender(NetSender* netConnForPacket, NetPacket* sockPacket);

	void UpdateIncomingInOrderTrafficForConn(NetConnection* conn, NetMessages& incomingMessages, NetSender* netSenderForPacket);

	void ProcessMessage(NetSender* netConnForPacket, NetMessage& msg);

	void UpdateSessionConnections();
	void UpdateSessionConnection(NetConnection* conn);

	//helpers
	void SendPacket(NetPacket* packet);

	void SendNetMessage(NetMessage &msg);

	void AddConnection(const NetAddress&addr);

	void AddConnection(const NetAddress&addr, const Byte& connIndex );

	void AddConnection(const NetAddress&addr, const Byte& connIndex, const std::string& connName);
	bool RemoveConnection(const NetAddress&addr, const Byte& connIndex);

	NetConnection* AddConnectionToSelf(const Byte& connIndex = 0xff, const std::string& connPortService = "1777");

	std::string GetNetConnectionMapKey(const NetAddress& connAddr, const Byte& connIndex = 0xff );

	bool CanProcessMessage(NetMessage& msg, NetSender* conn);

	

	NetConnection* FindConnectionInMap(const NetAddress&addr, const Byte& connIndex);

	NetConnection* FindConnectionInMap(std::string& addrName, const std::string& portName, const Byte& connIndex);

	NetConnection* FindConnectionInMap(std::string& connName);

	NetConnection* FindConnectionInMap(const Byte& connIndex);

	std::string GetConnNameFromIndex(const Byte& idx);

	void SetSendFrequency(const float& hz) { m_sendFreqHz = hz; }

	bool CanAddNewConn() {
		if (m_joinable && (m_currentJoinedConnections < m_maxConnections)) {
			return true;
		}
		return false;
	}

	bool CanConnJoin(NetConnection* conn) {
		if (conn) {
			if (m_joinable && (m_currentJoinedConnections < m_maxConnections) ) {
				return true;
			}
		}
		return false;
	}
	bool IsConnSelf(NetConnection* conn) {
		return (conn == m_connSelf); 
	}

	Byte GetMaxConnections() { return m_maxConnections; }

	Byte GetNextJoinableConnIndex() {
		m_currentJoinableConnIndex++;

		return m_currentJoinableConnIndex;
	}

	NetPacketQueue m_packetQueue;
	NetAddress m_netAddress;

	NetConnection* m_connSelf = NULL;

private:

	float m_sendFreqHz;//hertz is cycles per second

	short m_port;
	
	bool m_listening;

	
	UDPSockets m_sockets;

	NetConnectionMap m_netConnectionMap;
	NetConnectionNameRegistry m_netConnNameRegistry;

	bool m_joinable;
	Byte m_currentJoinedConnections = 0;
	Byte m_maxConnections;
	Byte m_currentJoinableConnIndex = 0;
	
};

//===========================================================================================================


inline std::string NetworkSession::GetNetConnectionMapKey(const NetAddress& connAddr, const Byte& connIndex) {
	return connAddr.ToString() + "_" + IntToString(connIndex);
}

//-----------------------------------------------------------------------------------------------------------

inline std::string NetworkSession::GetConnNameFromIndex(const Byte& idx) {
	NetConnectionNameRegistryIterator connNameToFind = m_netConnNameRegistry.find(idx);
	if (connNameToFind != m_netConnNameRegistry.end()) {
		return connNameToFind->second;
	}
	return "";
}



//===========================================================================================================

#endif //__includedNetworkSession__

