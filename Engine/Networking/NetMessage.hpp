//==============================================================================================================
//NetMessage.hpp
//by Albert Chen Feb-2-2016.
//==============================================================================================================

#pragma once

#ifndef _included_NetMessage__
#define _included_NetMessage__

#include "Engine/Core/BinaryUtils.hpp"

//===========================================================================================================

const size_t MESSAGE_MTU = 1024; //max byte size for message

struct NetPacket;

class NetConnection;

struct NetMessage;

typedef Byte NetMessageID;

//===========================================================================================================

//callback event
typedef void(NetMessageCallbackFunc)(NetConnection* conn, NetMessage& msg);

//register message callback macro
#define REGISTER_MESSAGE_CALLBACK(id, name ) static NetMessageCallback msg_callback_##name## = NetMessageCallback(&name, 0) //MUST BE USED IN CONSTRUCTOR

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///common net message callbacks

enum ENGINE_MSG_ID_DEF{
	EN_MSG_ID_PING = 0,
	EN_MSG_ID_PONG = 1,
	EN_MSG_ID_DEFS
};

void NetPingCallback(NetConnection* conn, NetMessage& msg); //id = 0

void NetPongCallback(NetConnection* conn, NetMessage& msg); //id = 1

//===========================================================================================================

struct NetMessageCallback {

	NetMessageCallback() {
		//do nothing
	}

	NetMessageCallback(NetConnection* conn, NetMessageCallbackFunc* func, NetMessage& data, NetMessageID newID );

	//use this one to register callbacks
	NetMessageCallback(NetMessageCallbackFunc* func, NetMessageID newID);

	~NetMessageCallback() {
		//do nothing
	}

	//methods
	void Execute();
	bool IsValid();

	friend void AddNetMessageCallbackToRegistry(NetMessageCallback* netMessageCallback);

	friend void FindNetMessageCallbackByMessageID(NetMessageCallback& outMessageCallback, NetMessageID id);

	friend bool IsNetMessageIDInRegistry(NetMessageID id);
	
	//vars
	NetMessageID id;
	NetMessageCallbackFunc* function;
	NetConnection* connection;
	NetMessage* message;

	static std::map<NetMessageID, NetMessageCallback*> s_netMessageCallbackRegistry;

};

typedef std::map<NetMessageID, NetMessageCallback*> NetMessageCallbackRegistry;
typedef std::map<NetMessageID, NetMessageCallback*>::iterator NetMessageCallbackRegistryIterator;
typedef std::pair<NetMessageID, NetMessageCallback*> NetMessageCallbackEntry;

//-----------------------------------------------------------------------------------------------------------
///----------------------------------------------------------------------------------------------------------
///inline methods

inline bool NetMessageCallback::IsValid() {
	//check ptrs are valid, and check if ID is in registry
	if (function && connection && message && IsNetMessageIDInRegistry(id)) {
		return true;
	}
	return false;
}

//===========================================================================================================

struct NetMessage;

typedef std::vector<NetMessage> NetMessages;
typedef std::vector<NetMessage>::iterator NetMessagesIterator;

struct NetMessage : ByteBuffer {

	NetMessage(Byte id);
	NetMessage(Byte id, void* data, size_t dataLen);
	NetMessage(NetPacket& packet);

	const unsigned short GetMessageLength() const { return messageLength;  }
	const Byte GetMessageID() const { return id; }

	//assumes the ID and length are written
	void WriteMessageData(void* data, size_t dataLen) {

		WriteBytes(data, dataLen);

		//increase message length
		messageLength += (unsigned short)dataLen;

// 		Byte* shortBytes = (Byte*)(&messageLength);
// 
// 		messageLengthPtr = shortBytes;

	}

	std::string ToString() {
		return "MSG[ ID: "+IntToString(GetMessageID()) + "| Length: " + IntToString(GetMessageLength()) +"| msg: "+ std::string((char*)buffer) +"]";
	}

	void GetMessageCallback(NetMessageCallback& outmessageCallback ) {

		FindNetMessageCallbackByMessageID(outmessageCallback, id);

		outmessageCallback.message = this;

	}

	friend void GetAllMessages(NetMessages& outNetMessages, NetPacket& packet);

	//vars
	NetMessageID id = 0;
	unsigned short messageLength = 3;
	Byte* messageLengthPtr;
	Byte messageBuffer[MESSAGE_MTU];

};




//===========================================================================================================

#endif //__includedNetMessage__

