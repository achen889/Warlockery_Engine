//==============================================================================================================
//NetworkSession.cpp
//by Albert Chen Jan-25-2016.
//==============================================================================================================

#include "NetworkSession.hpp"

#include "NetworkSystem.hpp"

#include <ws2tcpip.h>
#include <winnt.h>

#include "Engine/Networking/NetAddress.hpp"
#include "../Core/Clock.hpp"
#include "Engine/Console/DevConsole.hpp"

//===========================================================================================================

//shuts down all the sockets and the conns
void NetworkSession::ShutDown(){
	for (NetConnectionMapIterator it = m_netConnectionMap.begin(); it != m_netConnectionMap.end(); ) {
		NetConnection* conn = (NetConnection*)(it->second);
		if (conn){
			delete conn;
			conn = NULL;
			it = m_netConnectionMap.erase(it);
		}
	}

	m_netConnectionMap.clear();

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
bool NetworkSession::CreateSocket(short port){
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

void NetworkSession::StartHost(const std::string& connPortService, const Byte& numConnections) {
	
	//add connection to self at index 0

	//RemoveConnection(m_netAddress, 0xff);

	m_connSelf = AddConnectionToSelf(0, connPortService);
	m_connSelf->SetConnIndex(0);


	m_currentJoinableConnIndex = 0;
	m_joinable = true;
	m_maxConnections = numConnections;

	std::string hostDebugString;
	hostDebugString += "\nHosting on port: " + connPortService + "|  Max Conns: " + IntToString(numConnections);

	ConsolePrintString(hostDebugString);
	OUTPUT_STRING_TO_CONSOLE(hostDebugString, 1000);

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
	double deltaSeconds = GetSystemClock().GetDeltaSeconds();
	if (secondsSinceLastUpdate < (1 / m_sendFreqHz) ) {
		secondsSinceLastUpdate += (float)deltaSeconds;
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
		//ConsolePrintString("packet validated!");
		//ConsolePrintString("\n" + sockPacket->ToStringAsBase(16));
		
 		NetAddress* sockPacketAddr = sockPacket->GetAddress();

		if (sockPacketAddr) {

			NetSender packetSender;
			packetSender.addr = sockPacketAddr; //grab the name in the map
			packetSender.conn = FindConnectionInMap(sockPacket->GetConnIndex());
			packetSender.session = (NetSession*)this;

			std::string packetSenderConnName = GetNetConnectionMapKey(*sockPacket->GetAddress(), sockPacket->GetConnIndex());
			//packetSenderConnName = GetConnNameFromIndex(sockPacket->GetConnIndex());
			if (packetSender.conn) {
				packetSenderConnName = packetSender.conn->GetName();
			}			

			std::string packetRecievedString = "\n===Received NetPacket=== from [" + packetSenderConnName + "] =>";
			packetRecievedString += "packet[" + sockPacket->ToStringAsBase(16) + "]";

			if (packetSender.conn) {
				packetSender.conn->m_timeRecievedPacket = GetCurrentSeconds();
				//packetRecievedString += " => Recv Packet Time = " + DoubleToString(packetSender.conn->m_timeRecievedPacket);
			}

			//this is the most spam
			//ConsolePrintString(packetRecievedString);
			
			ExecuteMessageCallbacksFromSender(&packetSender, sockPacket);


// 			//find connection on that addr
// 			NetConnection* netConnForPacket = FindConnectionInMap(*sockPacketAddr);
// 			//bool trackAck = false;
// 			if (netConnForPacket) {
// 				//break packet out into it's messages and add them to the connections 
// 				//ConsolePrintString("\ninConnection!!");
// 
// 				netConnForPacket->m_timeRecievedPacket = GetCurrentSeconds();
// 
// 				ExecuteMessageCallbacksFromSender(netConnForPacket, sockPacket);
// 
// 			}
		}

	}//end of nested if
	
}

//-----------------------------------------------------------------------------------------------------------


void NetworkSession::ExecuteMessageCallbacksFromSender(NetSender* netSenderForPacket, NetPacket* sockPacket ) {
	//copy messages into connection
	NetMessages incomingMessages;// = netSenderForPacket->conn->m_unsentUnreliables;
	GetAllMessages(incomingMessages, *sockPacket);

	uint16_t ack = sockPacket->GetAckID();

	bool trackAck = false;


	NetConnection* conn = FindConnectionInMap(sockPacket->GetConnIndex());

	UpdateIncomingInOrderTrafficForConn(conn, incomingMessages, netSenderForPacket);
	
	//call the callbacks on all the messages
	for (NetMessagesIterator it = incomingMessages.begin(); it != incomingMessages.end(); ++it) {
		 NetMessage& msg = (*it);
		 
		 trackAck = trackAck || msg.IsReliable();

		//ConsolePrintString("\nReliableMsg =>"+msg.ToString());
		ProcessMessage(netSenderForPacket, msg);
		

	}//end of for

	if (trackAck && netSenderForPacket != nullptr) {
		if(netSenderForPacket->conn)
		   netSenderForPacket->conn->TrackAck(ack);
	}


}

//-----------------------------------------------------------------------------------------------------------

void NetworkSession::UpdateIncomingInOrderTrafficForConn(NetConnection* conn, NetMessages& incomingMessages, NetSender* netSenderForPacket) {
	if (conn) {
		//pop messages off the order list
		while (conn->m_incomingOrderedReliables.size() > 0) {
			NetMessage orderedMsg = (NetMessage)conn->m_incomingOrderedReliables.top();
			incomingMessages.push_back(orderedMsg);
			conn->m_incomingOrderedReliables.pop();
		}

		//add the msgs to the in order list
		for (NetMessagesIterator it = incomingMessages.begin(); it != incomingMessages.end(); ) {
			NetMessage& msg = (*it);
			if (msg.IsInOrder()) {
				std::string inOrderDebugString;
				inOrderDebugString += "conn: " + conn->GetName();

				inOrderDebugString += "nextIncomingOrderID: " + IntToString(conn->m_nextIncomingOrderID);

				//if match, process as normal and increment orderID
				if (msg.GetOrderID() == conn->m_nextIncomingOrderID) {
					
					conn->m_nextIncomingOrderID++;
					if (conn->m_nextIncomingOrderID > BIT(16)) {
						conn->m_nextIncomingOrderID = 0;
					}
					
					ProcessMessage(netSenderForPacket, msg);
					it = incomingMessages.erase(it);
					continue;
				}
				else {
					//save it off to process later
					conn->m_incomingOrderedReliables.push(msg);

					it = incomingMessages.erase(it);
				}//end of inner if/else

			}//end of if
			else {
				++it;
			}
		}//end of for

	}
}



//-----------------------------------------------------------------------------------------------------------

void NetworkSession::ProcessMessage(NetSender* netSenderForPacket, NetMessage& msg) {

	NetMessageCallback* outMessageCallback;
	outMessageCallback = msg.GetMessageCallback(netSenderForPacket); //callback has crap data
	
	if (outMessageCallback && CanProcessMessage(msg, netSenderForPacket)) {
		//sets the msg and conn
		//outMessageCallback->message = &msg;

		outMessageCallback->sender = netSenderForPacket;

		//execute callback
		outMessageCallback->Execute();

		//do reliable traffic

		if (netSenderForPacket->conn) {
			
			NetConnection* cp = netSenderForPacket->conn;

			//update the msg reliable ID
			if (cp && outMessageCallback->IsReliable()) {
				cp->m_recievedReliableIDs.Set(msg.reliableID);

				uint16_t reliable_id = msg.reliableID;
				uint16_t offset = 2 * CONN_RELIABLE_WINDOW_SIZE;
				if (offset > reliable_id) {
					reliable_id = CONN_MAX_RELIABLE_ID - (offset - reliable_id);
				}
				else {
					reliable_id = reliable_id - offset;
				}//end of if/else

				cp->m_recievedReliableIDs.Unset(reliable_id);
			}
		}
		
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
	//FindConnectionInMap(addr);
	//if(addr.ToString())
	NetConnectionMapIterator it = m_netConnectionMap.find(GetNetConnectionMapKey(addr, 0xff));
	if (it == m_netConnectionMap.end()) {
		m_currentJoinedConnections++;
		m_netConnectionMap.insert(NetConnectionMapEntry(GetNetConnectionMapKey(addr, 0xff), new NetConnection(addr)));
		m_netConnNameRegistry[0xff] = GetNetConnectionMapKey(addr, 0xff);
		m_netConnectionMap[GetNetConnectionMapKey(addr, 0xff)]->SetConnIndex(0xff);

		m_netConnectionMap[GetNetConnectionMapKey(addr, 0xff)]->SetOwningSession(this);
	}
	

}

//-----------------------------------------------------------------------------------------------------------

bool NetworkSession::RemoveConnection(const NetAddress&addr, const Byte& connIndex) {
	NetConnectionMapIterator it = m_netConnectionMap.find(GetNetConnectionMapKey(addr, connIndex));
	if (it != m_netConnectionMap.end()) {
		NetConnection* connection = it->second;
		if (connection) {
			connection->CleanupTrackers();
			delete connection;
			connection = NULL;
		}
		it = m_netConnectionMap.erase(it);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------

void NetworkSession::AddConnection(const NetAddress&addr, const Byte& connIndex) {

	NetConnectionMapIterator it = m_netConnectionMap.find(GetNetConnectionMapKey(addr, connIndex));
	if (it == m_netConnectionMap.end()) {
		m_currentJoinedConnections++;
		m_netConnectionMap.insert(NetConnectionMapEntry(GetNetConnectionMapKey(addr, connIndex), new NetConnection(addr)));
		m_netConnNameRegistry[connIndex] = GetNetConnectionMapKey(addr, connIndex);
		m_netConnectionMap[GetNetConnectionMapKey(addr, connIndex)]->SetOwningSession(this);

		m_netConnectionMap[GetNetConnectionMapKey(addr, connIndex)]->SetConnIndex(connIndex);
	}

}

void NetworkSession::AddConnection(const NetAddress&addr, const Byte& connIndex, const std::string& connName ) {

	NetConnectionMapIterator it = m_netConnectionMap.find(connName);
	if (it == m_netConnectionMap.end()) {
		m_currentJoinedConnections++;
		m_netConnectionMap.insert(NetConnectionMapEntry(connName, new NetConnection(addr)));
		m_netConnNameRegistry[connIndex] = connName;
		m_netConnectionMap[connName]->SetOwningSession(this);
		m_netConnectionMap[connName]->SetName(connName);
		m_netConnectionMap[connName]->SetConnIndex(connIndex);
	}
}

//-----------------------------------------------------------------------------------------------------------

NetConnection* NetworkSession::AddConnectionToSelf(const Byte& connIndex, const std::string& connPortService ) {

	static const std::string selfConnName = "self";

	NetAddresses netAddressesToConnect;
	std::string localHostName = AllocLocalHostName();
	AllocAddressesForHost(netAddressesToConnect, localHostName, connPortService );

	std::string addConnDebugToConsole = "Adding Connection to Self";

	AddConnection(netAddressesToConnect[0], connIndex, selfConnName);

	m_netConnNameRegistry[connIndex] = "self";

	addConnDebugToConsole += "\nAdded Connection to Self:" + netAddressesToConnect[0].ToString();

	OUTPUT_STRING_TO_CONSOLE(addConnDebugToConsole, 1000);
	ConsolePrintString(addConnDebugToConsole);

	
	return  m_netConnectionMap[selfConnName];


}

//-----------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------

bool NetworkSession::CanProcessMessage(NetMessage& msg, NetSender* sender ) {
	NetMessageCallback* msgCallback;
	msgCallback = msg.GetMessageCallback(sender);
	if (msgCallback && !(msgCallback->m_options == MSG_OPTION_CONNECTIONLESS) && msgCallback->sender == NULL) {
		return false;
	}

	//msg options reliable
	if (msg.IsReliable()) {
		if (msgCallback) {
			
			if (sender->conn) {
				NetConnection* conn = sender->conn;
				if (conn->m_recievedReliableIDs.Get(msg.reliableID) == true) {
					return false;
				}
			}

			return true;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------------------------------------

NetConnection* NetworkSession::FindConnectionInMap(const NetAddress&addr, const Byte& connIndex){
	NetConnectionMapIterator connToFind = m_netConnectionMap.find(GetNetConnectionMapKey(addr, connIndex));
	if (connToFind != m_netConnectionMap.end()) {
		return (NetConnection*)connToFind->second;
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

NetConnection* NetworkSession::FindConnectionInMap(std::string& addrName, const std::string& portName, const Byte& connIndex){
	NetAddresses netAddressesToConnect;
	AllocAddressesForHost(netAddressesToConnect, addrName, portName);
	if (netAddressesToConnect.empty())
		return NULL;

	return FindConnectionInMap(netAddressesToConnect[0], connIndex);

}

//-----------------------------------------------------------------------------------------------------------

NetConnection* NetworkSession::FindConnectionInMap(std::string& connName) {
	NetConnectionMapIterator connToFind = m_netConnectionMap.find(connName);
	if (connToFind != m_netConnectionMap.end()) {
		return (NetConnection*)connToFind->second;
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

NetConnection* NetworkSession::FindConnectionInMap(const Byte& connIndex) {
	std::string nameToFind = GetConnNameFromIndex(connIndex);
	if (nameToFind != "") {
		return (NetConnection*)FindConnectionInMap(nameToFind);
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------



//===========================================================================================================

