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

struct NetSender;

struct NetMessage;

typedef Byte NetMessageID;

//===========================================================================================================

//callback event
typedef void(NetMessageCallbackFunc)(NetSender* conn, NetMessage& msg);

//register message callback macro
#define REGISTER_MESSAGE_CALLBACK(id, name ) static NetMessageCallback msg_callback_##name## = NetMessageCallback(&name, id) //MUST BE USED IN CONSTRUCTOR
#define REGISTER_MESSAGE_CALLBACK_OPTIONS(id, name, option ) static NetMessageCallback msg_callback_##name## = NetMessageCallback(&name, id, option) //MUST BE USED IN CONSTRUCTOR

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///common net message callbacks

enum ENGINE_MSG_ID_DEF {
	EN_MSG_ID_PING = 0,
	EN_MSG_ID_PONG = 1,
	EN_MSG_ID_HEARTBEAT = 2,
	EN_MSG_ID_ACK = 3,
	EN_MSG_ID_JOIN_REQUEST = 4,
	EN_MSG_ID_JOIN_ACCEPT = 5,
	EN_MSG_ID_JOIN_DENY = 6,
	EN_MSG_ID_LEAVE = 7,

	EN_MSG_ID_START_ORDER_TEST = 8,
	EN_MSG_ID_IN_ORDER_TEST = 9,
	EN_MSG_ID_FORCE_ORDER_TEST = 10,

	NUMBER_OF_EN_MSG_ID_DEFS
};

enum NET_MESSAGE_OPTIONS {
	MSG_OPTION_RELIABLE = BIT(0),
	MSG_OPTION_CONNECTIONLESS = BIT(1),
	MSG_OPTION_IN_ORDER = BIT(2),

	NUMBER_OF_MSG_OPTIONS
};

void NetPingCallback(NetSender* conn, NetMessage& msg); //id = 0

void NetPongCallback(NetSender* conn, NetMessage& msg); //id = 1

void NetHeartbeatCallback(NetSender* sender, NetMessage& msg); //id = 2

void NetProcessAckCallback(NetSender* conn, NetMessage& msg); //id = 3

void NetJoinRequestCallback(NetSender* conn, NetMessage& msg); //id = 4

void NetJoinAcceptCallback(NetSender* conn, NetMessage& msg); //id = 5

void NetJoinDenyCallback(NetSender* conn, NetMessage& msg); //id = 6

void NetLeaveCallback(NetSender* conn, NetMessage& msg); //id = 7

void NetStartOrderTestCallback(NetSender* conn, NetMessage& msg); //id = 8

void NetInOrderTestCallback(NetSender* conn, NetMessage& msg); //id = 9

void NetForceOrderTestCallback(NetSender* conn, NetMessage& msg); //id = 10

//===========================================================================================================

struct NetMessageCallback {

	NetMessageCallback() {
		//do nothing
	}

	NetMessageCallback(NetSender* conn, NetMessageCallbackFunc* func, NetMessage& data, NetMessageID newID );

	//use this one to register callbacks
	NetMessageCallback(NetMessageCallbackFunc* func, NetMessageID newID, const uint32_t& options = 0 );

	~NetMessageCallback() {
		//do nothing
	}

	//methods
	void Execute();
	bool IsValid();
	bool IsReliable() { return IsBitSet(m_options, MSG_OPTION_RELIABLE); }
	bool IsInOrder() { return IsBitSet(m_options, MSG_OPTION_IN_ORDER); }

	friend void AddNetMessageCallbackToRegistry(NetMessageCallback* netMessageCallback);

	friend NetMessageCallback* FindNetMessageCallbackByMessageID( NetMessageID id);

	friend bool IsNetMessageIDInRegistry(NetMessageID id);
	
	//vars
	NetMessageID id;
	uint32_t m_options;
	NetMessageCallbackFunc* function;
	NetSender* sender;
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
	if (function && sender && message && IsNetMessageIDInRegistry(id)) {
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

	const unsigned short GetMessageLength() { return messageLength;  }
	const Byte GetMessageID() const { return id; }

	const size_t GetHeaderLength() {
		return 2 + 1 + (IsReliable() ? 2 : 0) + (IsInOrder() ? 2 : 0); //ternary operators
	}

	//assumes the ID and length are written
	void WriteMessageData(void* data, size_t dataLen) {

		WriteBytes(data, dataLen);
		
		//increase message length
		messageLength += (unsigned short)dataLen;

	}

	std::string ToString() {
		std::string debugString = "MSG[ ID: " + IntToString(GetMessageID()) + "| Length: " + IntToString(GetMessageLength()) + "| msg: " + std::string((char*)&messageBuffer[0]) + "]";
		return  debugString;
	}

	NetMessageCallback* GetMessageCallback(NetSender* senderForMsg = NULL);

	void SetOrderID(const uint16_t& newOrderID ) {
		orderID = newOrderID;
	}
	uint16_t NetMessage::GetOrderID() const { return orderID; }

	bool operator<(const NetMessage& msgToEqual) const {
		return ( GetOrderID() < msgToEqual.GetOrderID() );
	}

	bool IsReliable();
	bool IsInOrder();

	friend void GetAllMessages(NetMessages& outNetMessages, NetPacket& packet);

	//vars
	NetMessageID id = 0;
	unsigned short messageLength = 0;
	Byte* messageLengthPtr;
	Byte messageBuffer[MESSAGE_MTU];
	uint16_t reliableID;
	double lastSentTime;

	uint16_t orderID = 0;// only used for in order traffic

	NetPacket* owner;

};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///inline methods

inline NetMessageCallback* NetMessage::GetMessageCallback(NetSender* senderForMsg) {

	NetMessageCallback* outmessageCallback = FindNetMessageCallbackByMessageID(id);
	if (outmessageCallback) {
		//msg to this, conn if needed
		outmessageCallback->message = this;
		outmessageCallback->sender = senderForMsg;
	}

	return outmessageCallback;
}

inline bool NetMessage::IsReliable() {
	NetMessageCallback* msgCallback = GetMessageCallback();
	if (msgCallback) {
		return msgCallback->IsReliable();
	}
	return false;
}

inline bool NetMessage::IsInOrder() {
	NetMessageCallback* msgCallback = GetMessageCallback();
	if (msgCallback) {
		return msgCallback->IsInOrder();
	}
	return false;
}



//===========================================================================================================

#endif //__includedNetMessage__

