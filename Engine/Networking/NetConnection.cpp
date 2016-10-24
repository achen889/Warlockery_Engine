//==============================================================================================================
//NetConnection.cpp
//by Albert Chen Feb-1-2016.
//==============================================================================================================

#include "NetConnection.hpp"

#include "NetworkSession.hpp"
#include "Engine\Renderer\Text\TextSystem.hpp"
#include "ReliableTracker.hpp"
#include <xutility>
#include "..\Core\Clock.hpp"
#include "NetworkSystem.hpp"

//===========================================================================================================

static uint16_t s_current_ack_id = 0;

void ConnectionHeartbeatCallback(const char* event_name, void* args) {
	UNUSED(event_name);
	NetConnection* conn = (NetConnection*)args;

	static NetMessage heartbeatMsg(EN_MSG_ID_HEARTBEAT, "", 0);

	if (conn && conn->IsConnIndexValid() && !(conn->GetOwningSession()->IsConnSelf(conn)) ) {
		conn->SendNetMessage(heartbeatMsg);
	}
	
}

///----------------------------------------------------------------------------------------------------------
///constructors

NetConnection::NetConnection(const NetAddress& conn_addr):
	m_address(conn_addr),
	m_nextAckID(s_current_ack_id),
	m_state(CONN_STATE_CONNECTED)
{

	s_current_ack_id++;

	//get the current time
	m_timeRecievedPacket = GetCurrentSeconds();

	m_lastTimeRecievedPacket = GetCurrentSeconds();

	m_heartbeatTimer = Timer((float)CONN_HEARTBEAT_TICK, "Net_Heartbeat",
		ConnectionHeartbeatCallback, this, true);
	
}

//-----------------------------------------------------------------------------------------------------------

NetConnection::~NetConnection(){
	//clean up all the queues
// 	NetMessages m_unsentUnreliables;
	m_unsentUnreliables.clear();

	CleanupTrackers();

	while (m_unsentReliables.size() > 0) {
		NetMessage* msg = m_unsentReliables.front();
		if (msg) {
			delete msg;
			msg = NULL;
		}
		m_unsentReliables.pop();
	}

	while (m_sentReliables.size() > 0) {
		NetMessage* msg = m_sentReliables.front();
		if (msg) {
			delete msg;
			msg = NULL;
		}
		m_sentReliables.pop();
	}
	

// 	std::queue<ReliableTracker*> m_trackers;
// 	std::queue<NetMessage*> m_unsentReliables;
// 	std::queue<NetMessage*> m_sentReliables;

}

//-----------------------------------------------------------------------------------------------------------

void NetConnection::Update(){

	CheckForDisconnect();
	if (!IsConnected()) {
		return;
	}

	double deltaSeconds = GetSystemClock().GetDeltaSeconds();

	//do heartbeat on timer //sending something
	m_heartbeatTimer.Update(deltaSeconds);

	//UpdateSendPackets();

	CleanupTrackers();
	//sends only one packet full of the unreliable ones and the reliable ones
	UpdateSendPacket();

}

//-----------------------------------------------------------------------------------------------------------

void NetConnection::CleanupTrackers() {
	while (m_trackers.size() > 0) {
		ReliableTracker* tracker = m_trackers.front();
		m_trackers.pop_back();

		if (tracker != NULL && m_trackers.empty()) {
			float age = float( GetCurrentSeconds() - tracker->timeCreated);
			if (age >= CONN_TRACKER_MAX_AGE) {
				m_trackers.pop_back();
				if(tracker != NULL)
				   delete tracker;
			}
			else {
				return;
			}//end of inner if/else
		}

	}
}

//-----------------------------------------------------------------------------------------------------------

bool NetConnection::CanUseReliableID(uint16_t reliableID) {
	//m_oldestRecvReliableID;

	if (reliableID < m_oldestRecvReliableID) {
		reliableID += CONN_MAX_RELIABLE_ID;
	}

	uint16_t dist = m_oldestRecvReliableID;

	return (dist < CONN_RELIABLE_WINDOW_SIZE);

}

