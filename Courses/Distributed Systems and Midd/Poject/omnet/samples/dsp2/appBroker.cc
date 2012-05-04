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
	EV << "Start Broker Application Module Initialization: " << endl;
	//initialize the address
	cModule::GateIterator i(this->getParentModule());
	cGate *gate = i();
	cGate *otherGate = gate->getType()==cGate::OUTPUT ? gate->getNextGate() :
		                                                gate->getPreviousGate();
	myAddr = otherGate->getIndex();
	WATCH(myAddr);
	myState = (myAddr==0) ? DSP2_BROKER_CONNECTED : DSP2_BROKER_IDLE;
	EV << "Current Node Address was set to " << myAddr << endl;
	EV << "State variable of current node was set to \"";
	switch(myState){
		case DSP2_BROKER_IDLE:
			EV <<"IDLE\"" << endl;
			break;
		case DSP2_BROKER_CONNECTED:
			EV <<"CONNECTED\"" << endl;
			connectedBrokers.push_back(myAddr);
			break;
		default:
			EV <<"UNKNOWN\"" << endl;
			error("BROKER Application Module: Unknown State");
			break;
	}
	powerOnTimer = new SelfMessage();
	powerOnTimer->setType(DSP2_BROKER_POWER_ON_TIMER);
	if(myState==DSP2_BROKER_IDLE){
		scheduleAt((simTime().dbl() + par("powerOnTime").doubleValue())/par("clockDrift").doubleValue(),powerOnTimer);
		EV << "power on timer set, this broker will connect to the network at local clock time "
		   << simTime().dbl() + par("powerOnTime").doubleValue() << "s, simulation time "
		   << (simTime().dbl() + par("powerOnTime").doubleValue())/par("clockDrift").doubleValue()
		   << "s"<< endl;
	}
	topologyIdx = 0;
	EV << "End Broker Application Module Initialization. " << endl << endl;

	WATCH_VECTOR(connectedBrokers);
	WATCH_MMAP(subscriptions);

}

void AppBroker::finish()
{
	std::map<int,cDoubleHistogram>::iterator subRecIt;
	for(subRecIt = subLatency.begin(); subRecIt!=subLatency.end();subRecIt++){
		std::stringstream recNameSt;
		recNameSt << "Subscribe Latency (Client address " << (*subRecIt).first << ")";
		(*subRecIt).second.recordAs(recNameSt.str().c_str());
	}
	std::map<int,cDoubleHistogram>::iterator unsubRecIt;
	for(unsubRecIt = unsubLatency.begin(); unsubRecIt!=unsubLatency.end();unsubRecIt++){
		std::stringstream recNameSt;
		recNameSt << "Un-Subscribe Latency (Client address " << (*unsubRecIt).first << ")";
		(*unsubRecIt).second.recordAs(recNameSt.str().c_str());
	}
}

