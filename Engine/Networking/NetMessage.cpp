//==============================================================================================================
//NetMessage.cpp
//by Albert Chen Feb-2-2016.
//==============================================================================================================

#include "NetMessage.hpp"
#include "NetConnection.hpp"
#include "NetPacket.hpp"
#include "Engine/Renderer/Text/TextSystem.hpp"

std::map<NetMessageID, NetMessageCallback*> NetMessageCallback::s_netMessageCallbackRegistry;

//===========================================================================================================

void NetPingCallback(NetConnection* conn, NetMessage& msg) {
	std::string netPingString = "\nNetPingCallback => Conn: " + conn->GetAddress().ToString()+" " + msg.ToString()+"\n";

	ConsolePrintString(netPingString);
	OUTPUT_STRING_TO_CONSOLE(netPingString, 1000);

}

void NetPongCallback(NetConnection* conn, NetMessage& msg) {
	std::string netPingString = "\nNetPongCallback  => Conn: " + conn->GetAddress().ToString() +" "+ msg.ToString()+"\n";

	ConsolePrintString(netPingString);
	OUTPUT_STRING_TO_CONSOLE(netPingString, 1000);

}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///Net Message callback

NetMessageCallback::NetMessageCallback(NetConnection* conn, NetMessageCallbackFunc* func, NetMessage& data, NetMessageID newID) :
	connection(conn),
	function(func),
	message((NetMessage*)&data),
	id(newID)
{
	
	

}

//-----------------------------------------------------------------------------------------------------------

NetMessageCallback::NetMessageCallback(NetMessageCallbackFunc* func, NetMessageID newID):
	function(func),
	id(newID)
{

	//AddNetMessageCallbackToRegistry(this);
	NetMessageCallback::s_netMessageCallbackRegistry[newID] = this;

}

//-----------------------------------------------------------------------------------------------------------

void NetMessageCallback::Execute() {
	//if all the ptrs are valid
	if (IsValid()){
		function(connection, *message);
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

void FindNetMessageCallbackByMessageID(NetMessageCallback& outMessageCallback, NetMessageID id){

	NetMessageCallbackRegistryIterator it = NetMessageCallback::s_netMessageCallbackRegistry.find(id);
	if( it != NetMessageCallback::s_netMessageCallbackRegistry.end()){
		NetMessageCallback* outMessageCallbackPtr = (NetMessageCallback*)(it->second);
		if (outMessageCallbackPtr) {
			outMessageCallback = *outMessageCallbackPtr;
		}
	}

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
	Set(messageBuffer, MESSAGE_MTU);
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

		BinaryBufferParser bp(packet.readMessagesPtr, packet.GetBufferLength() - 3);
		unsigned short curMessageLength = bp.ReadNextUShort();
		Byte curMessageID = bp.ReadNextUChar();

		outNetMessages.push_back(NetMessage(curMessageID, packet.readMessagesPtr, curMessageLength - 1));
		packet.readMessagesPtr += curMessageLength;

	}

}

//===========================================================================================================