//==============================================================================================================
//NetworkSystem.cpp
//by Albert Chen Jan-14-2016.
//==============================================================================================================

#include "NetworkSystem.hpp"
#include "Engine/Console/DevConsole.hpp"
#include "Engine/Multithreading/JobManager.hpp"

//===========================================================================================================

// CONSOLE_COMMAND(clientQuit) {
// 
// 	//need to get client thread reference
// 	printf("client will quit now!");
// 	if (theJobManager) {
// 		//add client quit job, pass in connection socket ref
// 		CloseConnectionSocketJob* job = new CloseConnectionSocketJob();
// 		theJobManager->AddPendingJob(job, PRIORITY_HIGH, NULL, JOB_TYPE_NET_CLIENT);
// 	}
// 
// }
// 
// //-----------------------------------------------------------------------------------------------------------
// 
// CONSOLE_COMMAND(serverQuit) {
// 
// 	//need to get server reference and all associated clients
// 	printf("server will quit now!");
// 
// }

//===========================================================================================================

bool NetworkSystem::StartUp(){

	//REGISTER_CONSOLE_COMMAND(clientQuit, "quits the active client.");
	//REGISTER_CONSOLE_COMMAND(serverQuit, "quits the active server and all associated clients.");

	WSADATA wsa_data;
	int error = WSAStartup(MAKEWORD(2, 2), &wsa_data);

	if (error == 0) {
		printf("WinSock Initialized!\n");
		return true;
	}
	else {
		//handle an error
		printf("Failed to initialize WinSock.  Error[%u]: %s\n", error, WindowsErrorAsString(error).c_str());
		return false;
	}

}

//-----------------------------------------------------------------------------------------------------------

void NetworkSystem::ShutDown(){

	WSACleanup();

}

//-----------------------------------------------------------------------------------------------------------

void NetworkSystem::PrintAddressesForHost(std::string& host_name, const std::string& service) {
	addrinfo hints;
	addrinfo* addr;

	if (host_name == "") {
		host_name = "local_host";
	}

	//which network layer it's using
	memset(&hints, 0, sizeof(hints)); //sets all hints to 0

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; // stream based, determines transport layer TCP
	hints.ai_flags = AI_PASSIVE; // used for binding/listening

	int status = getaddrinfo(host_name.c_str(), service.c_str(), &hints, &addr);
	if (status != 0) {
		printf("Failed to create socket address! %s\n", gai_strerror(status));
	}

	addrinfo *iter;
	for (iter = addr; iter != nullptr; iter = iter->ai_next) {
		char addr_name[INET6_ADDRSTRLEN];
		inet_ntop(iter->ai_family, GetInAddress(iter->ai_addr), addr_name, INET6_ADDRSTRLEN);
		printf("Address family[%i] type[%i] %s : %s\n", iter->ai_family, iter->ai_socktype, addr_name, service.c_str());
	}

	freeaddrinfo(addr);
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================


