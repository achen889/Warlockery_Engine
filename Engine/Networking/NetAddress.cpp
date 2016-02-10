//==============================================================================================================
//NetAddress.cpp
//by Albert Chen Jan-27-2016.
//==============================================================================================================

#include "NetAddress.hpp"

#include "NetworkSystem.hpp"

#include <ws2tcpip.h>

//===========================================================================================================

//-----------------------------------------------------------------------------------------------------------

const std::string NetAddress::ToString() const {
	sockaddr* addrToString = (sockaddr*)&addr;

	char addr_name[INET6_ADDRSTRLEN];

	PCSTR addressPCStr = inet_ntop(addrToString->sa_family, GetInAddress(addrToString), addr_name, INET6_ADDRSTRLEN);
	char* addressCStr = (char*)addressPCStr;

	return std::string(addressCStr);
}


//===========================================================================================================
///----------------------------------------------------------------------------------------------------------
///global helpers

void AllocAddressesForHost(NetAddresses& addressesToAllocTo, std::string& host_name, const std::string& service){
	
	addrinfo hints;
	addrinfo* addr;

	if (host_name == ""){
		host_name = "local_host";
	}

	//which network layer it's using
	memset(&hints, 0, sizeof(hints)); //sets all hints to 0

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; // stream based, determines transport layer TCP
	hints.ai_flags = AI_PASSIVE; // used for binding/listening

	int status = getaddrinfo(host_name.c_str(), service.c_str(), &hints, &addr);
	if (status != 0) {
		//printf("Failed to create socket address! %s\n", gai_strerror(status));
	}
	
	addrinfo *iter;
	for (iter = addr; iter != nullptr; iter = iter->ai_next) {

		GetInAddress(iter->ai_addr);

		//instantiate address
		NetAddress newOutAddress;
		newOutAddress.Init(iter->ai_addr, iter->ai_addrlen);

		addressesToAllocTo.push_back(newOutAddress);

	}

	freeaddrinfo(addr);
}

//===========================================================================================================
