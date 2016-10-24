//==============================================================================================================
//NetMessage.cpp
//by Albert Chen Feb-2-2016.
//==============================================================================================================

#include "NetMessage.hpp"
#include "NetConnection.hpp"
#include "NetPacket.hpp"
#include "Engine/Renderer/Text/TextSystem.hpp"
#include "NetworkSession.hpp"
#include "NetworkSystem.hpp"

std::map<NetMessageID, NetMessageCallback*> NetMessageCallback::s_netMessageCallbackRegistry;

//===========================================================================================================

void NetPingCallback(NetSender* sender, NetMessage& msg) {
	std::string netPingString = "\nNetPingCallback => Conn: " + sender->addr->ToString()+" " + msg.ToString()+"\n";

	ConsolePrintString(netPingString);
	OUTPUT_STRING_TO_CONSOLE(netPingString, 1000);

}

void NetPongCallback(NetSender* sender, NetMessage& msg) {
	std::string netPingString = "\nNetPongCallback  => Conn: " + sender->addr->ToString() +"\n "+ msg.ToString()+"\n";

	ConsolePrintString(netPingString);
	OUTPUT_STRING_TO_CONSOLE(netPingString, 1000);

}

void NetHeartbeatCallback(NetSender* sender, NetMessage& msg) {
	UNUSED(sender);
	UNUSED(msg);
	//do nothing
}

void NetJoinRequestCallback(NetSender* sender, NetMessage& msg) {
	//do nothing
	ConsolePrintString("\nNetJoinRequestCallback => ");

	BinaryBufferParser bp(&msg.messageBuffer[0], msg.curSize);
	std::string otherName = bp.ReadNextString();
	ConsolePrintString(otherName);

	if (NetworkSystem::s_gameSession->CanAddNewConn()) {
		Byte nextConnIndex = NetworkSystem::s_gameSession->GetNextJoinableConnIndex();
		NetworkSystem::s_gameSession->AddConnection(*sender->addr, 0xff, "client_"+ShortToString(nextConnIndex)+"(O.o)((_ | __ | _");

		//sender->conn = NetworkSystem::s_gameSession->FindConnectionInMap(nextConnIndex);
		NetConnection* newClientConnPtr = NetworkSystem::s_gameSession->FindConnectionInMap(0xff);
		if (newClientConnPtr) {
			NetMessage* joinAcceptMsg = new NetMessage(EN_MSG_ID_JOIN_ACCEPT);
			Byte myMaxConns = NetworkSystem::s_gameSession->GetMaxConnections();
			joinAcceptMsg->WRITE_BYTES(myMaxConns);
			joinAcceptMsg->WRITE_BYTES(nextConnIndex);

			if (NetworkSystem::s_gameSession->m_connSelf) {

				Byte myConnIndex = NetworkSystem::s_gameSession->m_connSelf->GetConnIndex();

				joinAcceptMsg->WRITE_BYTES(myConnIndex);
				//joinAcceptMsg->WriteMessageData(&myConnIndex, SIZE_OF(myConnIndex));

				//joinAcceptMsg->WriteMessageData(&std::string(AllocLocalHostName() + "__(O.o)((_|__|_\0"), SIZE_OF(std::string));
				std::string hostNameString = AllocLocalHostName();
				joinAcceptMsg->WRITE_BYTES(hostNameString);

			}//end of if my info exists

			//doing sender->conn causes system to spam new conns

			newClientConnPtr->SendNetMessage(*joinAcceptMsg);

			//newClientConnPtr->SetConnIndex(0xff);

			newClientConnPtr->SetName("client_" + ShortToString(nextConnIndex) + "(O.o)((_ | __ | _");
		}
		
	}
	else {
		NetMessage* joinDenyMsg = new NetMessage(EN_MSG_ID_JOIN_DENY);

		NetworkSystem::s_gameSession->SendNetMessage(*joinDenyMsg);
	}
	

}