void AppBroker::handleMessage(cMessage *msg)
{
	EV << "Start Broker Application Module Event. " << endl;
	if(msg->isSelfMessage()){
		SelfMessage *self = check_and_cast<SelfMessage *>(msg);
		switch(self->getType()){
			case DSP2_BROKER_POWER_ON_TIMER:{
				EV << "Power On Timer fired, sending connect message to broker address " << DSP2_MASTER_BROKER << endl;
				AppPacket *joinMsg = new AppPacket();
				joinMsg->setPktType(DSP2_BROKER_CONNECT_MESSAGE);
				joinMsg->setSrcAddress(myAddr);
				joinMsg->setDestAddress(DSP2_MASTER_BROKER);
				send(joinMsg,"middleware$o");
				break;
			}
			default:{
				error("Broker Application Module: Wrong self message type");
				break;
			}
		}
	} else {
		AppPacket *arrivedPkt = check_and_cast<AppPacket *>(msg);
		switch(arrivedPkt->getPktType()){
			case DSP2_BROKER_CONNECT_MESSAGE:{
				EV << "Received connection request from broker address " << arrivedPkt->getSrcAddress() << ", ";
				std::vector<int>::iterator it;
				it = lower_bound(connectedBrokers.begin(),connectedBrokers.end(),arrivedPkt->getSrcAddress());
				if(it==connectedBrokers.end()||(*it)!=arrivedPkt->getSrcAddress()){
					EV << "adding source broker to connected broker list, broadcast update to all brokers." << endl;
					connectedBrokers.insert(it,arrivedPkt->getSrcAddress());
					topologyIdx++;
					AppPacket *netUp = new AppPacket();
					netUp->setPktType(DSP2_BROKER_NET_UPDATE_MESSAGE);
					netUp->setSrcAddress(myAddr);
					netUp->setTopologyIdx(topologyIdx);
					netUp->setConnectedBrokersArraySize(connectedBrokers.size());
					for(unsigned i=0;i<connectedBrokers.size();i++){
						netUp->setConnectedBrokers(i,connectedBrokers[i]);
					}
					for(unsigned i=0;i<connectedBrokers.size();i++){
						if(myAddr!=connectedBrokers[i]){
							AppPacket *temp=netUp->dup();
							temp->setDestAddress(connectedBrokers[i]);
							send(temp,"middleware$o");
						}
					}
					cancelAndDelete(netUp);
					cancelAndDelete(msg);
				} else {
					EV << "broker already joined, ignore." << endl;
					cancelAndDelete(msg);
				}
				break;
			}
			case DSP2_BROKER_NET_UPDATE_MESSAGE:{
				EV << "Received net update message from broker address " << arrivedPkt->getSrcAddress() << ", ";
				if(arrivedPkt->getTopologyIdx()>topologyIdx){
					EV << "update the connect broker list." << endl;
					std::vector<int> temp;
					for(unsigned i=0;i<arrivedPkt->getConnectedBrokersArraySize();i++){
						temp.push_back(arrivedPkt->getConnectedBrokers(i));
					}
					connectedBrokers = temp;
					topologyIdx = arrivedPkt->getTopologyIdx();
					if(binary_search(connectedBrokers.begin(),connectedBrokers.end(),myAddr)){
						EV << "current node is in the connected list, set state to \"CONNECTED\"" << endl;
						myState =DSP2_BROKER_CONNECTED;
					}
					cancelAndDelete(msg);
				} else {
					EV << "old version of topology, ignore." << endl;
					cancelAndDelete(msg);
				}
				break;
			}
			case DSP2_CLIENT_SUBSCRIBE_MESSAGE:{
				EV << "received subscription message for topic \"" << arrivedPkt->getTopic() << "\" from client address " << arrivedPkt->getSrcAddress() << endl;
				if(myState!=DSP2_BROKER_CONNECTED){
					EV << "current broker is not connected, reply a reset message." << endl;
					AppPacket *reset = new AppPacket();
					reset->setDestAddress(arrivedPkt->getSrcAddress());
					reset->setSrcAddress(myAddr);
					reset->setPktType(DSP2_BROKER_NOT_READY_MESSAGE);
					send(reset,"middleware$o");
				} else {
					bool subscribed = false;
					std::pair<std::multimap<std::string,int>::iterator,std::multimap<std::string,int>::iterator> range_subscriptions;
					std::multimap<std::string,int>::iterator it_subscriptions;
					range_subscriptions = subscriptions.equal_range(std::string(arrivedPkt->getTopic()));
					for (it_subscriptions=range_subscriptions.first; it_subscriptions!=range_subscriptions.second; ++it_subscriptions)
					{
						if((*it_subscriptions).second==arrivedPkt->getSrcAddress()){
							subscribed = true;
							EV << "Same topic for same client already registered, ignore." << endl;
							break;
						}
					}
					if(!subscribed){
						subscriptions.insert(std::pair<std::string,int>(std::string(arrivedPkt->getTopic()),arrivedPkt->getSrcAddress()));
						EV << "Subscription succeeded." << endl;
						EV << "Recorded Latency: " << simTime().dbl()-arrivedPkt->getTimestamp().dbl() << "s" << endl;
						if(simTime().dbl()-arrivedPkt->getTimestamp().dbl() < 0) error("Wrong Latency");
						subLatency[arrivedPkt->getSrcAddress()].collect(simTime()-arrivedPkt->getTimestamp());
					}
				}
				cancelAndDelete(msg);
				break;
			}
			case DSP2_CLIENT_PUBLISH_MESSAGE:{
				if(myState!=DSP2_BROKER_CONNECTED){
					EV << "current broker is not connected, reply a reset message." << endl;
					AppPacket *reset = new AppPacket();
					reset->setDestAddress(arrivedPkt->getSrcAddress());
					reset->setSrcAddress(myAddr);
					reset->setPktType(DSP2_BROKER_NOT_READY_MESSAGE);
					send(reset,"middleware$o");
				} else {
					EV << "Received publish message for topic \"" << arrivedPkt->getTopic() << "\" originated by client address " << arrivedPkt->getOriginator() << endl;
					if(arrivedPkt->getSrcAddress()==arrivedPkt->getOriginator()){
						EV << "Source address of the packet matches the originator, packet need to be broadcasted to all brokers" << endl;
						for(unsigned i=0;i<connectedBrokers.size();i++){
							if(myAddr!=connectedBrokers[i]){
								AppPacket *fMsg = arrivedPkt->dup();
								fMsg->setDestAddress(connectedBrokers[i]);
								fMsg->setSrcAddress(myAddr);
								send(fMsg,"middleware$o");
							}
						}
					}
					std::pair<std::multimap<std::string,int>::iterator,std::multimap<std::string,int>::iterator> range_subscriptions;
					std::multimap<std::string,int>::iterator it_subscriptions;
					range_subscriptions = subscriptions.equal_range(std::string(arrivedPkt->getTopic()));
					for (it_subscriptions=range_subscriptions.first; it_subscriptions!=range_subscriptions.second; ++it_subscriptions){
						EV << "client with address " << (*it_subscriptions).second << " was registered for the topic, forward message." << endl;
						AppPacket *copy = arrivedPkt->dup();
						copy->setDestAddress((*it_subscriptions).second);
						copy->setPktType(DSP2_PUBLISHED_CONTENT_MESSAGE);
						copy->setSrcAddress(myAddr);
						send(copy,"middleware$o");
					}
				}
				cancelAndDelete(msg);
				break;
			}
			case DSP2_CLIENT_UNSUBSCRIBE_MESSAGE:{
				EV << "received un-subscription message for topic \"" << arrivedPkt->getTopic() << "\" from client address " << arrivedPkt->getSrcAddress() << endl;
				if(myState!=DSP2_BROKER_CONNECTED){
					EV << "current broker is not connected, reply a reset message." << endl;
					AppPacket *reset = new AppPacket();
					reset->setDestAddress(arrivedPkt->getSrcAddress());
					reset->setSrcAddress(myAddr);
					reset->setPktType(DSP2_BROKER_NOT_READY_MESSAGE);
					send(reset,"middleware$o");
				} else {
					std::pair<std::multimap<std::string,int>::iterator,std::multimap<std::string,int>::iterator> range_subscriptions;
					std::multimap<std::string,int>::iterator it_subscriptions;
					range_subscriptions = subscriptions.equal_range(std::string(arrivedPkt->getTopic()));
					for (it_subscriptions=range_subscriptions.first; it_subscriptions!=range_subscriptions.second; ++it_subscriptions){
						if((*it_subscriptions).second==arrivedPkt->getSrcAddress()){
							EV << "Subscription entry found, delete." << endl;
							subscriptions.erase(it_subscriptions);
							EV << "Recorded Latency: " << simTime().dbl()-arrivedPkt->getTimestamp().dbl() << "s" << endl;
							if(simTime().dbl()-arrivedPkt->getTimestamp().dbl() < 0) error("Wrong Latency");
							unsubLatency[arrivedPkt->getSrcAddress()].collect(simTime()-arrivedPkt->getTimestamp());
							break;
						}
					}
				}
				cancelAndDelete(msg);
				break;
			}
			default:{
				error("Broker Application Module: Wrong message type");
				break;
			}
		}
	}
	EV << "**********state of the module**********" << endl;
	EV << "Is Power On Timer Scheduled? " << (powerOnTimer->isScheduled()?" Yes ":" No ") << endl;
	EV << "state of current node is: ";
	switch(myState){
		case DSP2_BROKER_IDLE:
			EV <<"\"IDLE\"" << endl;
			break;
		case DSP2_BROKER_CONNECTED:
			EV <<"\"CONNECTED\"" << endl;
			break;
		default:
			EV <<"\"UNKNOWN\"" << endl;
			error("BROKER Application Module: Unknown State");
			break;
	}
	EV << "contents in connected brokers list is: " << endl;
	for(unsigned i=0;i<connectedBrokers.size();i++){
		EV << connectedBrokers[i] << " ";
	}
	EV << endl << connectedBrokers.size() << " entries in all." << endl;
	EV << "current topology index is: " << topologyIdx << endl;

	EV << "contents in subscription list: " << endl << "Topic | Client id" << endl;
	std::multimap<std::string,int>::iterator it_subscriptions;
	for(it_subscriptions=subscriptions.begin();it_subscriptions!=subscriptions.end();it_subscriptions++){
		EV << "\"" << (*it_subscriptions).first.c_str() << "\" | " << (*it_subscriptions).second << endl;
	}
	EV << subscriptions.size() << " entries in all" << endl;
	EV << "End Broker Application Module Event. " << endl << endl;
}

AppBroker::~AppBroker()
{
	cancelAndDelete(powerOnTimer);
}


