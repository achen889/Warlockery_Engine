//==============================================================================================================
//NetworkSession.cpp
//by Albert Chen Jan-25-2016.
//==============================================================================================================

#include "NetworkSession.hpp"

#include "NetworkSystem.hpp"

#include <ws2tcpip.h>
#include <winnt.h>

#include "Engine/Networking/NetAddress.hpp"

//===========================================================================================================

//shuts down all the sockets and the conns
void NetworkSession::ShutDown(){
	for (NetConnectionMapIterator it = m_netConnectionMap.begin(); it != m_netConnectionMap.end(); ) {
		NetConnection* conn = (NetConnection*)(it->second);
		if (conn){
			it = m_netConnectionMap.erase(it);
		}
	}

	if (theNetworkSystem) {
		for (unsigned int i = 0; i < m_sockets.size(); i++){
			UDPSocket* sock = m_sockets[i];	
			theNetworkSystem->FreeSocket(sock);
		}
	}
	m_sockets.clear();
	m_listening = false;
}

//-----------------------------------------------------------------------------------------------------------

//starts session
bool NetworkSession::Host(short port){
	m_port = port;
	if (theNetworkSystem){
		
		UDPSocket* sock = theNetworkSystem->CreateUDPSocket(&m_packetQueue, port);

		m_netAddress = sock->m_netAddress;

		m_sockets.push_back(sock); //I think there's only one socket per session right now

		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

std::string NetworkSession::GetAddressStringForHostName(int addrFamily){
	addrinfo hints;
	addrinfo* addr;

	std::string host_name = AllocLocalHostName();

	std::string service = IntToString((int)m_port);

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = addrFamily; //get all addresses AF_INET or AF_INET6
	hints.ai_socktype = SOCK_STREAM; // stream based, determines transport layer TCP
	hints.ai_flags = AI_PASSIVE; // used for binding/listening

	int status = getaddrinfo(host_name.c_str(), service.c_str(), &hints, &addr);
	if (status != 0) {
		ConsolePrintf("Failed to create socket address! %s\n", (char*)gai_strerror(status));
	}

	std::string addressString = "\n===" + host_name + "===\n";

	addrinfo *iter;
	char addr_name[INET6_ADDRSTRLEN];

	for (iter = addr; iter != nullptr; iter = iter->ai_next) {
 		
 		PCSTR addressPCStr = inet_ntop(iter->ai_family, GetInAddress(iter->ai_addr), addr_name, INET6_ADDRSTRLEN);
		char* addressCStr = (char*)addressPCStr;

		addressString += "IP: ";
		addressString += addressCStr;
		addressString += " | PORT: " + service;
		addressString += "\n";
	}

	return addressString;
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///upkeep

void NetworkSession::Update(){

	//incoming?
	UpdateIncomingTraffic();

	static float secondsSinceLastUpdate = 0.0;
	if (secondsSinceLastUpdate < (1 / m_sendFreqHz) ) {
		secondsSinceLastUpdate += (float)GetDeltaSeconds();
	}
	else {
		//outgoing?
		UpdateSessionConnections();

		secondsSinceLastUpdate = 0.0f; //reset seconds since last
	}


}

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///update helpers

void NetworkSession::UpdateIncomingTraffic() {

	for (UDPSocketsIterator it = m_sockets.begin(); it != m_sockets.end(); ++ it ){
		UDPSocket* sock = (*it);
		if (sock) {
			//do stuff on sock
			//ConsolePrintString("Incoming on Socket[" + sock->m_netAddress.ToString() + "]");
			
			UpdateNetPacketQueueOnSocket(sock);
		}
	}

}

//-----------------------------------------------------------------------------------------------------------

void NetworkSession::UpdateNetPacketQueueOnSocket(UDPSocket* sock) {
		NetPacketQueue* socketNetPacketQueue = sock->GetNetPacketQueue();
		if (socketNetPacketQueue) {
			NetPacket* sockPacket = socketNetPacketQueue->DequeueRead(); //data is corrupted by this point already

			while(sockPacket != NULL) {

// 				std::string incomingPacketString = "\nNetSession=>Incoming Packet[" + sockPacket->ToStringAsBase(16) + "]";
// 				ConsolePrintString(incomingPacketString); 

				UpdateIncomingPacketOnSocket(sockPacket);

				delete sockPacket;
				//sockPacket = NULL;

				sockPacket = socketNetPacketQueue->DequeueRead();
			}//end of while
		}
}

//-----------------------------------------------------------------------------------------------------------

void NetworkSession::UpdateIncomingPacketOnSocket(NetPacket* sockPacket) {

	//do something with sockPacket
	if (sockPacket->IsValid()) {
		ConsolePrintString("packet validated!");

		
 		NetAddress* sockPacketAddr = sockPacket->GetAddress();

		if (sockPacketAddr) {
// 			std::string packetRecievedString = "\n===Received NetPacket for " + sockPacketAddr->ToString();
// 			packetRecievedString += "\n[" + sockPacket->ToStringAsBase(16) + "]";
// 			ConsolePrintString(packetRecievedString);

			//find connection on that addr
			NetConnection* netConnForPacket = FindConnectionInMap(*sockPacketAddr);
			if (netConnForPacket) {
				//break packet out into it's messages and add them to the connections 
				//ConsolePrintString("\ninConnection!!");
				ExecuteMessageCallbacksOnConnection(netConnForPacket, sockPacket);

			}
		}

	}//end of nested if
	
}

//-----------------------------------------------------------------------------------------------------------


void NetworkSession::ExecuteMessageCallbacksOnConnection(NetConnection* netConnForPacket, NetPacket* sockPacket ) {
	//copy messages into connection
	NetMessages& connOutMessages = netConnForPacket->m_outgoingMessages;
	GetAllMessages(connOutMessages, *sockPacket);

	//call the callbacks on all the messages
	for (NetMessagesIterator it = connOutMessages.begin(); it != connOutMessages.end(); ++it) {
		NetMessage& msg = (*it);

		NetMessageCallback outMessageCallback;
		msg.GetMessageCallback(outMessageCallback); //callback has crap data

		//sets the msg and conn
		//outMessageCallback.message = &msg;
		outMessageCallback.connection = netConnForPacket;

		//execute callback
		outMessageCallback.Execute();

	}
}



//-----------------------------------------------------------------------------------------------------------

void NetworkSession::UpdateSessionConnections(){
	if (m_netConnectionMap.empty()) return;

	for (NetConnectionMapIterator it = m_netConnectionMap.begin(); it != m_netConnectionMap.end(); ++ it ){
		NetConnection* conn = (NetConnection*)(it->second);
		UpdateSessionConnection(conn);
	}
}

//-----------------------------------------------------------------------------------------------------------

void NetworkSession::UpdateSessionConnection(NetConnection* conn) {
	if (conn) {
		conn->Update();
	}
}

//===========================================================================================================

//-----------------------------------------------------------------------------------------------------------

void NetworkSession::SendPacket(NetPacket* packet) {

	m_packetQueue.EnqueueWrite(packet);

}

//-----------------------------------------------------------------------------------------------------------

void NetworkSession::SendNetMessage(NetMessage &msg) {
	//sends msg to all connections
	for (NetConnectionMapIterator it = m_netConnectionMap.begin(); it != m_netConnectionMap.end(); ++ it ){
		NetConnection& conn = *(it->second);

		conn.SendNetMessage(msg);

	}


}

//-----------------------------------------------------------------------------------------------------------

void NetworkSession::AddConnection(const NetAddress &addr){
	m_netConnectionMap.insert(NetConnectionMapEntry(addr.ToString(), new NetConnection(addr)));

	m_netConnectionMap[addr.ToString()]->SetOwningSession(this);

}

//-----------------------------------------------------------------------------------------------------------

NetConnection* NetworkSession::FindConnectionInMap(const NetAddress&addr){
	NetConnectionMapIterator connToFind = m_netConnectionMap.find(addr.ToString());
	if (connToFind != m_netConnectionMap.end()) {
		return (NetConnection*)connToFind->second;
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------



//===========================================================================================================

