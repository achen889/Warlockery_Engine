//==============================================================================================================
//NetworkSystem.cpp
//by Albert Chen Jan-14-2016.
//==============================================================================================================

#include "NetworkSystem.hpp"
#include "Engine/Console/DevConsole.hpp"

#include <ws2tcpip.h>

#include "Engine/Multithreading/CriticalSection.hpp"

#pragma comment(lib, "ws2_32.lib")

NetworkSystem* theNetworkSystem = NULL;

NetworkSession* NetworkSystem::s_gameSession;

//===========================================================================================================

CONSOLE_COMMAND(create_session) {

	short port = 1234;
	if (theNetworkSystem) {
		if (COMMAND_HAS_ARGS) {
			port = (short)ARG_TOKEN_I(0);

			theNetworkSystem->NetworkSystem::StartHost(NetworkSystem::s_gameSession, port);
		}
		else {
			port = 1234;

			theNetworkSystem->NetworkSystem::StartHost(NetworkSystem::s_gameSession, port);
		}
	}
}

//-----------------------------------------------------------------------------------------------------------

CONSOLE_COMMAND(ping) {

	if (COMMAND_HAS_ARGS) {
		int numArgs = NUMBER_OF_ARG_TOKENS;
		std::string pingHostName = AllocLocalHostName();
		std::string pingPortService = "1234";
		if (numArgs >= 1) {
			pingHostName = ARG_TOKEN_S(0);
		}
		if (numArgs >= 2) {
			pingPortService = ARG_TOKEN_S(1);
		}
		
		NetAddresses netAddressesToPing;
		AllocAddressesForHost(netAddressesToPing, pingHostName, pingPortService);

		std::string pingMessage = "waffle";
		if (numArgs > 2) {
			pingMessage = ARG_TOKEN_S(2);
		}

		std::string pingToConsole = "\npinging " + pingHostName + " on port[" + pingPortService + "] with message[" + pingMessage + "]!";
		OUTPUT_STRING_TO_CONSOLE(pingToConsole, 1000);
		ConsolePrintString(pingToConsole);

		int pingsSent = 0;
		pingToConsole = "";
		//NetPacket packet = NetPacket((void*)pingMessage.c_str(), pingMessage.length());

		//std::string pingAttempt;

		NetPacket* packetPrototype = new NetPacket((void*)pingMessage.c_str(), pingMessage.length());
		std::string debugPacketDat = packetPrototype->ToStringAsBase(16);

		//spawn a net packet for each address on the outgoing queue of this computer's net session
		for (NetAddressesIterator it = netAddressesToPing.begin(); it != netAddressesToPing.end(); ++ it ){
			 NetAddress& netAddressToPing = (*it);

			 NetPacket* packet = new NetPacket(*packetPrototype);
			 packet->SetAddress(netAddressToPing);
			 NetworkSystem::s_gameSession->m_packetQueue.EnqueueWrite(packet);

			 pingToConsole = "\naddr: " + netAddressToPing.ToString() + " => packet["+debugPacketDat+"]";
			 OUTPUT_STRING_TO_CONSOLE(pingToConsole, 1000);
			 ConsolePrintString(pingToConsole);

			 pingsSent++;
		}//end of for

		pingToConsole = "\npings sent: " + IntToString(pingsSent)+"\n";
		
		OUTPUT_STRING_TO_CONSOLE(pingToConsole, 1000);
		ConsolePrintString(pingToConsole);
	}
}

//-----------------------------------------------------------------------------------------------------------

CONSOLE_COMMAND(add_connection) {

	if (COMMAND_HAS_ARGS) {
		int numArgs = NUMBER_OF_ARG_TOKENS;
		std::string connAddrName;
		if (numArgs >= 1) {
			connAddrName = ARG_TOKEN_S(0);
		}
		std::string connPortService = "1234";
		if (numArgs >= 2) {
			connPortService = ARG_TOKEN_S(1);
		}

		NetAddresses netAddressesToConnect;
		AllocAddressesForHost(netAddressesToConnect, connAddrName, connPortService);

		std::string addConnDebugToConsole = "Adding Connection";

		int connsAdded = 0;
		for (NetAddressesIterator it = netAddressesToConnect.begin(); it != netAddressesToConnect.end(); ++it) {
			NetAddress& netAddressToConnect = (*it);

			NetworkSystem::s_gameSession->AddConnection(netAddressToConnect);

			addConnDebugToConsole += "\nAdded Connection to " + netAddressToConnect.ToString();

			connsAdded++;
		}
		addConnDebugToConsole += "\nConnections Added: " + IntToString(connsAdded);
		
		OUTPUT_STRING_TO_CONSOLE(addConnDebugToConsole, 1000);
		ConsolePrintString(addConnDebugToConsole);
	}
}