ReliableTracker* NetConnection::FindAndRemoveTracker(uint16_t ack){

	for (size_t i = 0; i < m_trackers.size(); i++){
		ReliableTracker* tracker =  m_trackers[i];
		if (tracker->packetAckID == ack) {
			return tracker;
		}
	}
	return NULL;

}

//-----------------------------------------------------------------------------------------------------------

void NetConnection::UpdateSendPacket() {

	NetPacket* packetToSend = new NetPacket(m_nextAckID, NetworkSystem::GetMyConnIndex(this)); //give nextAckID
	packetToSend->SetAddress(m_address);

	std::queue<NetMessage*> reliablesSentThisFrame;

	//send reliables

	//resend old reliables //void UpdateSendOldReliables()
	while (m_sentReliables.size() > 0 && !m_sentReliables.empty()) {
		NetMessage* msg = m_sentReliables.front();
		//if it got back to me, it will mark as free
		if (m_sentReliableIDs.Get(msg->reliableID) == false) {
			m_sentReliables.pop();
			//delete msg;
		}
		else {
			float age = float( GetCurrentSeconds() - msg->lastSentTime);
			if (age >= CONN_NET_MESSAGE_RESEND_TIME) {
				if (packetToSend->AddMessage(*msg)) {
					m_sentReliables.pop();

					reliablesSentThisFrame.push(msg);
				}
				else {
					break;
				}//end of inner if/else
			}
			else {
				break;
			}//end of if/else
		}//end of outer if/else
	}//end of while

	//push all reliable mesages to packet //void UpdateSendNewReliables()
	while (m_unsentReliables.size() > 0) {
		NetMessage* msg = m_unsentReliables.front();
		uint16_t reliable_id = m_nextReliableID;

		if (!CanUseReliableID(m_nextReliableID)) {
			break;
		}

		msg->reliableID = reliable_id;
		if (packetToSend->AddMessage(*msg)) {
			IncrementReliableID();
			m_unsentReliables.pop();
			//delete m_unsentReliables.back();

			reliablesSentThisFrame.push(msg);

		}
		else {
			break;
		}

	}

	//track reliables //void UpdateTrackReliables()
	if (reliablesSentThisFrame.size() > 0) {
		ReliableTracker* tracker = new ReliableTracker(m_nextAckID, reliablesSentThisFrame.size());

		//add to front of vector
		std::vector<ReliableTracker*>::iterator it = m_trackers.begin();
		m_trackers.insert(it, tracker);

		while (reliablesSentThisFrame.size()) {
			NetMessage* msg = reliablesSentThisFrame.front();
			reliablesSentThisFrame.pop();

			tracker->reliableIDs.push_back(msg->reliableID);
			msg->lastSentTime = GetCurrentSeconds();
			m_sentReliables.push(msg);
		}
	}

	// Tell the person about all acks we've received from them. //void UpdateTrackedAcks()
	if (m_trackedAcks.size() > 0) {
		NetMessage msg(EN_MSG_ID_ACK);
		uint8_t ackCount = (uint8_t)m_trackedAcks.size();
		//msg->WRITE_BYTES(ackCount);

		msg.WriteMessageData(&ackCount, SIZE_OF(ackCount));

		for (size_t i = 0; i < m_trackedAcks.size(); ++i) {
			msg.WriteMessageData(&m_trackedAcks[i], SIZE_OF(m_trackedAcks[i]));
			
			//msg->WRITE_BYTES(m_trackedAcks[i]);
		}

		packetToSend->AddMessage(msg);

		m_trackedAcks.clear();
	}

	//add unreliables //void UpdateSendUnreliables()
	if (m_unsentUnreliables.size() > 0) {
		for (NetMessagesIterator it = m_unsentUnreliables.begin(); it != m_unsentUnreliables.end(); ) {
			 NetMessage& msg = (*it);
			 packetToSend->AddMessage(msg);

			 it = m_unsentUnreliables.erase(it);
		}
	}
	
	//only send if packet has messages
	if (packetToSend && packetToSend->GetMessageCount() > 0) {
		IncrementAckID();
		SendPacket(packetToSend);
	}
	else {
		delete packetToSend;
		packetToSend = NULL;
	}
		
	

}

