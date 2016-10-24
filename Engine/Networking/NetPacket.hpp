//==============================================================================================================
//NetPacket.hpp
//by Albert Chen Jan-25-2016.
//==============================================================================================================

#pragma once

#ifndef _included_NetPacket__
#define _included_NetPacket__

#include "NetAddress.hpp"
#include "Engine/Core/BinaryUtils.hpp"
#include <stdint.h>

//===========================================================================================================

const size_t PACKET_MTU = 1400; //maximum transmission unit for packets

//extern CriticalSection* packetCritSec; //crit sec for when packets are in use

struct NetMessage;

//===========================================================================================================

struct NetPacket : ByteBuffer{
	
	//constructors
	NetPacket();	
	NetPacket(const uint16_t& ackID, const Byte& connID = 0xff );

	~NetPacket() {
		//do nothing
		//ByteBuffer::~ByteBuffer(); 
	}

	NetPacket(void* data, size_t dataLen);
	NetPacket(void* data, size_t dataLen, const NetAddress& packetNetAddress);
	NetPacket(void* data, size_t dataLen, sockaddr* addr, size_t addrLen);

	void Init();

	void WriteInitHeaderBytes(const uint16_t& ackID = 0xffff, const Byte& connID = 0xff );

	void SetAddress(sockaddr* addr, size_t addrLen) {
		m_address.Init(addr, addrLen);
	}

	void SetAddress(const NetAddress& packetNetAddress) {
		SetAddress((sockaddr*)&packetNetAddress.addr, packetNetAddress.addrLen);
	}

	NetAddress* GetAddress(){ return (NetAddress*)&m_address; }

	const uint16_t GetAckID();

	const Byte GetConnIndex() {
		return packetBuffer[0];
	}

	const Byte GetMessageCount();

	Byte IncrementMessageCount() {
 		Byte& messageCountByte = *messageCountPtr; //couldn't read message count ptr, packet's probably deleted or something

 		messageCountByte++;
		
		messageCountPtr = &messageCountByte;

		return *messageCountPtr;
	}

	bool IsValid();

	size_t GetLength() { return (size_t)*messageCountPtr; }
	size_t GetHeaderLength() { return SIZE_OF_SHORT + SIZE_OF_CHAR + SIZE_OF_CHAR; }
	
	Byte* GetBuffer() { /*buffer[writeIndex] = (Byte)'/0';*/ return buffer; }

	bool AddMessage( NetMessage& msgToAdd); //write this to the end of the buffer and update message count
	
	bool ValidatePacketSize() {
		if (curSize < maxSize) {
			return true;
		}
		return false;
	}

	Byte* readMessagesPtr;

private:
	//vars
	Byte packetBuffer[PACKET_MTU];
	NetAddress m_address;
	Byte* messageCountPtr;
	
};

typedef std::vector<NetPacket*> NetPackets;
typedef std::vector<NetPacket*>::iterator NetPacketsIterator;

//===========================================================================================================

#endif //__includedNetPacket__

