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

#include "appClient.h"

Define_Module(AppClient);

void AppClient::initialize()
{
	EV << "Start Client Application Module Initialization: " << endl;
	//initialize the address
	cModule::GateIterator i(this->getParentModule());
	cGate *gate = i();
	cGate *otherGate = gate->getType()==cGate::OUTPUT ? gate->getNextGate() :
		                                                gate->getPreviousGate();
	myAddr = otherGate->getIndex();
	WATCH(myAddr);
	EV << "Current Node Address was set to " << myAddr << endl;
	myBroker = 0;
	EV << "End Client Application Module Initialization. " << endl << endl;
}

void AppClient::finish()
{
	std::map<int,cDoubleHistogram>::iterator pubRecIt;
	for(pubRecIt = pubLatency.begin(); pubRecIt!=pubLatency.end();pubRecIt++){
		std::stringstream recNameSt;
		recNameSt << "Publish Latency (Client address " << (*pubRecIt).first << ")";
		(*pubRecIt).second.recordAs(recNameSt.str().c_str());
	}
}

void AppClient::handleMessage(cMessage *msg)
{
	EV << "Start Client Application Module Event. " << endl;
	if(!msg->isSelfMessage()){
		if(strcmp(msg->getArrivalGate()->getName(),"user$i")==0){
			UserCommand *usrCmd = check_and_cast<UserCommand *>(msg);
			switch (usrCmd->getCmdType()){
				case DSP2_USER_PUBLISH_COMMAND:{
					EV << "Received user publish command for topic \"" << usrCmd->getTopic() << "\", send packet to middleware." << endl;
					AppPacket *pubReq = new AppPacket();
					pubReq->setSrcAddress(myAddr);
					pubReq->setDestAddress(myBroker);
					pubReq->setOriginator(myAddr);
					pubReq->setTopic(usrCmd->getTopic());
					pubReq->setContent(usrCmd->getContent());
					pubReq->setPktType(DSP2_CLIENT_PUBLISH_MESSAGE);
					pubReq->setTimestamp();
					send(pubReq,"middleware$o");
					cancelAndDelete(msg);
					break;
				}
				case DSP2_USER_SUBSCRIBE_COMMAND:{
					EV << "Received user subscribe command for topic \"" << usrCmd->getTopic() << "\", send packet to middleware." << endl;
					AppPacket *subReq = new AppPacket();
					subReq->setSrcAddress(myAddr);
					subReq->setDestAddress(myBroker);
					subReq->setOriginator(myAddr);
					subReq->setTopic(usrCmd->getTopic());
					subReq->setPktType(DSP2_CLIENT_SUBSCRIBE_MESSAGE);
					subReq->setTimestamp();
					send(subReq,"middleware$o");
					cancelAndDelete(msg);
					break;
				}
				case DSP2_USER_UNSUBSCRIBE_COMMAND:{
					EV << "Received user un-subscribe command for topic \"" << usrCmd->getTopic() << "\", send packet to middleware." << endl;
					AppPacket *unsubReq = new AppPacket();
					unsubReq->setSrcAddress(myAddr);
					unsubReq->setDestAddress(myBroker);
					unsubReq->setOriginator(myAddr);
					unsubReq->setTopic(usrCmd->getTopic());
					unsubReq->setPktType(DSP2_CLIENT_UNSUBSCRIBE_MESSAGE);
					unsubReq->setTimestamp();
					send(unsubReq,"middleware$o");
					cancelAndDelete(msg);
					break;
				}
				case DSP2_USER_SET_BROKER:{
					EV << "Reveived set broker user command, ";
					myBroker = usrCmd->getBroker();
					EV << "broker for current node was set to " << myBroker << endl;
					cancelAndDelete(msg);
					break;
				}
			}
		} else if (strcmp(msg->getArrivalGate()->getName(),"middleware$i")==0){
			AppPacket *arrivedPkt = check_and_cast<AppPacket *>(msg);
			switch (arrivedPkt->getPktType()){
				case DSP2_PUBLISHED_CONTENT_MESSAGE:{
					EV << "Received publish content message for topic \"" << arrivedPkt->getTopic() << "\" from originator address "
					   << arrivedPkt->getOriginator() << ", send reply to user."<< endl;
					SysReply *rep = new SysReply();
					rep->setRepType(DSP2_PUBLISHED_MESSAGE);
					rep->setTopic(arrivedPkt->getTopic());
					rep->setContent(arrivedPkt->getContent());
					rep->setOriginator(arrivedPkt->getOriginator());
					send(rep,"user$o");
					EV << "Recorded Latency: " << simTime().dbl()-arrivedPkt->getTimestamp().dbl() << "s" << endl;
					if(simTime().dbl()-arrivedPkt->getTimestamp().dbl() < 0) error("Wrong Latency");
					pubLatency[arrivedPkt->getOriginator()].collect(simTime()-arrivedPkt->getTimestamp());
					cancelAndDelete(msg);
					break;
				}
				case DSP2_BROKER_NOT_READY_MESSAGE:{
					EV << "Received broker not ready message, send reply to user."<< endl;
					SysReply *rep = new SysReply();
					rep->setRepType(DSP2_BROKER_NOT_READY);
					send(rep,"user$o");
					cancelAndDelete(msg);
					break;
				}
				default:{
					error("Client Application Module: Unknown message type");
					break;
				}
			}
		} else {
			error("Client Application Module: Received unknown message");
			return;
		}
	} else {
		error("Client Application Module: Unknown self-message");
		return;
	}

	EV << "State: this node was configured to send request to broker address " << myBroker << endl;
	EV << "End Client Application Module Event. " << endl << endl;
}

AppClient::~AppClient()
{

}


