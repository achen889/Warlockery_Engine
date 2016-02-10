//==============================================================================================================
//UDPSocket.cpp
//by Albert Chen Jan-25-2016.
//==============================================================================================================

#include "UDPSocket.hpp"

#include "NetPacketQueue.hpp"

#include "Engine/Multithreading/Thread.hpp"
#include "Engine/Console/Console.hpp"

#ifdef _WINSOCKAPI_
#undef _WINSOCKAPI_
#endif
#include <Winsock2.h>

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///socket thread

struct UDPSocketThread : public Thread {

	//constructors
	UDPSocketThread() {
		//do nothing
	}
	~UDPSocketThread() {
		//do nothing
	}

	void Init(UDPSocket* myOwner, NetPacketQueue* queue, short newPort);

	virtual void Execute();

	//methods
	void CreateAndBindSocket();
	void ServiceSocket();

	std::string GetWSALastErrorString(int error);

	void ProcessIncoming();
	void ProcessOutgoing();

	NetAddress netAddress;
	//owned by a session, which it uses to read and write
	NetPacketQueue* packetQueue;

private:
	SOCKET socketID;
	short port;
	UDPSocket* owner;
	


};

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///socket helpers

void UDPSocketThread::Init(UDPSocket* myOwner, NetPacketQueue* queue, short newPort) {

	this->owner = myOwner;
	this->socketID = INVALID_SOCKET;
	this->port = newPort;
	this->packetQueue = queue;

	CreateAndBindSocket();
	
}

void UDPSocketThread::Execute() {
	
	if (socketID != INVALID_SOCKET) {
		ServiceSocket();
		closesocket(socketID);
	}
}


//-----------------------------------------------------------------------------------------------------------

void UDPSocketThread::CreateAndBindSocket() {
	SOCKET sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	ASSERT_RECOVERABLE(sock != INVALID_SOCKET, "Error: Socket is Invalid."); //do this better later
	
	//bind socket to whatever address is available, on the port supplied.
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //address to use for bind
	addr.sin_port = htons(port); //port to bind to
	addr.sin_family = PF_INET; //ipv4

	//bind address here
	sockaddr* bindAddr = (sockaddr*)&addr;
	int status = bind(sock, bindAddr, (int)SIZE_OF(addr)); //how to get the actual bound address out of this...
	if (status == SOCKET_ERROR) {
		ERROR_RECOVERABLE("Failed to bind address");//WSAGetError
		closesocket(sock);
		return;
	}
	netAddress.Init((sockaddr*)&bindAddr, SIZE_OF(addr)); //the IP is all zeros here
	//owner->m_netAddress.Start(bindAddr, SIZE_OF(addr));
	
	socketID = sock;

	//set to non blocking
	unsigned long nonBlocking = 1;
	ioctlsocket(socketID, FIONBIO, &nonBlocking);

}

//-----------------------------------------------------------------------------------------------------------

void UDPSocketThread::ServiceSocket() {

	while (owner->IsRunning()) {

		ProcessIncoming();

		ProcessOutgoing();

		Thread::Wait();
	}

}

//-----------------------------------------------------------------------------------------------------------

std::string UDPSocketThread::GetWSALastErrorString(int error ) {
	std::string netErrorString = "Network Error Code[";
	//int error = WSAGetLastError();
	switch (error) {
	case WSAEWOULDBLOCK:
		//nothing was read
		netErrorString += "WSAEWOULDBLOCK]: Nothing to read here!\n";
		break;
	case WSAEMSGSIZE:
		netErrorString += "WSAEMSGSIZE]: Message too large for buffer, thrown out!\n";
		//was too large truncated throw out
		break;
	default:
		netErrorString += IntToString(error) + "] unknown error!\n";
		break;
	};//end of switch

	return netErrorString;
}

//-----------------------------------------------------------------------------------------------------------

void UDPSocketThread::ProcessIncoming() {

	Byte buffer[PACKET_MTU];
	//memset(buffer, 0, PACKET_MTU);

	sockaddr_storage otherAddr;

	while (owner->IsRunning()) {

		memset(buffer, 0, PACKET_MTU);

		size_t addrLen = SIZE_OF(otherAddr);
		int recvd = recvfrom(socketID,  //socket we're reading on
			(char*)buffer,
			PACKET_MTU, //max buffer size
			0,	//options
			(sockaddr*)&otherAddr, //address we received
			(int*)&addrLen); //length of address
		
		if (recvd > 0) {

			//when the packet is here, it still has waffle, but somehow the mem gets screwy in time
			//std::string debugRecievedPacketBuffer = "\nUDPSocket=>Received[" + GetBufferAsNumbers(buffer, (size_t)recvd, 16) + "]";
			//ConsolePrintString(debugRecievedPacketBuffer); //ff1900waffle turns out it was there all along..so it screwed up later...

			packetQueue->EnqueueRead(&buffer[0], recvd, (sockaddr*)&otherAddr, addrLen);
		
		}
		else if (recvd < 0) {
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK) {
				//do nothing
			}
			else {
				ConsolePrintString(GetWSALastErrorString(error));
				owner->Stop();
			}

			return;
		}//end of else if
		else {
			owner->Stop();
		}
	}
}

//-----------------------------------------------------------------------------------------------------------

void UDPSocketThread::ProcessOutgoing() {

	NetPacket* packet = packetQueue->DequeueWrite();
	while (packet != NULL) {

		NetAddress* address = packet->GetAddress();
		sendto(socketID,
			(char*)packet->GetBuffer(),
			packet->writeIndex,
			0,
			(sockaddr*)&address->addr,
			address->addrLen);

		delete packet;
		packet = packetQueue->DequeueWrite();
	}//end of while

}

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

UDPSocket::UDPSocket(){
	//do nothing
}

UDPSocket::UDPSocket(NetPacketQueue* queue, short port){

	m_isRunning = true;
	m_serviceThread = new UDPSocketThread(); //mem leak here
	m_serviceThread->Init(this, queue, port);

	m_netAddress = m_serviceThread->netAddress;

	static size_t numServiceThreads = 0;

	Thread* thread = (Thread*)m_serviceThread;
	thread->Thread::Start("Net Service Thread " + IntToString(numServiceThreads));
	numServiceThreads++;

}

UDPSocket::~UDPSocket(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

void UDPSocket::Join() { 
	Thread* thread = (Thread*)m_serviceThread;
	thread->Join();
}

//-----------------------------------------------------------------------------------------------------------

NetPacketQueue* UDPSocket::GetNetPacketQueue() {
	return m_serviceThread->packetQueue;
}

//===========================================================================================================

