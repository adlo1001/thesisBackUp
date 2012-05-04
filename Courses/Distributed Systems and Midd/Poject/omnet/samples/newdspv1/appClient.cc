//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 


//There is no publis timeout, so if publish lost, then it's lost
#include "appClient.h"
#include <algorithm>
#include "dsp_type_def.h"

Define_Module(AppClient);

void AppClient::initialize()
{
	//initialize the address
		cModule::GateIterator i(this->getParentModule());
		cGate *gate = i();
		cGate *otherGate = gate->getType()==cGate::OUTPUT ? gate->getNextGate() :
			                                                gate->getPreviousGate();
		addr = otherGate->getIndex();
		WATCH(addr);

		subSeqNum = 0;

		//initialize the topics
		topics = new std::string[4];
		topics[0] = "Mary";
		topics[1] = "John";
		topics[2] = "Bill";
		topics[3] = "Smith";

		currStat = DSP_CLIENT_IDLE;
		WATCH(currStat);

		nextActionScheduler = new InternetMsg();
		nextActionScheduler->setMsgType(DSP_SELF_CLIENT_ACTION_SCHEDULER);
		scheduleAt(simTime()+par("powerOnDelay"),nextActionScheduler);

		currBroker = DSP_MASTER_BROKER;
		WATCH(currBroker);

	    agreedlocaltime = 0;
        clockdriftrate = 1;
        globaltime = 0;
}

void AppClient::handleMessage(cMessage *msg)
{
	InternetMsg *tempmsg = check_and_cast<InternetMsg *>(msg);
	if (tempmsg==NULL) { error("Unknown message type"); return; }

	switch (tempmsg->getMsgType()){
	case DSP_SELF_CLIENT_ACTION_SCHEDULER:
			{
				randomActionGenerator();
				nextActionScheduler = new InternetMsg();
				nextActionScheduler->setMsgType(DSP_SELF_CLIENT_ACTION_SCHEDULER);
				scheduleAt(simTime()+par("actionDelay"),nextActionScheduler);
				break;
			}
	case DSP_SELF_CLIENT_JOIN_TIMEOUT:
			{
				if(currStat==DSP_CLIENT_TRANSIENT){
					EV << "Did not receive join replay before timeout, retry." << endl;
					InternetMsg *retry_msg = new InternetMsg();
					retry_msg->setSource(addr);
					retry_msg->setDestination(currBroker);
					retry_msg->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
					retry_msg->setMsgType(DSP_CLIENT_JOIN_MESSAGE);
					send(retry_msg,"toconsis");
					joinTimeout = new InternetMsg();
					joinTimeout ->setMsgType(DSP_SELF_CLIENT_JOIN_TIMEOUT);
					scheduleAt(simTime()+par("joinTimeout"), joinTimeout);
				}
				break;
			}
	case DSP_CLIENT_JOIN_REPLY_MESSAGE:
			{
				if(currStat==DSP_CLIENT_TRANSIENT){
					EV << "join replay received, join succeeded." << endl;
					cancelAndDelete(joinTimeout);
					currStat = DSP_CLIENT_JOINED;
					currBroker = tempmsg->getSource();
					EV << "Now my broker is " << currBroker<<endl;
				}
				break;
			}
	case DSP_CLIENT_LEAVE_REPLY_MESSAGE:{
				if(currStat==DSP_CLIENT_TRANSIENT){
				EV << "leave replay received, leave succeeded." << endl;
				cancelAndDelete(leaveTimeout);
				currStat = DSP_CLIENT_IDLE;
				currBroker = DSP_MASTER_BROKER;
				EV<<"Now My Broker is "<<currBroker<<endl;
				}
				break;
			}
	case DSP_SELF_CLIENT_LEAVE_TIMEOUT:{
				if(currStat==DSP_CLIENT_TRANSIENT){
					EV << "Did not receive leave replay before timeout, retry." << endl;
					InternetMsg *retry_msg = new InternetMsg();
					retry_msg->setSource(addr);
					retry_msg->setDestination(currBroker);
					retry_msg->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
					retry_msg->setMsgType(DSP_CLIENT_LEAVE_MESSAGE);
					send(retry_msg,"toconsis");
					leaveTimeout = new InternetMsg();
					leaveTimeout->setMsgType(DSP_SELF_CLIENT_LEAVE_TIMEOUT);
					scheduleAt(simTime()+par("leaveTimeout"), leaveTimeout);
				}
				break;
			}
	case DSP_SELF_CLIENT_SUBSCRIBE_TIMEOUT:{
				EV << "Subscription for topic \"" << notConfirmedSub.front().getTopic() << "\" sequence number: " << notConfirmedSub.front().getSeqNum() << " timed out, resent. " << endl;
				InternetMsg *msg = new InternetMsg();
				msg->setSource(addr);
				msg->setDestination(currBroker);
				msg->setMsgType(DSP_CLIENT_SUBSCRIBE_MESSAGE);
				msg->setSeqNumber(subSeqNum);
				msg->setTopic(notConfirmedSub.front().getTopic().c_str());
				msg->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
				send(msg,"toconsis");
				notConfirmedSub.push_back(SubscriptionSeq(simTime().dbl()+par("subscribeTimeout").doubleValue(),subSeqNum,notConfirmedSub.front().getTopic()));
				notConfirmedSub.pop_front();
				subscribeTimeout = new InternetMsg();
				subscribeTimeout ->setMsgType(DSP_SELF_CLIENT_SUBSCRIBE_TIMEOUT);
				scheduleAt(notConfirmedSub.front().getTimestamp(), subscribeTimeout);
				subSeqNum++;
				break;
			}
			case DSP_CLIENT_SUBSCRIBE_REPLY_MESSAGE:{
				EV << "Subscription for sequence number: " << tempmsg->getSeqNumber() << " succeeded. "<<endl ;
				it_notConfirmedSub = find(notConfirmedSub.begin(),notConfirmedSub.end(),SubscriptionSeq(tempmsg->getSeqNumber()));
				if(it_notConfirmedSub!=notConfirmedSub.end()){
					bool firstModified = false;
					if (it_notConfirmedSub==notConfirmedSub.begin()) firstModified = true;
					notConfirmedSub.erase(it_notConfirmedSub);
					if(firstModified && !notConfirmedSub.empty()){
						cancelAndDelete(subscribeTimeout);
						subscribeTimeout = new InternetMsg();
						subscribeTimeout ->setMsgType(DSP_SELF_CLIENT_SUBSCRIBE_TIMEOUT);
						scheduleAt(notConfirmedSub.front().getTimestamp(), subscribeTimeout);
					} else if (notConfirmedSub.empty()){
						cancelAndDelete(subscribeTimeout);
					}
				}
				break;
			}
			case DSP_BROKER_TOPIC_MESSAGE:{
			if(currStat==DSP_CLIENT_JOINED){
			EV << "topic messaged received, published by client with address " << tempmsg->getSource() << ", related topic is: " << tempmsg->getTopic();
			}
			break;
			}
			case CONSIS_APP_SYN:{
			 agreedlocaltime = tempmsg->getAgreedlocaltime();
			 clockdriftrate = tempmsg->getClockdriftrate();
			 globaltime = tempmsg->getGlobaltime();
			 EV<<"Since Consistency Module Synchronized, Now App knows:"<<endl;
			 EV<<"agreedlocaltime = "<<agreedlocaltime<<endl;
			 EV<<"globaltime = "<<globaltime<<endl;
			 EV<<"clockdriftrate = "<<clockdriftrate<<endl;
			 break;
			}
    default: break;
    }
cancelAndDelete(msg);
return;
}