//-----------------------------------------------------------------------------------------------------------

void NetConnection::UpdateSendPackets() {

	NetPackets netPacketsToSend;

	UpdatePacketsToSend(netPacketsToSend);

	//send all the packets that were created
	for (NetPacketsIterator it = netPacketsToSend.begin(); it != netPacketsToSend.end(); ++it) {
		NetPacket* packet = (*it);
		if (packet) {


			SendPacket(packet);

			//it = netPacketsToSend.erase(it);
		}
	}// end of for

}

//-----------------------------------------------------------------------------------------------------------

void NetConnection::SendPacket(NetPacket* packet) {
	//send it off using NetSession::SendPacket.
	if (m_owningSession) {

		std::string debugPacketString = "\nconn: index:"+ IntToString(m_connIndex) +"name ["+ GetName() +"] to " + m_address.ToString() + " => sent packet[";
		debugPacketString += packet->ToStringAsBase(16) + "]";
		ConsolePrintString(debugPacketString);
		//OUTPUT_STRING_TO_CONSOLE(debugPacketString, 1000);
		
		m_owningSession->SendPacket(packet);
	}
}


//-----------------------------------------------------------------------------------------------------------

//packs as many messages as it can on each packet
void NetConnection::UpdatePacketsToSend(NetPackets& netPacketsToSend) {

	NetMessagesIterator it = m_unsentUnreliables.begin();
	//send all my outgoing messages out
	while (!m_unsentUnreliables.empty()) {
		NetMessage& outMsg = (*it);

		//fit as many messages as you can into the outPacket in the vector
		netPacketsToSend.push_back(new NetPacket());
		NetPacket*& outPacket = netPacketsToSend.back();
		outPacket->SetAddress(m_address);//manually set addr
		outPacket->WriteInitHeaderBytes();
		
// 		std::string debugPacketString = "buildingPacket(before)["+ outPacket->ToStringAsBase(16) + "]";
// 		ConsolePrintString(debugPacketString);

		while(outPacket->ValidatePacketSize() && it != m_unsentUnreliables.end()) {
			outMsg = (*it);
			//append messages if there is room
			outPacket->AddMessage(outMsg);
			
			it = m_unsentUnreliables.erase(it);
			//++it;
		}

// 		std::string debugPacketString = "buildingPacket(after)[" + outPacket->ToStringAsBase(16) + "]";
// 		ConsolePrintString(debugPacketString);
		
	}//end of while

}



//-----------------------------------------------------------------------------------------------------------

void NetConnection::SendNetMessage(NetMessage& msg) {

	if (msg.IsReliable()) {
		if (msg.IsInOrder()) {
			msg.orderID = m_nextOutgoingOrderID;

			m_nextOutgoingOrderID++;
			if (m_nextOutgoingOrderID > BIT(16)) {
				m_nextOutgoingOrderID = 0;
			}

		}
		m_unsentReliables.push(&msg);
	}
	else {
		m_unsentUnreliables.push_back(msg);
	}

}

//-----------------------------------------------------------------------------------------------------------

void NetConnection::CheckForDisconnect(){
	
	//do not check if it is me
	if (!NetworkSystem::s_gameSession->IsConnSelf(this)) {
		double curTime = GetCurrentSeconds();

		double age = curTime - m_timeRecievedPacket;

		//ConsolePrintString("\n Conn: "+GetName()+" => Age: " + DoubleToString(age));
		if (age > CONN_TIME_FOR_DISCONNECT) {
			SetState(CONN_STATE_DISCONNECTED);
			ConsolePrintString(" => " + GetName() + " is Disconnected!");
		}
	}


}

//===========================================================================================================