void NetProcessAckCallback(NetSender* sender, NetMessage& msg) {
	
	std::string ackDebugString = "ackCount = ";

	if (sender) {
		uint8_t ackCount;

		BinaryBufferParser bp(msg.messageBuffer, msg.GetMessageLength() + msg.GetHeaderLength());
		//ConsolePrintString(msg.ToStringAsBase(16)); 
		uint16_t msgLen = bp.ReadNextUShort();
		UNUSED(msgLen);
		Byte msgType = bp.ReadNextUChar();
		UNUSED(msgType);
		uint16_t msgReliableID;
		if(msg.IsReliable()) {
			msgReliableID = bp.ReadNextUShort();
		}
		
		ackCount = bp.ReadNextUChar();
		ackDebugString += IntToString(ackCount) + " acks[";

		//msg.ReadBytes(&ackCount, sizeof(uint8_t));

		uint16_t ack;

		while (ackCount > 0) {
			//msg.ReadBytes(&ack, sizeof(uint16_t));

			ack = bp.ReadNextUShort();
			ackDebugString += UShortToString(ack) +", ";

			//loop through if conn exists
			if (sender->conn) {
				ReliableTracker* tracker = sender->conn->FindAndRemoveTracker(ack);
				if (tracker) {
					for (size_t i = 0; i < tracker->reliableIDs.size(); i++) {
						uint16_t reliableID = tracker->reliableIDs[i];
						sender->conn->m_sentReliableIDs.Unset(reliableID);
						//conn->m_recievedReliableIDs.Set(reliableID);
					}
					delete tracker;
					tracker = NULL;
				}
				--ackCount;
			}
	
		}
		ackDebugString += "] ";

	}//end of if

	ConsolePrintString("\nNetProcessAckCallback => "+ackDebugString);

}

void NetJoinAcceptCallback(NetSender* sender, NetMessage& msg) {

	//sets temp sender conn
	//sender->conn = NetworkSystem::s_gameSession->FindConnectionInMap(*sender->addr, 0xff);

	std::string msgDebugString;
	//msgDebugString +=  "msg hex[" + msg.ToStringAsBase(16) +"] ";
	msgDebugString += msg.ToString() + " | ";

	BinaryBufferParser bp(msg.messageBuffer, msg.GetMessageLength()+msg.GetHeaderLength());
	uint16_t msgLen = bp.ReadNextUShort();
	UNUSED(msgLen);
	Byte msgType = bp.ReadNextUChar();
	UNUSED(msgType);

	uint16_t msgReliableID;
	if (msg.IsReliable()) {
		msgReliableID = bp.ReadNextUShort();
	}
	Byte maxConnsOnHost = bp.ReadNextUChar();
	UNUSED(maxConnsOnHost);
	Byte myConnIndex = bp.ReadNextUChar();

	Byte yourConnIndex = bp.ReadNextUChar();

	std::string yourName = bp.ReadNextString();

	//add new one
	//sender->conn
	//NetworkSystem::s_gameSession->AddConnection(*sender->addr, myConnIndex, "client_"+ShortToString(myConnIndex));
	
	//remove old one
	//NetworkSystem::s_gameSession->RemoveConnection(*sender->addr, 0xff);

	std::string newClientLinkName = "client_" + ShortToString(myConnIndex) + "_link_to_host_" + ShortToString(yourConnIndex);

	//set sender one to new one
	NetworkSystem::s_gameSession->AddConnection(*sender->addr, yourConnIndex, newClientLinkName);
	sender->conn = NetworkSystem::s_gameSession->FindConnectionInMap(newClientLinkName);
	//sender->conn = NetworkSystem::s_gameSession->FindConnectionInMap(*sender->addr, 0xff);
	if (sender->conn){ //conn is null when we get here

		sender->conn->SetName(newClientLinkName);
		sender->conn->SetConnIndex(myConnIndex);

// 		if (NetworkSystem::s_gameSession->m_connSelf) {
// 			NetworkSystem::s_gameSession->m_connSelf->SetName(AllocLocalHostName());
// 			NetworkSystem::s_gameSession->m_connSelf->SetConnIndex(myConnIndex);
// 		}
	}

	//NetworkSystem::s_gameSession->RemoveConnection(*sender->addr, 0xff);

	ConsolePrintString("\nNetJoinAcceptCallback => "+ msgDebugString+" connIndex = " +IntToString(myConnIndex));

}

