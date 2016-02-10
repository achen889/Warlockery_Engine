//==============================================================================================================
//NetPacketQueue.hpp
//by Albert Chen Jan-25-2016.
//==============================================================================================================

#pragma once


#ifndef _included_NetPacketQueue__
#define _included_NetPacketQueue__

#include "Engine/Multithreading/ThreadSafeQueue.hpp"

#include "NetPacket.hpp"

//===========================================================================================================

struct NetPacketQueue{
	
	//Enqueue/Dequeue methods
	void EnqueueWrite(NetPacket* packet ){
		if (packet) {
			outgoing.Enqueue(packet);
		}
	}

	void EnqueueWrite(Byte* buffer, size_t bufferLen, sockaddr* addr, size_t addrLen ){
		NetPacket* packet = new NetPacket();
		packet->WriteBytes(buffer, bufferLen); //manually set buffer
		packet->SetAddress(addr, addrLen);//manually set addr

		
		EnqueueWrite(packet);
	}

	NetPacket* DequeueWrite(){
		
		NetPacket* packet = NULL;
		if (outgoing.Dequeue(&packet)){
			return packet;
		}

		return NULL;

	}

	void EnqueueRead(NetPacket* packet){
		if (packet) {
			incoming.Enqueue(packet);
		}
	}

	void EnqueueRead(Byte* buffer, size_t len, sockaddr* addr, size_t addrLen){
		
		//std::string debugRecievedPacketBuffer = "\nEnqueueRead::Buffer[" + GetBufferAsNumbers(buffer, len, 16) + "]";
		///ConsolePrintString(debugRecievedPacketBuffer);
		
		//create a brand new packet with just the buffer in it
		NetPacket* packet = new NetPacket();
		packet->WriteBytes(buffer, len); //manually set buffer
		packet->SetAddress(addr, addrLen);//manually set addr

		//std::string enqueuePacketString = "\nEnqueueRead=>Packet[" + packet->ToStringAsBase(16) + "]";
		//ConsolePrintString(enqueuePacketString); //whatever was in the queue was bad...

		EnqueueRead(packet);
	}

	NetPacket* DequeueRead(){
		NetPacket* packet = NULL;
		if (incoming.Dequeue(&packet)){
			//std::string dequeuePacketString = "\nDequeueRead=>Packet["+packet->ToStringAsBase(16)+"]";
			//ConsolePrintString(dequeuePacketString); // data is right here too
			
			return packet;
		}

		return NULL;
	}

//private:
	//vars
	ThreadSafeQueue<NetPacket*> incoming;
	ThreadSafeQueue<NetPacket*> outgoing;

};


//===========================================================================================================

#endif //__includedNetPacketQueue__