void AppClient::randomActionGenerator(){
	switch(currStat){
		case DSP_CLIENT_IDLE:
		{
			InternetMsg *msg = new InternetMsg();
			msg->setSource(addr);
			msg->setDestination(currBroker);
			msg->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
			msg->setMsgType(DSP_CLIENT_JOIN_MESSAGE);
			send(msg,"toconsis");
			EV << "sending join request to broker." << endl;
			joinTimeout = new InternetMsg();
			joinTimeout->setMsgType(DSP_SELF_CLIENT_JOIN_TIMEOUT);
			scheduleAt(simTime()+par("joinTimeout"), joinTimeout);
			currStat = DSP_CLIENT_TRANSIENT;
			break;
		}
		case DSP_CLIENT_TRANSIENT:{
			break;
		}
		case DSP_CLIENT_JOINED:
		{
			int dice = intrand(2000);
			int choice;
			if (dice>=0 && dice<=199){
				choice = DSP_CLIENT_SUBSCRIBE_OPERATION;
			} else if (dice>=200 && dice<=1899){
				choice = DSP_CLIENT_PUBLISH_OPERATION;
			} else {
				choice = DSP_CLIENT_LEAVE_OPERATION;
			}
			switch (choice){
				case DSP_CLIENT_SUBSCRIBE_OPERATION:{
					//subscribe to a topic
					InternetMsg *msg = new InternetMsg();
					msg->setSource(addr);
					msg->setDestination(currBroker);
					msg->setMsgType(DSP_CLIENT_SUBSCRIBE_MESSAGE);
					msg->setSeqNumber(subSeqNum);
					msg->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
					int randomInteger = intrand(topics->size());
					msg->setTopic(topics[randomInteger].c_str());
					send(msg,"toconsis");
					if(notConfirmedSub.empty()){
						subscribeTimeout = new InternetMsg();
						subscribeTimeout ->setMsgType(DSP_SELF_CLIENT_SUBSCRIBE_TIMEOUT);
						scheduleAt(simTime()+par("subscribeTimeout"), subscribeTimeout);
					}
					notConfirmedSub.push_back(SubscriptionSeq(simTime().dbl()+par("subscribeTimeout").doubleValue(),subSeqNum, topics[randomInteger]));
					EV << "subscription request for topic \"" << topics[randomInteger] << "\" sequence number: " << subSeqNum<<" sent to broker with address " << currBroker << endl;
					subSeqNum++;
					break;
				}
				case DSP_CLIENT_PUBLISH_OPERATION:{
					//publish a thing
					InternetMsg *msg = new InternetMsg();
					int randomInteger = intrand(topics->size());
					msg->setSource(addr);
					msg->setDestination(currBroker);
					msg->setMsgType(DSP_CLIENT_PUBLISH_MESSAGE);
					msg->setTopic(topics[randomInteger].c_str());
					msg->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
					send(msg,"toconsis");
					break;
					EV<<"Node "<<addr<<" now publish topic: "<<topics[randomInteger]<<endl;
				}
				case DSP_CLIENT_LEAVE_OPERATION:{
					InternetMsg *msg = new InternetMsg();
					msg->setSource(addr);
					msg->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
					msg->setDestination(currBroker);
					msg->setMsgType(DSP_CLIENT_LEAVE_MESSAGE);
					send(msg,"toconsis");
					EV << "sending leave request to broker." << endl;
					leaveTimeout = new InternetMsg();
					leaveTimeout->setMsgType(DSP_SELF_CLIENT_LEAVE_TIMEOUT);
					scheduleAt(simTime()+par("leaveTimeout"), leaveTimeout);
					currStat = DSP_CLIENT_TRANSIENT;
					break;
				}
				default:
					break;
			}
			break;
		}
		default:error("Client State Error"); break;
	}
	return;
}