void NetJoinDenyCallback(NetSender* conn, NetMessage& msg){
	UNUSED(conn);
	UNUSED(msg);

	//do nothing
	ConsolePrintString("\nNetJoinDenyCallback => ");
}

void NetLeaveCallback(NetSender* conn, NetMessage& msg) {
	UNUSED(conn);
	UNUSED(msg);

	ConsolePrintString("\nNetLeaveCallback => ");
}

//-----------------------------------------------------------------------------------------------------------

void NetStartOrderTestCallback(NetSender* conn, NetMessage& msg) {
	UNUSED(conn);
	UNUSED(msg);
	std::string orderTestDebugString = "\nNetStartOrderTestCallback =>";

	//this crashes
// 	BinaryBufferParser bp(msg.messageBuffer, msg.GetMessageLength() + msg.GetHeaderLength());
// 	uint16_t msgLen = bp.ReadNextUShort();
// 	Byte msgType = bp.ReadNextUChar();
// 
// 	uint16_t msgReliableID;
// 	if (msg.IsReliable()) {
// 		msgReliableID = bp.ReadNextUShort();
// 	}
// 	uint16_t msgOrderID;
// 	if (msg.IsInOrder()) {
// 		msgOrderID = bp.ReadNextUShort();
// 	}
// 	unsigned int messageCount = bp.ReadNextUInt();
// 
// 	//NetConnection* conn;
// 
// 	for (int i = 0; i < messageCount; i++) {
// 		NetMessage* msgTest = new NetMessage(EN_MSG_ID_IN_ORDER_TEST);
// 		msgTest->WRITE_BYTES(i);
// 		NetworkSystem::s_gameSession->SendNetMessage(msg);
// 	}

	

	ConsolePrintString( orderTestDebugString);
	OUTPUT_STRING_TO_CONSOLE(orderTestDebugString, 1000);

}


void NetInOrderTestCallback(NetSender* conn, NetMessage& msg) {
	UNUSED(conn);
	std::string orderTestDebugString = "\nNetInOrderTestCallback => ";

	BinaryBufferParser bp(msg.messageBuffer, msg.GetMessageLength() + msg.GetHeaderLength());
	uint16_t msgLen = bp.ReadNextUShort();
	UNUSED(msgLen);
	Byte msgType = bp.ReadNextUChar();
	UNUSED(msgType);

	uint16_t msgReliableID;
	if (msg.IsReliable()) {
		msgReliableID = bp.ReadNextUShort();
	}
	uint16_t msgOrderID;
	if (msg.IsInOrder()) {
		msgOrderID = bp.ReadNextUShort();
	}

	unsigned int msgTestNum = bp.ReadNextUInt();


	orderTestDebugString += UIntToString(msgTestNum);

	ConsolePrintString(orderTestDebugString);
	OUTPUT_STRING_TO_CONSOLE(orderTestDebugString, 1000);
}

void NetForceOrderTestCallback(NetSender* conn, NetMessage& msg) {
	UNUSED(conn);
	UNUSED(msg);
	std::string orderTestDebugString = "\nNetForceOrderTestCallback = > ";
	ConsolePrintString(orderTestDebugString);
	OUTPUT_STRING_TO_CONSOLE(orderTestDebugString, 1000);

}


//===========================================================================================================

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Net Message callback

NetMessageCallback::NetMessageCallback(NetSender* newSender, NetMessageCallbackFunc* func, NetMessage& data, NetMessageID newID) :
	sender(newSender),
	function(func),
	message((NetMessage*)&data),
	id(newID)
{
	
	

}

//-----------------------------------------------------------------------------------------------------------

