//==============================================================================================================
//ConnectionSocket.cpp
//by Albert Chen Jan-14-2016.
//==============================================================================================================

#include "ConnectionSocket.hpp"
#include "Engine\Core\Utilities.hpp"

//===========================================================================================================

///----------------------------------------------------------------------------------------------------------
///constructors

ConnectionSocket::ConnectionSocket(){
	//do nothing
}

ConnectionSocket::~ConnectionSocket(){
	//do nothing
}

//-----------------------------------------------------------------------------------------------------------

//===========================================================================================================

std::string AllocLocalHostName() {

	char buffer[256];

	//socket function get host name
	if (gethostname(buffer, 256) == SOCKET_ERROR) {
		return "";
	}

	std::string localHostName = std::string(buffer);

	return localHostName;
}

//-----------------------------------------------------------------------------------------------------------

std::string GetLocalHostName() {
	std::string hostName = AllocLocalHostName();
	//set default host name
	if (hostName == "") {
		hostName = "localhost";
	}
	return hostName;
}

//-----------------------------------------------------------------------------------------------------------

Connection GetConnection(const std::string& host_name, const std::string& service, int addrFamily, int addrInfoFlags ) {
	addrinfo hints;
	addrinfo* addr;
	Connection c;
	c.didSucceed = true;

	memset(&hints, 0, sizeof(hints)); //sets all hints to 0
	hints.ai_family = addrFamily; //only get INET
	hints.ai_socktype = SOCK_STREAM; // stream based, determines transport layer TCP
	hints.ai_flags = addrInfoFlags; // used for binding/listening

	int status = getaddrinfo(host_name.c_str(), service.c_str(), &hints, &addr);
	if (status != 0) {
		c.didSucceed = false;
		printf("Failed to create socket address! \n");
	}
	
	c.addrInfo = addr;
	c.service = service;
	return c;
}

//-----------------------------------------------------------------------------------------------------------

