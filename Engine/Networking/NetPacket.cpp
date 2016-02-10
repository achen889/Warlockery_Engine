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

//-----------------------------------------------------------------------------------------------------------

NetPacket::NetPacket(void* data, size_t dataLen){

	Init();
	WriteInitHeaderBytes();

	//simple ping data, dataLen
	Byte* dataAsByteBuffer = (Byte*)data;
	if (dataLen == 0) {
		dataLen = GetUCStrLength(dataAsByteBuffer); //somehow has to do this when len is 0
	}
	

	//ping message
	//NetMessage msg;
	//msg = NetMessage(0);
// 	NetMessage msg (0, data, dataLen);
// 	AddMessage(msg);

	if (data != NULL) {
		AddMessage(NetMessage(0, data, dataLen));
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

	messageCountPtr = buffer + SIZE_OF_SHORT;

	readMessagesPtr = messageCountPtr;
	++readMessagesPtr;
}

//-----------------------------------------------------------------------------------------------------------

void NetPacket::WriteInitHeaderBytes(){
	unsigned short ackID = (unsigned short)0xffff;
	WRITE_BYTES(ackID);

	Byte messageCount = 0;
	WRITE_BYTES(messageCount);

	curSize += SIZE_OF_SHORT + SIZE_OF_CHAR;
}

//-----------------------------------------------------------------------------------------------------------

const unsigned short NetPacket::GetAckID() {

	BinaryBufferParser bp(buffer, curSize);
	return bp.ReadNextUShort();

}

//-----------------------------------------------------------------------------------------------------------

const Byte NetPacket::GetMessageCount() {

	return *messageCountPtr;

}

//-----------------------------------------------------------------------------------------------------------

bool NetPacket::IsValid() {
	
	//validate myself then validate others
	//BinaryBufferParser bp = BinaryBufferParser(buffer, curSize);
	unsigned short ackID = GetAckID();
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
	readMessagesPtr = buffer + SIZE_OF_SHORT + SIZE_OF_CHAR;

	if (validMessageCount == messageCount) {
		return true;
	}

	

	//return false;
}

//-----------------------------------------------------------------------------------------------------------

bool NetPacket::AddMessage(const NetMessage& msgToAdd) {
	//write this to the end of the buffer and update message count
	
	unsigned short messageLen = msgToAdd.GetMessageLength();
	//const unsigned short messageHeaderLen = 3;
	if (messageLen < maxSize) { //somehow packet is null by this point or rather 0xccccccc
		//write length
		WRITE_BYTES(messageLen);

		//write ID
		Byte messageID = msgToAdd.GetMessageID();
		WRITE_BYTES(messageID);

		//write message buffer to my buffer
		WriteBytes((void*)msgToAdd.GetBuffer(), msgToAdd.GetBufferLength());

		IncrementMessageCount();
		return true;
	}

	return false;
	
}

//-----------------------------------------------------------------------------------------------------------



//===========================================================================================================

