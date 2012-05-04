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

#include "appBroker.h"


Define_Module(AppBroker);


void AppBroker::initialize()
{
	//initialize the address
		cModule::GateIterator i(this->getParentModule());
		cGate *gate = i();
		cGate *otherGate = gate->getType()==cGate::OUTPUT ? gate->getNextGate() :
			                                                gate->getPreviousGate();
		addr = otherGate->getIndex();
		WATCH(addr);

		if(addr==DSP_MASTER_BROKER){
			currStat = DSP_BROKER_JOINED;
			connectedBrokers.push_front(TimestampedId(DSP_MASTER_BROKER,0));
		} else {
			currStat = DSP_BROKER_IDLE;
			nextActionScheduler = new InternetMsg();
			nextActionScheduler->setMsgType(DSP_SELF_BROKER_ACTION_SCHEDULER);
			scheduleAt(simTime()+par("actionDelay"),nextActionScheduler);
		}
		WATCH(currStat);
		netVersion = 0;

	    agreedlocaltime = 0;
        clockdriftrate = 1;
        globaltime = 0;
}

void AppBroker::handleMessage(cMessage *msg)
{
	InternetMsg *tempmsg = check_and_cast<InternetMsg *>(msg);
		if (tempmsg==NULL) { error("Unknown message type"); return; }
		switch (tempmsg->getMsgType()){
		case DSP_SELF_BROKER_ACTION_SCHEDULER:
				{
					joinActionGenerator();
					cancelAndDelete(msg);
					nextActionScheduler = new InternetMsg();
					nextActionScheduler->setMsgType(DSP_SELF_BROKER_ACTION_SCHEDULER);
					scheduleAt(simTime()+par("actionDelay"),nextActionScheduler);
					break;
				}
		case DSP_SELF_BROKER_JOIN_TIMEOUT:{
			if(currStat==DSP_BROKER_TRANSIENT){
				EV << "Did not receive join replay before timeout, retry." << endl;
				InternetMsg *retry_msg = new InternetMsg();
				retry_msg->setSource(addr);
				retry_msg->setDestination(DSP_MASTER_BROKER);
				retry_msg->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
				retry_msg->setMsgType(DSP_BROKER_JOIN_MESSAGE);
				send(retry_msg,"toconsis");
				cancelAndDelete(msg);
				joinTimeout = new InternetMsg();
				joinTimeout ->setMsgType(DSP_SELF_BROKER_JOIN_TIMEOUT);
				scheduleAt(simTime()+par("joinTimeout"), joinTimeout);
			}
			break;
		}
		case DSP_BROKER_JOIN_MESSAGE:{
			if(currStat==DSP_BROKER_JOINED){
				netVersion++;
				//insert in a sorted way
				it_brokers = lower_bound (connectedBrokers.begin(), connectedBrokers.end(), TimestampedId(tempmsg->getSource(),0));
				if (it_brokers==connectedBrokers.end()||(*it_brokers).getClient()!=tempmsg->getSource()){
					EV << "join request from broker with address " << tempmsg->getSource() << " received, "
					   << "broker added to the list and net update message sent to all brokers." << endl;
					connectedBrokers.insert(it_brokers,TimestampedId(tempmsg->getSource(),tempmsg->getTimestamp()));
				} else {
					EV << "duplicated join request from broker with address "
					   << tempmsg->getSource() << ", ignore but send reply message." << endl;
				}
				InternetMsg *join_reply = new InternetMsg();
				join_reply->setSource(addr);
				join_reply->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
				join_reply->setMsgType(DSP_BROKER_NET_UPDATE_MESSAGE);
				join_reply->setConnectedBrokersArraySize(connectedBrokers.size());
				join_reply->setConnectedBrokersTimeStampArraySize(connectedBrokers.size());
				join_reply->setSeqNumber(netVersion);
				unsigned int i=0;
				for(it_brokers=connectedBrokers.begin();it_brokers!=connectedBrokers.end();it_brokers++){
					join_reply->setConnectedBrokers(i,(*it_brokers).getClient());
					join_reply->setConnectedBrokersTimeStamp(i,(*it_brokers).getTimestamp());
					i++;
				}
				for(it_brokers=connectedBrokers.begin();it_brokers!=connectedBrokers.end();it_brokers++){
					if((*it_brokers).getClient()!=addr){
						InternetMsg *forward = join_reply->dup();
						forward->setDestination((*it_brokers).getClient());
						InternetMsg *forwardDup = forward->dup();
						unconfirmedNetUpdate.insert(std::pair<int,InternetMsg*>((*it_brokers).getClient(),forwardDup));
						send(forward,"toconsis");
					}
				}
				cancelAndDelete(join_reply);
				netUpdateTimeout = new InternetMsg();
				netUpdateTimeout ->setMsgType(DSP_SELF_BROKER_NET_UPDATE_TIMEOUT);
				scheduleAt(simTime()+par("netUpdateTimeout"), netUpdateTimeout);
			}
			break;
		}
		case DSP_BROKER_NET_UPDATE_MESSAGE:{
					if(tempmsg->getSeqNumber() <= netVersion) {
						InternetMsg *netUpdateReply = new InternetMsg();
						netUpdateReply->setMsgType(DSP_BROKER_NET_UPDATE_REPLY_MESSAGE);
						netUpdateReply->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
						netUpdateReply->setDestination(tempmsg->getSource());
						netUpdateReply->setSource(addr);
						netUpdateReply->setSeqNumber(netVersion);
						send(netUpdateReply,"toconsis");
						break;
					}
					if(currStat != DSP_BROKER_TRANSIENT) {
						currStat = DSP_BROKER_TRANSIENT;
					} else {
						cancelAndDelete(joinTimeout);
					}
					netVersion = tempmsg->getSeqNumber();
					while(!connectedBrokers.empty()){
						it_brokers = connectedBrokers.begin();
						connectedBrokers.erase(it_brokers);
					}
					for(unsigned int i=0;i<tempmsg->getConnectedBrokersArraySize();i++){
						it_brokers = connectedBrokers.end();
						connectedBrokers.insert(it_brokers,TimestampedId(tempmsg->getConnectedBrokers(i),tempmsg->getConnectedBrokersTimeStamp(i)));
					}
					InternetMsg *netUpdateReply = new InternetMsg();
					netUpdateReply->setMsgType(DSP_BROKER_NET_UPDATE_REPLY_MESSAGE);
					netUpdateReply->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
					netUpdateReply->setDestination(tempmsg->getSource());
					netUpdateReply->setSource(addr);
					netUpdateReply->setSeqNumber(netVersion);
					send(netUpdateReply,"toconsis");
					std::list<TimestampedId>::iterator temp_ite;
					for (temp_ite=connectedBrokers.begin();temp_ite!=connectedBrokers.end();temp_ite++) {
					EV<<"I Node "<<addr<<" now knows node "<<temp_ite->getClient()<<" is in the network,update msg transmitted sucessfully "<<endl;
					}
					currStat = DSP_BROKER_JOINED;
					break;
				}
				case DSP_SELF_BROKER_NET_UPDATE_TIMEOUT:{
					for(it_unconfirmedNetUpdate=unconfirmedNetUpdate.begin();it_unconfirmedNetUpdate!=unconfirmedNetUpdate.end();it_unconfirmedNetUpdate++){
						InternetMsg *resend = (*it_unconfirmedNetUpdate).second->dup();
						EV<<"Some Net Update Msg is missing , so now resending the missing parts"<<endl;
						EV<<"Now Broker 0 will resend net update msg to node "<<resend->getDestination()<<endl;
						send(resend,"toconsis");
					}
					netUpdateTimeout = new InternetMsg();
					netUpdateTimeout ->setMsgType(DSP_SELF_BROKER_NET_UPDATE_TIMEOUT);
					scheduleAt(simTime()+par("netUpdateTimeout"), netUpdateTimeout);
					break;
				}
				case DSP_BROKER_NET_UPDATE_REPLY_MESSAGE:{
					EV<<"Receving Net Update Reply"<<endl;
					it_unconfirmedNetUpdate = unconfirmedNetUpdate.find(tempmsg->getSource());
					delete((*it_unconfirmedNetUpdate).second);
					unconfirmedNetUpdate.erase(it_unconfirmedNetUpdate);
					std::map<int,InternetMsg*>::iterator temp_ite;
					for (temp_ite=unconfirmedNetUpdate.begin();temp_ite!=unconfirmedNetUpdate.end();temp_ite++) {
					EV<<"Remaining Unconfirmed Net Update are "<<temp_ite->first<<endl;}
					if(unconfirmedNetUpdate.empty()){
						cancelAndDelete(netUpdateTimeout);
					}
					break;
				}
				case DSP_CLIENT_JOIN_MESSAGE:{
					if(currStat==DSP_BROKER_JOINED){
					//insert in a sorted way
				    it_clients = lower_bound (clients.begin(), clients.end(), TimestampedId(tempmsg->getSource(),0));
					if (it_clients==clients.end()||(*it_clients).getClient()!=tempmsg->getSource()){
					EV << "join request from client with address " << tempmsg->getSource() << " received, "
					   << "client added to the list and join reply message sent." << endl;
					clients.insert(it_clients,TimestampedId(tempmsg->getSource(),tempmsg->getTimestamp()));
					} else {
					EV << "duplicated join request from client with address "
				    << tempmsg->getSource() << ", ignore but send reply message." << endl;
					}
					InternetMsg *join_reply = new InternetMsg();
					join_reply->setSource(addr);
					join_reply->setDestination(tempmsg->getSource());
					join_reply->setMsgType(DSP_CLIENT_JOIN_REPLY_MESSAGE);
					send(join_reply,"toconsis");
					}
							/* debug clients
							for(it_clients = clients.begin();it_clients!=clients.end();it_clients++){
								EV << "Client: " << (*it_clients).getClient() << " TimeStamp: " << (*it_clients).getTimestamp() << endl;
							}
							*/
							break;
			}
				case DSP_CLIENT_LEAVE_MESSAGE:{
				if(currStat==DSP_BROKER_JOINED){
				EV << "leave request from client with address " << tempmsg->getSource() << " received, ";
				it_clients = lower_bound(clients.begin(),clients.end(),TimestampedId(tempmsg->getSource(),0));
				if(it_clients!=clients.end() && !(tempmsg->getSource()<(*it_clients).getClient()) && (*it_clients).getTimestamp() < tempmsg->getTimestamp()){
				EV << "removing client from clients list and send replay message." << endl;
				clients.erase(it_clients);
				} else {
				EV << "client not in clients list, ignore but send reply message." << endl;
				}
				//clear the subscription list for the client
				for(it_subscriptions=subscriptions.begin();it_subscriptions!=subscriptions.end();it_subscriptions++){
				if((*it_subscriptions).second.getClient()==tempmsg->getSource()){
				subscriptions.erase(it_subscriptions);
				}
				}
		    	InternetMsg *leave_reply = new InternetMsg();
				leave_reply->setSource(addr);
				leave_reply->setDestination(tempmsg->getSource());
				leave_reply->setMsgType(DSP_CLIENT_LEAVE_REPLY_MESSAGE);
				send(leave_reply,"toconsis");
				}
				break;
				}
				case DSP_CLIENT_SUBSCRIBE_MESSAGE:{
				if(currStat==DSP_BROKER_JOINED){
				EV << "received subscription request from client with address " << tempmsg->getSource() << ", "<< "the topic is: " << tempmsg->getTopic() << endl;
				it_clients = lower_bound(clients.begin(),clients.end(),TimestampedId(tempmsg->getSource(),0));
				if(it_clients==clients.end()||tempmsg->getSource()!=(*it_clients).getClient()||(*it_clients).getTimestamp() > tempmsg->getTimestamp()){
				EV << "client not joined, ignore." << endl;
				InternetMsg *sub_reply = new InternetMsg();
				sub_reply->setMsgType(DSP_CLIENT_SUBSCRIBE_REPLY_MESSAGE);
				sub_reply->setSource(addr);
				sub_reply->setSeqNumber(tempmsg->getSeqNumber());
				sub_reply->setDestination(tempmsg->getSource());
				send(sub_reply,"toconsis");
				break;
				}
				bool subscribed = false;
				range_subscriptions = subscriptions.equal_range(std::string(tempmsg->getTopic()));
				for (it_subscriptions=range_subscriptions.first; it_subscriptions!=range_subscriptions.second; ++it_subscriptions)
				{
				if((*it_subscriptions).second.getClient()==tempmsg->getSource()){
				subscribed = true;
				EV << "same topic for same client already registered, ignore." << endl;
				break;
				}
				}
				if(!subscribed){
				subscriptions.insert(std::pair<std::string,TimestampedId>(std::string(tempmsg->getTopic()),TimestampedId(tempmsg->getSource(),tempmsg->getTimestamp())));
				EV << "subscription succeeded." << endl;
				}
				}
				InternetMsg *sub_reply = new InternetMsg();
				sub_reply->setMsgType(DSP_CLIENT_SUBSCRIBE_REPLY_MESSAGE);
				sub_reply->setSource(addr);
				sub_reply->setSeqNumber(tempmsg->getSeqNumber());
				sub_reply->setDestination(tempmsg->getSource());
				send(sub_reply,"toconsis");
				break;
			}
			case DSP_CLIENT_PUBLISH_MESSAGE:{
				if(currStat==DSP_BROKER_JOINED){
				EV << "received publish message from client with address " << tempmsg->getSource() << ", " << "the topic is: " << tempmsg->getTopic() << endl;
				it_clients = lower_bound(clients.begin(),clients.end(),TimestampedId(tempmsg->getSource(),0));
				if(it_clients==clients.end()||tempmsg->getSource()!=(*it_clients).getClient()||tempmsg->getTimestamp() < (*it_clients).getTimestamp()){
				EV << "client not joined, ignore." << endl;
				break;
			}
				//broadcast among joined brokers
				EV<<"Broadcasting to other brokers"<<endl;
				for(it_brokers=connectedBrokers.begin();it_brokers!=connectedBrokers.end();it_brokers++){
				if((*it_brokers).getClient()!=addr){
				InternetMsg *copy = tempmsg->dup();
				copy->setDestination((*it_brokers).getClient());
				copy->setMsgType(DSP_BROKER_FORWARD_PUBLISHED_MESSAGE);
				send(copy,"toconsis");
			}
			}
			//forwarding to clients subscribed to current broker
			range_subscriptions = subscriptions.equal_range(std::string(tempmsg->getTopic()));
			for (it_subscriptions=range_subscriptions.first; it_subscriptions!=range_subscriptions.second; ++it_subscriptions)
			{
			it_clients = lower_bound(clients.begin(),clients.end(),TimestampedId(tempmsg->getSource(),0));
			if((it_clients!=clients.end()&&(*it_clients).getClient()==tempmsg->getSource()) && //check if the client has joined, since order has been checked when subscribe, no need to check the join TimeStamp
			(*it_subscriptions).second.getTimestamp()<=tempmsg->getTimestamp()) //check casual order
			{
			EV << "client with address " << (*it_subscriptions).second.getClient() << " was registered for the topic, forward message." << endl;
		    InternetMsg *copy = tempmsg->dup();
			copy->setDestination((*it_subscriptions).second.getClient());
			copy->setMsgType(DSP_BROKER_TOPIC_MESSAGE);
			send(copy,"toconsis");
			}
			else {EV<<"No clients subscribed for this topic";}
			}
			}
			break;
			}
			case DSP_BROKER_FORWARD_PUBLISHED_MESSAGE: {
				range_subscriptions = subscriptions.equal_range(std::string(tempmsg->getTopic()));
				for (it_subscriptions=range_subscriptions.first; it_subscriptions!=range_subscriptions.second; ++it_subscriptions) {
				if( (*it_subscriptions).second.getTimestamp()<=tempmsg->getTimestamp()) //check casual order
				{
				EV << "client with address " << (*it_subscriptions).second.getClient() << " was registered for the topic, forward message." << endl;
				InternetMsg *copy = tempmsg->dup();
				copy->setDestination((*it_subscriptions).second.getClient());
				copy->setMsgType(DSP_BROKER_TOPIC_MESSAGE);
				send(copy,"toconsis");
				}
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
		default:{
		cancelAndDelete(msg);
		break;
		}
		}
}

void AppBroker::joinActionGenerator(){
	switch(currStat){
		case DSP_BROKER_IDLE:{
			InternetMsg *msg = new InternetMsg();
			msg->setSource(addr);
			msg->setDestination(DSP_MASTER_BROKER);
			msg->setTimestamp((simTime().dbl()-globaltime)*clockdriftrate+agreedlocaltime);
			msg->setMsgType(DSP_BROKER_JOIN_MESSAGE);
			send(msg,"toconsis");
			EV << "sending join request to broker 0." << endl;
			joinTimeout = new InternetMsg();
			joinTimeout->setMsgType(DSP_SELF_BROKER_JOIN_TIMEOUT);
			scheduleAt(simTime()+par("joinTimeout"), joinTimeout);
			currStat = DSP_BROKER_TRANSIENT;
			break;
		}
		default:
			break;
	}
}
