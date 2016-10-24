//==============================================================================================================
//NetPacket.cpp
//by Albert Chen Jan-25-2016.
//==============================================================================================================

#include "NetPacket.hpp"
#include "NetMessage.hpp"

#include "Engine/Multithreading/CriticalSection.hpp"

//===========================================================================================================

// CriticalSection* packetCritSec = new CriticalSection();

//===========================================================================================================

NetPacket::NetPacket() {

	Init();
}

NetPacket::NetPacket(const uint16_t& ackID, const Byte& connID) {

	Init();
	WriteInitHeaderBytes(ackID, connID);

}

//-----------------------------------------------------------------------------------------------------------

NetPacket::NetPacket(void* data, size_t dataLen){

	Init();
	WriteInitHeaderBytes();

	//simple ping data, dataLen
	Byte* dataAsByteBuffer = (Byte*)data;
	if (dataLen == 0) {
		dataLen = GetUCStrLength(dataAsByteBuffer); //somehow has to do this when len is 0
	}

	if (data != NULL) {
		NetMessage netMsg = NetMessage(0, data, dataLen);
		AddMessage(netMsg);
	}
	

	//ping data
	//WriteBytes(data, dataLen);
}

//-----------------------------------------------------------------------------------------------------------

NetPacket::NetPacket(void* data, size_t dataLen, const NetAddress& packetNetAddress) : NetPacket(data, dataLen){
	
	SetAddress(packetNetAddress);
	
}

//-----------------------------------------------------------------------------------------------------------

NetPacket::NetPacket(void* data, size_t dataLen, sockaddr* addr, size_t addrLen) : NetPacket(data, dataLen){
	
	SetAddress(addr, addrLen);

}

//-----------------------------------------------------------------------------------------------------------

void NetPacket::Init() {
	maxSize = PACKET_MTU;

	ByteBuffer::Set(packetBuffer, PACKET_MTU);
	memset(buffer, 0, PACKET_MTU);

	messageCountPtr = buffer + SIZE_OF_SHORT + SIZE_OF_CHAR;

	readMessagesPtr = messageCountPtr;
	++readMessagesPtr;
}

//-----------------------------------------------------------------------------------------------------------

void NetPacket::WriteInitHeaderBytes( const uint16_t& ackID, const Byte& connID ){
	//unsigned short ackID = ackID;
	
	WRITE_BYTES(connID);
	
	WRITE_BYTES(ackID);

	Byte messageCount = 0;
	WRITE_BYTES(messageCount);

	curSize += GetHeaderLength();
}

//-----------------------------------------------------------------------------------------------------------

const uint16_t NetPacket::GetAckID() {

	BinaryBufferParser bp(&packetBuffer[0], curSize);
	bp.ReadNextChar();
	return bp.ReadNextUShort();

}

//-----------------------------------------------------------------------------------------------------------

const Byte NetPacket::GetMessageCount() {
	if (messageCountPtr) {
		//ConsolePrintString(IntToString(*messageCountPtr));
		return *messageCountPtr;
	}
	
	return 0;

}

//-----------------------------------------------------------------------------------------------------------

bool NetPacket::IsValid() {
	
	//validate myself then validate others
	//BinaryBufferParser bp = BinaryBufferParser(buffer, curSize);
	unsigned short ackID = GetAckID();
	UNUSED(ackID);
	Byte messageCount = GetMessageCount();

// 	if (ackID == 0xffff) {
// 		return true;
// 	}

	int validMessageCount = 0;
	for(int i = 0; i < messageCount; i++){
		BinaryBufferParser bp(readMessagesPtr, curSize);
		unsigned short messageLen = bp.ReadNextUShort();
		Byte messageID = bp.ReadNextUChar();

		if (messageLen < MESSAGE_MTU) {
			if (IsNetMessageIDInRegistry(messageID)) {
				validMessageCount++;
			}
		}

		readMessagesPtr += messageLen;
		//make sure it ends at packetLength
		
		//everything after that in the buffer is messages
		//validate each messageIDsocketNetPacketQueue->DequeueRead();
	}

	//add all message lengths + 3 should be packet length


	//reset read messages
	readMessagesPtr = buffer + GetHeaderLength();

	if (validMessageCount == messageCount) {
		return true;
	}

	

	return false;
}

//-----------------------------------------------------------------------------------------------------------

bool NetPacket::AddMessage(NetMessage& msgToAdd) {
	//write this to the end of the buffer and update message count
	//ConsolePrintString(msgToAdd.ToString());

	msgToAdd.owner = this;

	unsigned short messageLen = msgToAdd.GetMessageLength() + (unsigned short)msgToAdd.GetHeaderLength();
	
	if (GetBufferLength() + messageLen < maxSize) {
		//write length
		WRITE_BYTES(messageLen);

		//write ID
		Byte messageID = msgToAdd.GetMessageID();
		WRITE_BYTES(messageID);

		//new bit: reliable id
		if (msgToAdd.IsReliable()) {
			WRITE_BYTES(msgToAdd.reliableID);
		}

		//new bits: order id
		if (msgToAdd.IsInOrder()) {
			WRITE_BYTES(msgToAdd.orderID);
		}

		//write message buffer data to my buffer
		WriteBytes((void*)msgToAdd.messageBuffer, msgToAdd.GetMessageLength());

		IncrementMessageCount();
		return true;
	}

	return false;
	
}

//-----------------------------------------------------------------------------------------------------------



//===========================================================================================================

