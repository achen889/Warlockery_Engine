//==============================================================================================================
//NetConnection.cpp
//by Albert Chen Feb-1-2016.
//==============================================================================================================

#include "NetConnection.hpp"

#include "NetworkSession.hpp"
#include "..\Renderer\Text\TextSystem.hpp"

//===========================================================================================================

static uint16_t s_current_ack_id = 0;

///----------------------------------------------------------------------------------------------------------
///constructors

NetConnection::NetConnection(const NetAddress& conn_addr):
	m_address(conn_addr),
	m_nextAckId(s_current_ack_id)
{

	s_current_ack_id++;
	
}

//-----------------------------------------------------------------------------------------------------------

void NetConnection::Update(){

	NetPackets netPacketsToSend;

	UpdatePacketsToSend(netPacketsToSend);
	
	//send all the packets that were created
	for (NetPacketsIterator it = netPacketsToSend.begin(); it != netPacketsToSend.end(); ++it){
		NetPacket* packet= (*it);
			if (packet) {

				std::string debugPacketString = "\nconn: " + m_address.ToString() + " => sent packet[";
				debugPacketString += packet->ToStringAsBase(16) + "]";
				ConsolePrintString(debugPacketString); //data here gets corrupted again somehow...
				OUTPUT_STRING_TO_CONSOLE(debugPacketString, 1000);

				//send it off using NetSession::SendPacket.
				if (m_owningSession) {
					m_owningSession->SendPacket(packet);
				}
				  

				//it = netPacketsToSend.erase(it);
			}
	}// end of for

}

//-----------------------------------------------------------------------------------------------------------

//packs as many messages as it can on each packet
void NetConnection::UpdatePacketsToSend(NetPackets& netPacketsToSend) {

	NetMessagesIterator it = m_outgoingMessages.begin();
	//send all my outgoing messages out
	while (!m_outgoingMessages.empty()) {
		NetMessage& outMsg = (*it);

		//fit as many messages as you can into the outPacket in the vector
		netPacketsToSend.push_back(new NetPacket());
		NetPacket*& outPacket = netPacketsToSend.back();
		outPacket->SetAddress(m_address);//manually set addr
		outPacket->WriteInitHeaderBytes();
		
// 		std::string debugPacketString = "buildingPacket(before)["+ outPacket->ToStringAsBase(16) + "]";
// 		ConsolePrintString(debugPacketString);

		while(outPacket->ValidatePacketSize() && it != m_outgoingMessages.end()) {
			outMsg = (*it);
			//append messages if there is room
			outPacket->AddMessage(outMsg);
			
			it = m_outgoingMessages.erase(it);
			//++it;
		}

// 		std::string debugPacketString = "buildingPacket(after)[" + outPacket->ToStringAsBase(16) + "]";
// 		ConsolePrintString(debugPacketString);
		
	}//end of while

}



//-----------------------------------------------------------------------------------------------------------

void NetConnection::SendNetMessage(NetMessage& msg) {

	m_outgoingMessages.push_back(msg);

}

//===========================================================================================================

