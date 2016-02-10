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

class NetworkSession{
public:
	NetworkSession(){
		m_listening = false;
	}
	~NetworkSession() {
		m_netConnectionMap.clear();
	}
	
	void ShutDown();

	bool Host(short port);
	void Listen(bool isListening){ m_listening = isListening;  }
	std::string GetAddressStringForHostName(int addrFamily = 0);

	//upkeep
	void Update();
	//update helpers
	void UpdateIncomingTraffic();
	void UpdateNetPacketQueueOnSocket(UDPSocket* sock);
	void UpdateIncomingPacketOnSocket(NetPacket* sockPacket);

	void ExecuteMessageCallbacksOnConnection(NetConnection* netConnForPacket, NetPacket* sockPacket);

	void UpdateSessionConnections();
	void UpdateSessionConnection(NetConnection* conn);

	//helpers
	void SendPacket(NetPacket* packet);

	void SendNetMessage(NetMessage &msg);

	void AddConnection(const NetAddress&addr);

	NetConnection* FindConnectionInMap(const NetAddress&addr);

	void SetSendFrequency(const float& hz) { m_sendFreqHz = hz; }

	NetPacketQueue m_packetQueue;
	NetAddress m_netAddress;

private:

	float m_sendFreqHz;//hertz is cycles per second

	short m_port;
	
	bool m_listening;
	UDPSockets m_sockets;

	NetConnectionMap m_netConnectionMap;
	

};


//===========================================================================================================

#endif //__includedNetworkSession__