NetMessageCallback::NetMessageCallback(NetMessageCallbackFunc* func, NetMessageID newID, const uint32_t& options):
	function(func),
	id(newID),
	m_options(options)
{

	//AddNetMessageCallbackToRegistry(this);
	NetMessageCallback::s_netMessageCallbackRegistry[newID] = this;

}

//-----------------------------------------------------------------------------------------------------------

void NetMessageCallback::Execute() {
	//if all the ptrs are valid
	if (IsValid()){
		function(sender, *message); //sender becomes null after function executed
	}
}

//-----------------------------------------------------------------------------------------------------------

void AddNetMessageCallbackToRegistry(NetMessageCallback* netMessageCallback) {
	//guarantees valid message callback
	if (netMessageCallback && netMessageCallback->IsValid()) {
		//add this callback to the net message callback registry
		NetMessageCallback::s_netMessageCallbackRegistry.insert(NetMessageCallbackEntry(netMessageCallback->id, netMessageCallback));
	}
	
}

//-----------------------------------------------------------------------------------------------------------

NetMessageCallback* FindNetMessageCallbackByMessageID(NetMessageID id){

	NetMessageCallbackRegistryIterator it = NetMessageCallback::s_netMessageCallbackRegistry.find(id);
	if( it != NetMessageCallback::s_netMessageCallbackRegistry.end()){
		NetMessageCallback* outMessageCallbackPtr = (NetMessageCallback*)(it->second);
		if (outMessageCallbackPtr) {
			return outMessageCallbackPtr;
		}
	}
	return NULL;

}

//-----------------------------------------------------------------------------------------------------------

bool IsNetMessageIDInRegistry(NetMessageID id) {
	NetMessageCallbackRegistryIterator it = NetMessageCallback::s_netMessageCallbackRegistry.find(id);
	if (it != NetMessageCallback::s_netMessageCallbackRegistry.end()) {
		return true;
	}
	return false;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

NetMessage::NetMessage(Byte messageID):
	id(messageID)
{

	maxSize = MESSAGE_MTU;

	//buffer = new Byte[MESSAGE_MTU];
	ByteBuffer::Set(messageBuffer, MESSAGE_MTU);
	memset(buffer, 0, MESSAGE_MTU);

// 	WRITE_BYTES(messageLength); //write length
// 
// 	WRITE_BYTES(id); //write messageID

}

//-----------------------------------------------------------------------------------------------------------

NetMessage::NetMessage(Byte id, void* data, size_t dataLen) : NetMessage(id) {
	
	WriteMessageData(data, dataLen);

}

//-----------------------------------------------------------------------------------------------------------

NetMessage::NetMessage(NetPacket& packet){
	UNUSED(packet);

}



//-----------------------------------------------------------------------------------------------------------



//===========================================================================================================


void GetAllMessages(NetMessages& outNetMessages, NetPacket& packet) {
	Byte messageCount = packet.GetMessageCount();
	//NetMessages outNetMessages;
	for (int i = 0; i < messageCount; i++) {

		BinaryBufferParser bp(packet.readMessagesPtr, packet.GetBufferLength() - packet.GetHeaderLength());
		unsigned short curMessageLength = bp.ReadNextUShort();
		Byte curMessageID = bp.ReadNextUChar();

		uint16_t msgReliableID = bp.ReadNextUShort();

		outNetMessages.push_back(NetMessage(curMessageID, packet.readMessagesPtr, curMessageLength - 1));
		uint16_t msgOrderID = 0;
		if (outNetMessages.back().IsInOrder()) {
			msgOrderID = bp.ReadNextUShort();
		}

		if (outNetMessages.back().IsReliable()) {
			outNetMessages.back().reliableID = msgReliableID;
		}

		if (outNetMessages.back().IsInOrder()) {
			outNetMessages.back().orderID = msgOrderID;
		}

		outNetMessages.back().owner = &packet;

		packet.readMessagesPtr += curMessageLength;

	}

}

//===========================================================================================================