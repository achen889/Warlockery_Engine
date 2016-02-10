//==============================================================================================================
//NetAddress.hpp
//by Albert Chen Jan-25-2016.
//==============================================================================================================

#pragma once

#ifndef _included_NetAddress__
#define _included_NetAddress__

#include <string>
#include <vector>

#ifdef _WINSOCKAPI_
#undef _WINSOCKAPI_
#endif
#include <Winsock2.h>

//===========================================================================================================

struct sockaddr_storage;
struct sockaddr;

struct NetAddress {
public:
	sockaddr_storage addr;
	size_t addrLen;

	NetAddress() {
		//do nothing
	}
	~NetAddress() {
		//do nothing
	}

	//methods
	void Init(sockaddr* addrToCopy, size_t addrLength);
	
	const std::string ToString() const;

};

typedef std::vector<NetAddress> NetAddresses;
typedef std::vector<NetAddress>::iterator NetAddressesIterator;

//-----------------------------------------------------------------------------------------------------------

inline void NetAddress::Init(sockaddr* addrToCopy, size_t addrLength) {
	addrLen = addrLength;
	memcpy(&addr, addrToCopy, addrLen); //causes some crash in the assembly for memcpy probably
}

//===========================================================================================================
//global helper

void AllocAddressesForHost(NetAddresses& addressesToAllocTo, std::string& host_name, const std::string& service);

//===========================================================================================================

#endif //__includedNetAddress__

