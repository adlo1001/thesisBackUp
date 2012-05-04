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

#include "causalConsistency.h"

Define_Module(CausalConsistency);

void CausalConsistency::initialize()
{
	EV << "Start Causal Consistency Module Initialization: " << endl;
	//initialize the address
	cModule::GateIterator i(this->getParentModule());
	cGate *gate = i();
	cGate *otherGate = gate->getType()==cGate::OUTPUT ? gate->getNextGate() :
		                                                gate->getPreviousGate();
	myAddr = otherGate->getIndex();
	WATCH(myAddr);
	EV << "Current Node Address was set to " << myAddr << endl;
	setVecTimeLength(myAddr + 1);
	EV << "Time Vector size for current node set to " << (myAddr + 1) << endl;
	EV << "End Causal Consistency Module Initialization. " << endl << endl;

	nDropPackets = 0;

	WATCH_VECTOR(vecTime);
}

void CausalConsistency::finish()
{
	recordScalar("#packetsDroped", nDropPackets);
}

bool CausalConsistency::setVecTimeLength(unsigned  length)
{
	while(length > vecTime.size()){
		vecTime.push_back(0);
	}
	return (length - vecTime.size() > 0);
}

void CausalConsistency::handleMessage(cMessage *msg)
{
	EV << "Causal Consistency Module Event Begin:" << endl;
	if(msg->isPacket()){
		if(strcmp(msg->getArrivalGate()->getName(),"upLayer$i")==0){
			AppPacket *arrivedPkt = check_and_cast<AppPacket *>(msg);
			if (arrivedPkt==NULL) {
				error("Causal Consistency Module: received wrong message");
				return;
			}
			//if(arrivedPkt->getPktType()==DSP2_CLIENT_PUBLISH_MESSAGE){
				EV << "Receiving Application Packet from up layer, destination address: " << arrivedPkt->getDestAddress() << " configure for causal order."<< endl;
				EV << "Increment the time index in time vector of current node, ";
				vecTime[myAddr]++;
				EV << "new value is: " << vecTime[myAddr] << endl;
				ConsistencyCtrlPacket* fPkt = new ConsistencyCtrlPacket();
				fPkt->setEnabled(true);
				fPkt->setDestAddr(arrivedPkt->getDestAddress());
				fPkt->setSrcAddr(myAddr);
				fPkt->setVecTimeArraySize(vecTime.size());
				for(unsigned i=0;i<vecTime.size();i++){
					fPkt->setVecTime(i,vecTime[i]);
				}
				fPkt->encapsulate(arrivedPkt);
				EV << "delivering packet to net fault module." << endl;
				send(fPkt,"downLayer$o");
			/*
			} else {
				EV << "Receiving NON-publish Application Packet from up layer, destination address: " << arrivedPkt->getDestAddress() << endl;
				ConsistencyCtrlPacket* fPkt = new ConsistencyCtrlPacket();
				fPkt->setEnabled(false);
				fPkt->setDestAddr(arrivedPkt->getDestAddress());
				fPkt->setSrcAddr(myAddr);
				fPkt->encapsulate(arrivedPkt);
				EV << "delivering packet to net fault module." << endl;
				send(fPkt,"downLayer$o");
			}*/
		} else if (strcmp(msg->getArrivalGate()->getName(),"downLayer$i")==0){
			ConsistencyCtrlPacket* arrivedPkt = check_and_cast<ConsistencyCtrlPacket *>(msg);
			if (arrivedPkt==NULL) {
				error("Causal Consistency Module: received wrong message");
				return;
			}
			EV << "Receiving Consistency Control Packet from down layer, originated by source address " << arrivedPkt->getSrcAddr() << endl;
			if(arrivedPkt->getEnabled()){
				EV << "causal delivery of this packet is required." << endl;
				EV << "Increment the time index in time vector of current node, ";
				vecTime[myAddr]++;
				EV << "new value is: " << vecTime[myAddr] << endl;
				EV << "Starting checking time vector for causal consistency" << endl;
				if(arrivedPkt->getVecTimeArraySize() > vecTime.size()){
					setVecTimeLength(arrivedPkt->getVecTimeArraySize());
					EV << "Time vector size of current node was incremented to " << arrivedPkt->getVecTimeArraySize() << endl;
				}
				int nGT = 0, nLT = 0;
				EV << "index | curNode | packet" << endl;
				for(unsigned i=0;i<arrivedPkt->getVecTimeArraySize();i++){
					EV << i << " | " << vecTime[i] << " | " << arrivedPkt->getVecTime(i) << "   ";
					if (vecTime[i] > arrivedPkt->getVecTime(i)){
						nGT++;
						EV << ">" << endl;
					} else if(vecTime[i] < arrivedPkt->getVecTime(i)){
						nLT++;
						EV << "<" << endl;
					} else {
						EV << "=" << endl;
					}
				}
				for(unsigned i=arrivedPkt->getVecTimeArraySize();i<vecTime.size();i++){
					EV << i << " | " << vecTime[i] << " | 0   ";
					if(vecTime[i]>0){
						nGT++;
						EV << ">" << endl;
					} else {
						EV << "=" << endl;
					}
				}
				bool consistOK;
				if(nLT > 0&& nGT == 0){
					EV << "RESULT: curNode < packet ,OK." << endl;
					consistOK = true;
				} else if(nGT > 0 && nLT ==0){
					EV << "RESULT: curNode > packet ,K.O." << endl;
					consistOK = false;
				} else {
					EV << "RESULT: curNode || packet ,OK." << endl;
					consistOK = true;
				}
				if(consistOK){
					EV << "Causal consistency check passed, deliver packet to up layer." << endl;
					for(unsigned i=0;i<arrivedPkt->getVecTimeArraySize();i++){
						if(arrivedPkt->getVecTime(i) > vecTime[i]) vecTime[i] = arrivedPkt->getVecTime(i);
					}
					cPacket *decapsulatedPkt = arrivedPkt->decapsulate();
					send(decapsulatedPkt,"upLayer$o");
					cancelAndDelete(msg);
				} else {
					EV << "Causal consistency check failed, drop packet." << endl;
					nDropPackets++;
					cancelAndDelete(msg);
				}
			} else {
				EV << "causal delivery of this packet is NOT required, deliver packet to up layer." << endl;
				cPacket *decapsulatedPkt = arrivedPkt->decapsulate();
				send(decapsulatedPkt,"upLayer$o");
				cancelAndDelete(msg);
			}
		} else {
			error("Causal Consistency Module: received wrong message");
			return;
		}
	} else {
		error("Causal Consistency Module: received wrong message");
		return;
	}
	EV << "**********state of the module**********" << endl;
	EV << "Contents in time vector:" << endl << "(index, time)" << endl;
	for(unsigned i=0;i<vecTime.size();i++){
		EV << "(" << i << "," << vecTime[i] << ")" << endl;
	}
	EV << "Causal Consistency Module Event End." << endl << endl;
}

CausalConsistency::~CausalConsistency()
{
}


