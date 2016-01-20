//==============================================================================================================
//NetworkSystem.hpp
//by Albert Chen Jan-14-2016.
//==============================================================================================================

#pragma once

#ifndef _included_NetworkSystem__
#define _included_NetworkSystem__

//===========================================================================================================

#include "ConnectionSocket.hpp"

//===========================================================================================================

class NetworkSystem {
public:

	bool StartUp();
	void ShutDown();

	void PrintAddressesForHost(std::string& host_name, const std::string& service);

};



//===========================================================================================================

#endif //__includedNetworkSystem__

