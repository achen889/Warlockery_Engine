//==============================================================================================================
//ReliableTracker.hpp
//by Albert Chen Feb-15-2016.
//==============================================================================================================

#pragma once
#include "Engine\Core\Time.hpp"

#ifndef _included_ReliableTracker__
#define _included_ReliableTracker__

//===========================================================================================================

typedef std::vector<uint16_t> ReliableIDs;

struct ReliableTracker {
public:
	ReliableTracker(const uint16_t& packetAck, size_t count) :
		timeCreated(GetCurrentSeconds()),
		packetAckID(packetAck)
	{
		reliableIDs.reserve(count);
	}

	double timeCreated;
	uint16_t packetAckID;
	ReliableIDs reliableIDs;

};


//===========================================================================================================

#endif //__includedReliableTracker__

