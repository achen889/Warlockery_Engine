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

	short port = 1777;
	if (theNetworkSystem) {
		if (COMMAND_HAS_ARGS) {
			if (NUMBER_OF_ARG_TOKENS >= 1) {
				port = (short)ARG_TOKEN_I(0);
			}
			

			theNetworkSystem->NetworkSystem::StartSession(NetworkSystem::s_gameSession, port);
		}
		else {
			port = 1777;

			theNetworkSystem->NetworkSystem::StartSession(NetworkSystem::s_gameSession, port);
		}

		NetworkSystem::s_gameSession->m_connSelf = NetworkSystem::s_gameSession->AddConnectionToSelf( 0xff, ShortToString(port));
		//EXECUTE_CONSOLE_COMMAND_STR("add_conn " +AllocLocalHostName()+ IntToString((int)port));
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

			 pingToConsole = "\naddr: " + netAddressToPing.ToString() + "\npacket["+debugPacketDat+"]";
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

CONSOLE_COMMAND(add_conn) {

	if (COMMAND_HAS_ARGS) {
		int numArgs = NUMBER_OF_ARG_TOKENS;
		std::string connAddrName;
		if (numArgs >= 1) {
			connAddrName = ARG_TOKEN_S(0);
		}
		std::string connPortService = "1777";
		if (numArgs >= 2) {
			connPortService = ARG_TOKEN_S(1);
		}
		Byte newConnIndex = 0xff; //use invalid if none specified
// 		if (numArgs >= 3) {
// 			newConnIndex = ARG_TOKEN_B(2);
// 		}

		NetAddresses netAddressesToConnect;
		AllocAddressesForHost(netAddressesToConnect, connAddrName, connPortService);

		std::string addConnDebugToConsole = "Adding Connection";

		int connsAdded = 0;
		for (NetAddressesIterator it = netAddressesToConnect.begin(); it != netAddressesToConnect.end(); ++it) {
			NetAddress& netAddressToConnect = (*it);

			NetworkSystem::s_gameSession->AddConnection(netAddressToConnect, newConnIndex);

			addConnDebugToConsole += "\nAdded Connection to " + netAddressToConnect.ToString();

			connsAdded++;
		}
		addConnDebugToConsole += "\nConnections Added: " + IntToString(connsAdded);
		
		OUTPUT_STRING_TO_CONSOLE(addConnDebugToConsole, 1000);
		ConsolePrintString(addConnDebugToConsole);
	}
}

//-----------------------------------------------------------------------------------------------------------

CONSOLE_COMMAND(testsend) {

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

CONSOLE_COMMAND(join_session) {

	if (COMMAND_HAS_ARGS) {

		int numArgs = NUMBER_OF_ARG_TOKENS;
		std::string connAddrName;
		if (numArgs >= 1) {
			connAddrName = ARG_TOKEN_S(0);
		}
		std::string connPortService = "1777";
		if (numArgs >= 2) {
			connPortService = ARG_TOKEN_S(1);
		}
		
		if(!NetworkSystem::s_gameSession)
			EXECUTE_CONSOLE_COMMAND_STR("create_session 1777");
		
		//conn for other
		EXECUTE_CONSOLE_COMMAND("add_conn " + connAddrName + " " + connPortService);

		//send 1 reliable join packet
		std::string myJoinIDString = AllocLocalHostName();
		if (numArgs >= 3) {
			myJoinIDString = ARG_TOKEN_S(2);
		}


		myJoinIDString += "\0";
		NetMessage* msg = new NetMessage(EN_MSG_ID_JOIN_REQUEST, (void*)myJoinIDString.c_str(), myJoinIDString.size() + 1);
		//msg->WRITE_BYTES(myJoinIDString);

		NetConnection* connToJoin = NetworkSystem::s_gameSession->FindConnectionInMap(connAddrName, connPortService, 0xff); //perhaps it's overwriting the entry in the map again
		if (connToJoin) {
			//ConsolePrintString("sending join msg => " + msg->ToString());
			connToJoin->SendNetMessage(*msg);
		}



	}

}

CONSOLE_COMMAND(host) {

		//int numArgs = NUMBER_OF_ARG_TOKENS;
	std::string connPortService = "1777";
	Byte numConnections = 8;
	if (COMMAND_HAS_ARGS) {

			if (NUMBER_OF_ARG_TOKENS >= 1) {
				connPortService = ARG_TOKEN_S(0);
			}
			   
			if(NUMBER_OF_ARG_TOKENS >= 2)
				numConnections = ARG_TOKEN_B(1);

			if(!NetworkSystem::s_gameSession)
			   EXECUTE_CONSOLE_COMMAND_STR("create_session " + connPortService);

			NetworkSystem::s_gameSession->StartHost(connPortService, numConnections);

	}
	else {
		if (!NetworkSystem::s_gameSession)
			EXECUTE_CONSOLE_COMMAND_STR("create_session " + connPortService);

			NetworkSystem::s_gameSession->StartHost(connPortService, numConnections);
	}
		


}

CONSOLE_COMMAND(start_test) {

	if (COMMAND_HAS_ARGS) {
		uint32_t messageCount = ARG_TOKEN_I(0);

		//std::string testSendString = "]__(O.o)((_|__|_";
		//testSendString += "\0";
// 		if (NUMBER_OF_ARG_TOKENS > 1) {
// 			testSendString = ARG_TOKEN_S(1);
// 		}

		NetMessage* startOrderTestMsg = new NetMessage (EN_MSG_ID_START_ORDER_TEST);
		//startOrderTestMsg.WRITE_BYTES(testSendString);
		startOrderTestMsg->WriteMessageData(&messageCount, SIZE_OF(uint32_t));

		NetworkSystem::s_gameSession->SendNetMessage(*startOrderTestMsg);

	}

}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///constructors

NetworkSystem::NetworkSystem() {
	if (!theNetworkSystem) {
		theNetworkSystem = this;
	}

	//network commands
	REGISTER_CONSOLE_COMMAND(create_session, "create_session <port>: creates a session on the port.");
	REGISTER_CONSOLE_COMMAND(ping, "ping <addr><port><message>: pings address with message.");
	REGISTER_CONSOLE_COMMAND(add_conn, "add conn <addr><port>: adds a connection to that addr and port.");
	REGISTER_CONSOLE_COMMAND(testsend, "test_send <msgCount>: sends msgCount messages to each connection.");
	REGISTER_CONSOLE_COMMAND(join_session, "join <addr><port>: joins addr at port ");
	REGISTER_CONSOLE_COMMAND(host, "host <port>: hosts session at port ");

	REGISTER_CONSOLE_COMMAND(start_test, "start_test <n>: tells all conns to send n InOrderTest msgs ");

	//network message callbacks
	REGISTER_MESSAGE_CALLBACK_OPTIONS(EN_MSG_ID_PING, NetPingCallback, MSG_OPTION_CONNECTIONLESS);
	REGISTER_MESSAGE_CALLBACK_OPTIONS(EN_MSG_ID_PONG, NetPongCallback, MSG_OPTION_CONNECTIONLESS);
	REGISTER_MESSAGE_CALLBACK(EN_MSG_ID_HEARTBEAT, NetHeartbeatCallback);

	REGISTER_MESSAGE_CALLBACK(EN_MSG_ID_ACK, NetProcessAckCallback);

	REGISTER_MESSAGE_CALLBACK_OPTIONS(EN_MSG_ID_JOIN_REQUEST, NetJoinRequestCallback, MSG_OPTION_RELIABLE);
	REGISTER_MESSAGE_CALLBACK_OPTIONS(EN_MSG_ID_JOIN_ACCEPT, NetJoinAcceptCallback, MSG_OPTION_RELIABLE);
	REGISTER_MESSAGE_CALLBACK_OPTIONS(EN_MSG_ID_JOIN_DENY, NetJoinDenyCallback, MSG_OPTION_CONNECTIONLESS);

	REGISTER_MESSAGE_CALLBACK_OPTIONS(EN_MSG_ID_START_ORDER_TEST, NetStartOrderTestCallback, MSG_OPTION_RELIABLE);
	REGISTER_MESSAGE_CALLBACK_OPTIONS(EN_MSG_ID_IN_ORDER_TEST, NetInOrderTestCallback, MSG_OPTION_RELIABLE | MSG_OPTION_IN_ORDER);
	REGISTER_MESSAGE_CALLBACK_OPTIONS(EN_MSG_ID_FORCE_ORDER_TEST, NetForceOrderTestCallback, MSG_OPTION_RELIABLE );

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
	if(NetworkSystem::s_gameSession)
	   NetworkSystem::s_gameSession->ShutDown();

	DestroySession(NetworkSystem::s_gameSession);

	WSACleanup();

}

//-----------------------------------------------------------------------------------------------------------

void NetworkSystem::Update(){
	if (NetworkSystem::s_gameSession) {
		NetworkSystem::s_gameSession->Update();
	}
}

//-----------------------------------------------------------------------------------------------------------

bool NetworkSystem::StartSession(NetworkSession*& hostSession, short port) {
	NetworkSession* session = theNetworkSystem->CreateSession();

	NetAddresses outAddressesForSession;

	if (session->CreateSocket(port)) { //error with console command here O.o
		hostSession = session;
		std::string hostSuccessString = "\nTest Game Session: " + hostSession->GetAddressStringForHostName();

		ConsolePrintf(hostSuccessString.c_str());
		OUTPUT_STRING_TO_CONSOLE(hostSuccessString, 1000);

		hostSession->Listen(true);
		return true;
	}
	else {
		theNetworkSystem->DestroySession(hostSession);
		std::string hostFailString = "\nFailed to Start Session.";

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