bool Connection::CreateSocket(addrinfo* iter, SOCKET& hostSock) {
	char addr_name[INET6_ADDRSTRLEN];
	inet_ntop(iter->ai_family, GetInAddress(iter->ai_addr), addr_name, INET6_ADDRSTRLEN);
	printf("Trying to Bind Address family[%i] type[%i] %s : %s\n", iter->ai_family, iter->ai_socktype, addr_name, service.c_str());

	//create socket
	hostSock = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);
	if (hostSock == INVALID_SOCKET) {
		int error = WSAGetLastError();
		printf("Failed to bind: Socket Error[%i]\n", error);
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------------------------------------

SOCKET BindSocketWithConnection(Connection conn) {
	SOCKET hostSock = INVALID_SOCKET;

	addrinfo* iter;
	for (iter = conn.addrInfo; iter != nullptr; iter = iter->ai_next) {

		if (conn.CreateSocket(iter, hostSock)) {

			int result = bind(hostSock, iter->ai_addr, (int)(iter->ai_addrlen));
			
			if (result == SOCKET_ERROR) {
				closesocket(hostSock);
				printf("Failed to bind: Socket Error[%i]\n", GetLastError());
				continue;
			}

		}
		

		break;

	}//end of loop

	freeaddrinfo(conn.addrInfo);

	if (hostSock == INVALID_SOCKET) {
		printf("Could not create socket!\n");
	}

	printf("Socket bound...\n");
	return hostSock;
}

//-----------------------------------------------------------------------------------------------------------

bool ConnectionSocket::ConnectSocketToHost(std::string& hostName, const std::string& service, int addrFamily) {

	Connection host = GetConnection(hostName, service, addrFamily);

	m_socketID = ConnectToHostSocket(host);

	return true;

}

//-----------------------------------------------------------------------------------------------------------

bool ConnectionSocket::BindSocketToHost(std::string& hostName, const std::string& service, int addrFamily, int addrInfoFlags) {

	m_name = hostName;

	Connection host = GetConnection(hostName, service, addrFamily, addrInfoFlags);
	
	m_socketID = BindSocketWithConnection(host);

	return true;
}

//-----------------------------------------------------------------------------------------------------------

char* ConnectionSocket::RecieveBufferDataFromOtherSocket(SOCKET otherSocket) {
	//how much data was received
	char buffer[1024];
	//printf("Waiting to Receive...\n");
	int recvd = recv(otherSocket, buffer, 1024, 0);
	if (recvd > 0) {
		buffer[recvd] = NULL; //null terminate this
		printf("Received:[%s]\n", buffer);
		//send message back echo
		return buffer;
	}
	return NULL;
}

//-----------------------------------------------------------------------------------------------------------

void ConnectionSocket::SendBufferDataToOtherSocket(SOCKET otherSocket, const char* buffer) {
	
	int sent = send(otherSocket, buffer, GetCStrLength(buffer), 0);
	if (sent == SOCKET_ERROR) {
		std::cout << "[" << m_name.c_str() << "] failed to send: " << buffer << "\n";
	}
	printf("[%s] sent: %s\n", m_name.c_str(), buffer);
	//std::cout <<"["<< m_name.c_str() << "] sent: " << buffer << "\n";

}

//-----------------------------------------------------------------------------------------------------------

SOCKET ServerSocket::AcceptOtherAddress(sockaddr* otherAddr, int otherAddrLen) {
	printf("[%s]Waiting to Accept...\n", m_name.c_str());
	SOCKET otherSocket = accept(m_socketID, (sockaddr*)otherAddr, &otherAddrLen); //accept connection on my socket ID

	if (otherSocket == INVALID_SOCKET) {
		//add error message here
		printf("[%s]Failed to Accept!\n", m_name.c_str());
	}
	return otherSocket;
}

//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///connection socket friend functions

SOCKET ConnectToHostSocket(Connection conn) {
	SOCKET hostSock = INVALID_SOCKET;

	addrinfo* iter;
	for (iter = conn.addrInfo; iter != nullptr; iter = iter->ai_next) {

		if (conn.CreateSocket(iter, hostSock)) {

			int result = connect(hostSock, iter->ai_addr, (int)(iter->ai_addrlen));
			if (result == SOCKET_ERROR) {
				closesocket(hostSock);
				printf("Failed to connect: Socket Error[%i]\n", GetLastError());
				continue;
			}
			else {
				printf("Connected...\n");
			}

		}


		break;

	}//end of loop

	freeaddrinfo(conn.addrInfo);

	if (hostSock == INVALID_SOCKET) {
		printf("Could not connect to socket!\n");
	}

	return hostSock;
}

//===========================================================================================================

void ServerSocket::Listen() {
	int result = listen(m_socketID, m_maxConnections);
	if (result == SOCKET_ERROR) {
		printf("Failed to listen\n");
	}
}

//-----------------------------------------------------------------------------------------------------------

SOCKET ServerSocket::Accept() {
	//the last server thread has garbage data for these
	sockaddr_storage otherAddr; //ext of sockaddr
	int otherAddrLen = sizeof(otherAddr);

	return AcceptOtherAddress((sockaddr*)&otherAddr, otherAddrLen);
}

//-----------------------------------------------------------------------------------------------------------

void ServerSocket::ServerLoop() {
	
	Listen();

	//loop this part
	while (true) {

		SOCKET otherSocket = Accept();
		
		char* bufferData = RecieveBufferDataFromOtherSocket(otherSocket);

		SendBufferDataToOtherSocket(otherSocket, bufferData);

	}
}

//===========================================================================================================

void ClientSocket::ClientLoop(const std::string& message) {

	const char* messageCstr = message.c_str();
	SendBufferDataToOtherSocket(m_socketID, messageCstr);

	RecieveBufferDataFromOtherSocket(m_socketID);

}


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///static methods

//-----------------------------------------------------------------------------------------------------------

std::string WindowsErrorAsString(DWORD error_id){
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


//-------------------------------------------------------------------------------------------------------
// get sockaddr, IPv4 or IPv6:
void* GetInAddress(sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((sockaddr_in*)sa)->sin_addr);
	}
	else {
		return &(((sockaddr_in6*)sa)->sin6_addr);
	}
}

//===========================================================================================================

