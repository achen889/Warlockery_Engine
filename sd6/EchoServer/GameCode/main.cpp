//==============================================================================================================
//Main_Console.cpp
//by Albert Chen Jan-14-2016.
//==============================================================================================================

#pragma once

#include <iostream>

#include "Engine/Multithreading/JobManager.hpp"

#include "Engine/Networking/ConnectionSocket.hpp"
#include "Engine/Networking/NetworkSystem.hpp"
#include "Engine/Networking/ServerThread.hpp"
#include "Engine/Networking/ClientThread.hpp"

#include "Engine/Math/MathCommands.hpp"

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///console app helpers

#define COUT_SINGLE_BORDER std::cout << "----------------------------------------------------\n";
#define COUT_DOUBLE_BORDER std::cout << "====================================================\n";

//old
void ConnectionSocketTest(int i, std::string zergHost, std::string service) {

	if (i == 0) {
		printf("Run as Server...\n");

		ServerSocket c;
		c.BindSocketToHost(zergHost, service, AF_INET, AI_PASSIVE);

		c.ServerLoop();
	}
	else {
		printf("Run as Client...\n");

		ClientSocket d;
		d.ConnectSocketToHost(zergHost, service, AF_INET);

		d.ClientLoop("Spawn more overlords!");
	}

}

//===========================================================================================================

int main(int argc, char* argv[]){

	printf("====================ECHO SERVER======================\n");
	printf("==================by Albert Chen=====================\n");

	int i = 0;
// 	std::cout << "\nargc = " << IntToString(argc);
// 	printf("\nargv[1] = %s", argv[1]);
// 	printf("\nargv[2] = %s", argv[2]);

	if (argv[1] == NULL) {
		COUT_DOUBLE_BORDER;
		
		std::cout << "\nPlease enter an integer value(0 = server, 1+ = client ): ";
		std::cin >> i;

		COUT_DOUBLE_BORDER;
	}
	else if(CompareCStr(argv[1], "server") == 0){
		i = 0;
	}

	MathCommands mathCommands; //register math commands

	NetworkSystem zergNet;
	zergNet.StartUp();

	//using connection socket class
	std::string zergHost = GetLocalHostName();
	printf("\nHost Name: %s\n", zergHost.c_str());

	//zergHost = "localhost";	

	std::string service = "1234";//port is 1024 up to max u short
	zergNet.PrintAddressesForHost(zergHost, service);

	COUT_DOUBLE_BORDER;

	JobManager jobManager;
	jobManager.StartUp(0);
	
	static int serverThreadID = 0;
	static int clientThreadID = 0;
	Server* server;
	if (i == 0){
		std::cout << "Initializing Server Threads!\n";
		size_t maxConnections = 8;
		printf("server max connections[%d]\n", maxConnections);
		server = new Server(maxConnections);
	}
	else{

		std::cout << "Initializing Client Thread!\n";
		char clientHostBuffer[64];
		std::string hostName = "";
		if (argv[1] != NULL) {
			hostName = std::string(argv[1]);
		}
		else {
			std::cout << "\nPlease Enter Host Name: ";
			while (hostName == "") {				
				std::cin.getline(clientHostBuffer, 64);
				hostName = std::string(clientHostBuffer);
			}
		}
		//need to test connection first
		Connection testConnection = GetConnection(hostName, service);
		if (testConnection.DidSucceed()) {
			printf("Successfully connected to Host!\n");
		}
		else {
			printf("Failed to connect to Host!\n");
		}

		while (!testConnection.didSucceed) {
			std::cout << "\nPlease Enter Host Name: ";
			std::cin.getline(clientHostBuffer, 64);
			hostName = std::string(clientHostBuffer);

			testConnection = GetConnection(hostName, service);
			if (testConnection.DidSucceed()) {
				printf("Successfully connected to Host!\n");
			}
			else {
				printf("Failed to connect to Host!\n");
			}
		}
		char clientNameBuffer[64];
		std::string clientName = "";
		std::cout << "\nEnter Client Name: ";
		while (clientName == "") {
			std::cin.getline(clientNameBuffer, 64);
			clientName = std::string(clientNameBuffer);
		}
		
		//now to make a client thread
		std::cout << "Initializing Client Thread!\n";
		ClientThread* clientThread = new ClientThread(clientName, hostName, service);
		clientThread->manager = &jobManager;
		jobManager.m_threads.push_back(clientThread);
		jobManager.m_threads.back()->Start("Client Thread " + IntToString(clientThreadID));
		clientThreadID++;

	}

	while (jobManager.IsRunning()) {

		jobManager.Update();

	}

	if (i == 0) {
		//last thing to shut down server
		server->CloseSocket();
	}
	

	jobManager.ShutDown();
	
	zergNet.ShutDown();

	system("pause");
	
	return 0;
}