//-----------------------------------------------------------------------------------------------------------

CONSOLE_COMMAND(test_send) {

	if (COMMAND_HAS_ARGS) {
		int messageCount = ARG_TOKEN_I(0);
		
		std::string testSendString = "waffle";
		if (NUMBER_OF_ARG_TOKENS > 1) {
			testSendString = ARG_TOKEN_S(1);
		}

		NetMessage testSendMsg((Byte)0, (void*)testSendString.c_str(), testSendString.length());

		//sends a number of messages to the connections
		for (int i = 0; i < messageCount; i++){
			NetworkSystem::s_gameSession->SendNetMessage(testSendMsg);
		}
	}

}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

NetworkSystem::NetworkSystem() {
	if (!theNetworkSystem) {
		theNetworkSystem = this;
	}

	REGISTER_CONSOLE_COMMAND(create_session, "create_session <port>: creates a session on the port.");
	REGISTER_CONSOLE_COMMAND(ping, "ping <addr><port><message>: pings address with message.");
	REGISTER_CONSOLE_COMMAND(add_connection, "add connection <addr><port>: adds a connection to that addr and port.");
	REGISTER_CONSOLE_COMMAND(test_send, "test_send <msgCount>: sends msgCount messages to each connection.");

	REGISTER_MESSAGE_CALLBACK(EN_MSG_ID_PING, NetPingCallback);
	REGISTER_MESSAGE_CALLBACK(EN_MSG_ID_PONG, NetPongCallback);

}


//-----------------------------------------------------------------------------------------------------------

bool NetworkSystem::StartUp(){

	ConsolePrintf("\n===Network System Start Up===\n");

	WSADATA wsa_data;
	int error = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (error == 0) {
		ConsolePrintf("WinSock Initialized!\n");
		return true;
	}
	else {
		//handle an error
		ConsolePrintf("Failed to initialize WinSock.  Error[%u]: %s\n", error, WindowsErrorAsString(error).c_str());
		return false;
	}

}

//-----------------------------------------------------------------------------------------------------------

void NetworkSystem::ShutDown(){

	NetworkSystem::s_gameSession->ShutDown();

	WSACleanup();

}

//-----------------------------------------------------------------------------------------------------------

void NetworkSystem::Update(){
	if (NetworkSystem::s_gameSession) {
		NetworkSystem::s_gameSession->Update();
	}
}

//-----------------------------------------------------------------------------------------------------------

bool NetworkSystem::StartHost(NetworkSession*& hostSession, short port) {
	NetworkSession* session = theNetworkSystem->CreateSession();
	if (session->Host(port)) { //error with console command here O.o
		hostSession = session;
		std::string hostSuccessString = "\nTest Game Hosted: " + hostSession->GetAddressStringForHostName();

		ConsolePrintf(hostSuccessString.c_str());
		OUTPUT_STRING_TO_CONSOLE(hostSuccessString, 1000);

		hostSession->Listen(true);
		return true;
	}
	else {
		theNetworkSystem->DestroySession(hostSession);
		std::string hostFailString = "\nFailed to Host.";

		ConsolePrintf(hostFailString.c_str());
		OUTPUT_STRING_TO_CONSOLE(hostFailString, 1000);

		return false;
	}
	return false;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

std::string WindowsErrorAsString(DWORD error_id) {
	if (error_id != 0) {
		LPSTR buffer;
		DWORD size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			error_id,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)(&buffer),
			0, NULL);

		std::string msg(buffer, size);
		LocalFree(buffer);

		return msg;
	}
	else {
		return "";
	}
}

//-----------------------------------------------------------------------------------------------------------

// get sockaddr, IPv4 or IPv6:
void* GetInAddress(sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((sockaddr_in*)sa)->sin_addr);
	}
	else {
		return &(((sockaddr_in6*)sa)->sin6_addr);
	}
}

//-----------------------------------------------------------------------------------------------------------

std::string AllocLocalHostName() {

	char buffer[256];

	//socket function get host name
	if (gethostname(buffer, 256) == SOCKET_ERROR) {
		return "";
	}

	std::string localHostName = std::string(buffer);

	return localHostName;
}

//===========================================================================================================


