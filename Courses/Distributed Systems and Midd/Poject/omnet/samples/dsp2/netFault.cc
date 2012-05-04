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

#include "netFault.h"

Define_Module(NetFault);

void NetFault::initialize()
{
	EV << "Start Network Fault Tolerance Module Initialization: " << endl;
	//initialize the address
	cModule::GateIterator i(this->getParentModule());
	cGate *gate = i();
	cGate *otherGate = gate->getType()==cGate::OUTPUT ? gate->getNextGate() :
		                                                gate->getPreviousGate();
	myAddr = otherGate->getIndex();
	WATCH(myAddr);
	EV << "Current Node Address was set to " << myAddr << endl;
	EV << "End Network Fault Tolerance Initialization. " << endl << endl;

	nDataPktSent = 0;
	nNackPktSent = 0;

	WATCH_MAP(rcvSeqNumber);
	WATCH_MAP(sendSeqNumber);
	WATCH_MAP(rcvCachedPackets);
	WATCH_MAP(sendCachedPackets);
}

void NetFault::finish()
{
	recordScalar("#dataPacketsSent", nDataPktSent);
	recordScalar("#nackPacketsSent", nNackPktSent);
}


void NetFault::handleMessage(cMessage *msg)
{
	EV << "Network Fault Tolerance Module Event Begin:" << endl;
	if(msg->isPacket()){
		if(strcmp(msg->getArrivalGate()->getName(),"upLayer$i")==0){
			ConsistencyCtrlPacket *arrivedPkt = check_and_cast<ConsistencyCtrlPacket *>(msg);
			if (arrivedPkt==NULL) {
				error("Network Fault Tolerance Module: received wrong message");
				return;
			}
			EV << "Received Consistency Control Packet from up layer, destination address " << arrivedPkt->getDestAddr() << ", deliver it to network." << endl;
			EV << "Updating sequence number, ";
			sendSeqNumber[arrivedPkt->getDestAddr()]++;
			EV << "new value is " << sendSeqNumber[arrivedPkt->getDestAddr()] << endl;
			InternetPacket *fPkt = new InternetPacket();
			fPkt->setAckSeqNumber(rcvSeqNumber[arrivedPkt->getDestAddr()]);
			fPkt->setSeqNumber(sendSeqNumber[arrivedPkt->getDestAddr()]);
			fPkt->setSrcAddress(myAddr);
			fPkt->setDestAddress(arrivedPkt->getDestAddr());
			fPkt->setPktType(DSP2_INTERNET_DATA_PACKET);
			fPkt->encapsulate(arrivedPkt);
			EV << "caching the packet to be sent to sending cache." << endl;
			sendCachedPackets[arrivedPkt->getDestAddr()][sendSeqNumber[arrivedPkt->getDestAddr()]]=fPkt->dup();
			nDataPktSent++;
			send(fPkt,"downLayer$o");
		} else if (strcmp(msg->getArrivalGate()->getName(),"downLayer$i")==0){
			InternetPacket *arrivedPkt = check_and_cast<InternetPacket *>(msg);
			if (arrivedPkt==NULL) {
				error("Network Fault Tolerance Module: received wrong message");
				return;
			}
			switch(arrivedPkt->getPktType()){
				case DSP2_INTERNET_DATA_PACKET:{
					EV << "Received Internet Packet from source address " << arrivedPkt->getSrcAddress()
					   << " to destination address " << arrivedPkt->getDestAddress() << " sequence number " << arrivedPkt->getSeqNumber() << endl;
					EV << "ACK sequence number in the package is " << arrivedPkt->getAckSeqNumber() << endl;
					std::map<int, CachedPacketsEntry >::iterator it_o;
					it_o = sendCachedPackets.find(arrivedPkt->getSrcAddress());
					if(it_o!=sendCachedPackets.end()){
						CachedPacketsEntry::iterator it_i = (*it_o).second.find(arrivedPkt->getAckSeqNumber());
						if(it_i!=(*it_o).second.end()){
							CachedPacketsEntry::iterator it_l = (*it_o).second.begin();
							while(it_l!=it_i){
								EV << "delete packet in send cache sequence number " << (*it_l).first << endl;
								cancelAndDelete((*it_l).second);
								(*it_o).second.erase(it_l);
								it_l++;
							}
							EV << "delete packet in send cache sequence number " << (*it_i).first << endl;
							cancelAndDelete((*it_i).second);
							(*it_o).second.erase(it_i);
						}
					}
					rcvSeqNumber[arrivedPkt->getSrcAddress()];
					EV << "sequence number of the correspond source stored locally: " << rcvSeqNumber[arrivedPkt->getSrcAddress()] << endl;
					std::map<int,unsigned long>::iterator itSeqNum = rcvSeqNumber.find(arrivedPkt->getSrcAddress());
					if((*itSeqNum).second >= arrivedPkt->getSeqNumber()){
						EV << "packet with same source and sequence number already delivered, drop packet." << endl;
						cancelAndDelete(arrivedPkt);
					} else if (arrivedPkt->getSeqNumber()==(*itSeqNum).second + 1){
						EV << "sequence number check passed, deliver packet to up layer and increment sequence number." << endl;
						cPacket *decapsulatedPkt = arrivedPkt->decapsulate();
						send(decapsulatedPkt,"upLayer$o");
						//update the sequence number
						(*itSeqNum).second = arrivedPkt->getSeqNumber();
						it_o = rcvCachedPackets.find(arrivedPkt->getSrcAddress());
						if(it_o!=rcvCachedPackets.end()){
							while((*it_o).second.find(rcvSeqNumber[arrivedPkt->getSrcAddress()] + 1)!=(*it_o).second.end()){
								EV << "packet from " << arrivedPkt->getSrcAddress() << " with sequence number " << rcvSeqNumber[arrivedPkt->getSrcAddress()] + 1
								   << " was cached before, deliver it to up layer." << endl;
								decapsulatedPkt = rcvCachedPackets[arrivedPkt->getSrcAddress()][rcvSeqNumber[arrivedPkt->getSrcAddress()] + 1]->decapsulate();
								send(decapsulatedPkt,"upLayer$o");
								cancelAndDelete(rcvCachedPackets[arrivedPkt->getSrcAddress()][rcvSeqNumber[arrivedPkt->getSrcAddress()] + 1]);
								(*rcvCachedPackets.find(arrivedPkt->getSrcAddress())).second.erase((*rcvCachedPackets.find(arrivedPkt->getSrcAddress())).second.find(rcvSeqNumber[arrivedPkt->getSrcAddress()] + 1));
								rcvSeqNumber[arrivedPkt->getSrcAddress()]++;
							}
						}

						cancelAndDelete(msg);
					} else {
						EV << "possible loss of packets, cache current packet and send NACK packets" << endl;
						for(unsigned long i=0;i<arrivedPkt->getSeqNumber()-rcvSeqNumber[arrivedPkt->getSrcAddress()]-1;i++){
							it_o = rcvCachedPackets.find(arrivedPkt->getSrcAddress());
							bool cached = false;
							if(it_o!=rcvCachedPackets.end()){
								CachedPacketsEntry::iterator it_i = (*it_o).second.find(arrivedPkt->getSeqNumber());
								if(it_i!=(*it_o).second.end()){
									cached = true;
								}
							}
							if(!cached){
								InternetPacket *nackPkt = new InternetPacket();
								nackPkt->setDestAddress(arrivedPkt->getSrcAddress());
								nackPkt->setSrcAddress(myAddr);
								nackPkt->setAckSeqNumber(rcvSeqNumber[arrivedPkt->getSrcAddress()]);
								nackPkt->setPktType(DSP2_INTERNET_NACK_PACKET);
								nackPkt->setSeqNumber(arrivedPkt->getSeqNumber()-1-i);
								EV << "Sending NACK packet with sequence number " << nackPkt->getSeqNumber() << " to node with address " << nackPkt->getDestAddress() << endl;
								nNackPktSent++;
								send(nackPkt,"downLayer$o");
							}
						}
						//actually we should ckeck first, if the packet is already cached, just drop the newly arrived one.
						it_o = rcvCachedPackets.find(arrivedPkt->getSrcAddress());
						bool found = false;
						if(it_o!=rcvCachedPackets.end()){
							CachedPacketsEntry::iterator it_i = (*it_o).second.find(arrivedPkt->getSeqNumber());
							if(it_i!=(*it_o).second.end()){
								found = true;
								cancelAndDelete(arrivedPkt);
							}
						}
						if(!found) rcvCachedPackets[arrivedPkt->getSrcAddress()][arrivedPkt->getSeqNumber()] = arrivedPkt;
					}
					break;
				}
				case DSP2_INTERNET_NACK_PACKET:{
					EV << "Received NACK packet from node address " << arrivedPkt->getSrcAddress() << " for sequence number " << arrivedPkt->getSeqNumber();
					EV << "ACK sequence number in the package is " << arrivedPkt->getAckSeqNumber() << endl;
					std::map<int, CachedPacketsEntry >::iterator it_o;
					it_o = sendCachedPackets.find(arrivedPkt->getSrcAddress());
					if(it_o!=sendCachedPackets.end()){
						CachedPacketsEntry::iterator it_i = (*it_o).second.find(arrivedPkt->getAckSeqNumber());
						if(it_i!=(*it_o).second.end()){
							CachedPacketsEntry::iterator it_l = (*it_o).second.begin();
							while(it_l!=it_i){
								EV << "delete packet in send cache sequence number " << (*it_l).first << endl;
								cancelAndDelete((*it_l).second);
								(*it_o).second.erase(it_l);
								it_l++;
							}
							EV << "delete packet in send cache sequence number " << (*it_i).first << endl;
							cancelAndDelete((*it_i).second);
							(*it_o).second.erase(it_i);
						}
					}
					it_o = sendCachedPackets.find(arrivedPkt->getSrcAddress());
					bool resent = false;
					if(it_o!=sendCachedPackets.end()){
						CachedPacketsEntry::iterator it_i = (*it_o).second.find(arrivedPkt->getSeqNumber());
						if(it_i!=(*it_o).second.end()){
							cPacket *resendPkt = (*it_i).second->dup();
							send(resendPkt,"downLayer$o");
							nDataPktSent++;
							resent = true;
						}
					}
					if(resent){
						EV << ", packet was re-sent."<< endl;
					} else {
						//We can do this because this only happens when the net is not FIFO, and the packet is in real successfully delivered.
						EV << ", packet was not found in the cache, just ignore." << endl;
					}
					cancelAndDelete(msg);
					break;
				}
				default:{
					break;
				}
			}
		} else {
			error("Network Fault Tolerance Module: received wrong message");
			return;
		}
	} else {
		error("Network Fault Tolerance Module: received wrong message");
		return;
	}
	EV << "**********state of the module**********" << endl;
	EV << "Contents in receiving sequence number map:" << endl << "(source, sequence number)" << endl;
	for(std::map<int,unsigned long>::iterator it=rcvSeqNumber.begin();it!=rcvSeqNumber.end();it++){
		EV << "("<< (*it).first <<"," << (*it).second << ")" << endl;
	}
	EV << rcvSeqNumber.size() << " entries in all." << endl;
	EV << "Contents in sending sequence number map:" << endl << "(destination, sequence number)" << endl;
	for(std::map<int,unsigned long>::iterator it=sendSeqNumber.begin();it!=sendSeqNumber.end();it++){
		EV << "("<< (*it).first <<"," << (*it).second << ")" << endl;
	}
	EV << sendSeqNumber.size() << " entries in all." << endl;
	EV << "Cached receiving packets: " << endl << "(source, sequence number)" << endl;
	int size = 0;
	for(std::map<int, CachedPacketsEntry>::iterator it_o=rcvCachedPackets.begin();it_o!=rcvCachedPackets.end();it_o++){
		size+=(*it_o).second.size();
		for(CachedPacketsEntry::iterator it_i=(*it_o).second.begin();it_i!=(*it_o).second.end();it_i++){
			EV << "(" << (*it_o).first << "," << (*it_i).first << ")" << endl;
		}

	}
	EV << size << " entries in all." << endl;
	EV << "Cached sending packets: " << endl << "(destination, sequence number)" << endl;
	size = 0;
	for(std::map<int, CachedPacketsEntry>::iterator it_o=sendCachedPackets.begin();it_o!=sendCachedPackets.end();it_o++){
		size+=(*it_o).second.size();
		for(CachedPacketsEntry::iterator it_i=(*it_o).second.begin();it_i!=(*it_o).second.end();it_i++){
			EV << "(" << (*it_o).first << "," << (*it_i).first << ")"<< endl;
		}

	}
	EV << size << " entries in all." << endl;
	EV << "Network Fault Tolerance Module Event End." << endl << endl;
}

NetFault::~NetFault()
{
	for(std::map<int, CachedPacketsEntry>::iterator it_o=rcvCachedPackets.begin();it_o!=rcvCachedPackets.end();it_o++){
		for(CachedPacketsEntry::iterator it_i=(*it_o).second.begin();it_i!=(*it_o).second.end();it_i++){
			cancelAndDelete((*it_i).second);
		}
	}
	for(std::map<int, CachedPacketsEntry>::iterator it_o=sendCachedPackets.begin();it_o!=sendCachedPackets.end();it_o++){
		for(CachedPacketsEntry::iterator it_i=(*it_o).second.begin();it_i!=(*it_o).second.end();it_i++){
			cancelAndDelete((*it_i).second);
		}
	}
}

std::ostream& operator <<(std::ostream & out, CachedPacketsEntry & tr)
{
	CachedPacketsEntry::iterator it;
	for(it=tr.begin();it!=tr.end();it++){
		out << std::endl << " " <<(*it).first << " --> " << *((*it).second) << " " << (void *)((*it).second);
	}
	return out;
}






