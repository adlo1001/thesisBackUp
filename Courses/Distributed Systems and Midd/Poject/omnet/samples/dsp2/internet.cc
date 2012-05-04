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

#include "internet.h"

Define_Module(Internet);

void Internet::initialize()
{

	//New
	delaySignal = registerSignal("delay");

	EV << "Start Internet Module Initialization: " << endl;
	msgForwardingTimer = new SelfMessage();
	msgForwardingTimer->setType(DSP2_INTERNET_MESSAGE_FORWARD_TIMER);
	EV << "Message Forwarding Timer (used by the packet queue) was initialized." << endl;


	//Load configuration file and get the array size defined in the file
	confFileLocation =  par("confFile").stringValue();
	std::ifstream confFile(confFileLocation);
	if (!confFile) {
		error("Failed to load Internet Configuration File.");
	}
	int confArraySize;
	std::string currLineConfFile;
	confFile >> confArraySize;
	confFile >> nParamArray;
	if(nParamArray<=0||nParamArray>5) error("Wrong Internet configuration File");
	getline(confFile,currLineConfFile);//consume the first line

	//get the number of nodes
	cModule::GateIterator i(this);
	cGate *gate = i();
	nConnectedNodes=gate->getVectorSize();

	//malloc the arrays
	netConditionParam = new double**[nParamArray];
	for(int i=0;i<nParamArray;i++){
		netConditionParam[i] = new double*[nConnectedNodes];
		for(int j=0;j<nConnectedNodes;j++){
			netConditionParam[i][j] = new double[nConnectedNodes];
		}
	}

	//fill the parameters
	for(int i=0;i<nParamArray;i++){
		if(i!=0){
			std::stringstream coeffLabel;
			coeffLabel << "coeff" << i;
			while(getline(confFile, currLineConfFile) && currLineConfFile.compare(coeffLabel.str())!=0);
			if(confFile.eof()) error("Wrong Internet configuration File");
		}
		for(int j=0;(j<nConnectedNodes && j<confArraySize);j++){
			if(getline(confFile, currLineConfFile)){
				std::istringstream tokenizer(currLineConfFile);
				std::string token;
				for(int k=0;(k<nConnectedNodes && k<confArraySize);k++){
					if(getline(tokenizer, token, ' ')){
						netConditionParam[i][j][k] = atof(token.c_str());
					} else {
						error("Wrong Internet configuration File");
					}
				}
				for(int k=0;k<nConnectedNodes-confArraySize;k++){
					i==0 ? netConditionParam[i][j][k+confArraySize] = 1 : netConditionParam[i][j][k+confArraySize] = 0;
				}
			} else {
				error("Wrong Internet configuration File");
			}
		}
		for(int j=0;j<nConnectedNodes-confArraySize;j++){
			for(int k=0;k<nConnectedNodes;k++){
				i==0 ? netConditionParam[i][j+confArraySize][k] = 1 : netConditionParam[i][j+confArraySize][k] = 0;
			}
		}
	}
	confFile.close();

	EV << "Network parameters were initialized from the file \"" << confFileLocation << "\", parameters matrix are: " << endl;

	for(int i=0;i<nParamArray;i++){
		EV << "**************************************************************************" << endl;
		EV << "************** Initialized Link Condition Matrix - index: " << i << " **************" << endl;
		EV << "**************************************************************************" << endl;
		for (int j=0;j<nConnectedNodes;j++){
			for (int k=0;k<nConnectedNodes;k++){
				EV << netConditionParam[i][j][k] << " ";
			}
			EV << endl;
		}
		EV << "**************************************************************************" << endl;
	}

	EV << "End Internet Module Initialization. " << endl << endl;

}

void Internet::finish()
{

}

double Internet::generateRandomDelay(int sourceAddr, int destAddr)
{
	if(sourceAddr>=nConnectedNodes||destAddr>=nConnectedNodes) return 0;
	double retValue = 0;
	switch((int)netConditionParam[0][sourceAddr][destAddr]){
		case DSP_OMNET_EXPONENTIAL_DISTRIBUTION:
			if(nParamArray < 3) break;
			retValue = exponential(netConditionParam[2][sourceAddr][destAddr],sourceAddr + nConnectedNodes * destAddr + DSP_NET_PARAM_RNG_ID_OFFSET);
			break;
		case DSP_OMNET_UNIFORM_DISTRIBUTION:
			if(nParamArray < 4) break;
			retValue = uniform(netConditionParam[2][sourceAddr][destAddr],netConditionParam[3][sourceAddr][destAddr],sourceAddr + nConnectedNodes * destAddr + DSP_NET_PARAM_RNG_ID_OFFSET);
			break;
		case DSP_OMNET_NORMAL_DISTRIBUTION:
			if(nParamArray < 4) break;
			retValue = normal(netConditionParam[2][sourceAddr][destAddr],netConditionParam[3][sourceAddr][destAddr],sourceAddr + nConnectedNodes * destAddr + DSP_NET_PARAM_RNG_ID_OFFSET);
			break;
		case DSP_OMNET_TRUNCNORMAL_DISTRIBUTION:
			if(nParamArray < 4) break;
			retValue = truncnormal(netConditionParam[2][sourceAddr][destAddr],netConditionParam[3][sourceAddr][destAddr],sourceAddr + nConnectedNodes * destAddr + DSP_NET_PARAM_RNG_ID_OFFSET);
			break;
		case DSP_OMNET_SHIFTED_PARETO_DISTRIBUTION:
			if(nParamArray < 5) break;
			retValue = pareto_shifted(netConditionParam[2][sourceAddr][destAddr],netConditionParam[3][sourceAddr][destAddr],netConditionParam[4][sourceAddr][destAddr],sourceAddr + nConnectedNodes * destAddr + DSP_NET_PARAM_RNG_ID_OFFSET);
			break;
		default:
			break;
	}
	return retValue >= 0 ? retValue : -retValue;
}

bool Internet::lostPacket(int sourceAddr, int destAddr)
{
	if(sourceAddr>=nConnectedNodes||destAddr>=nConnectedNodes) return false;
	double randomNum = uniform(0,100,sourceAddr + nConnectedNodes * (destAddr + nConnectedNodes) + DSP_NET_PARAM_RNG_ID_OFFSET) / 100;
	return (randomNum <= netConditionParam[1][sourceAddr][destAddr]);
}

void Internet::handleMessage(cMessage *msg)
{
	//New
	double delayOnInternet = 0.0;

	EV << "Internet Module Event Begin:" << endl;
	if(msg->isPacket()) {
		InternetPacket *arrivedPkt = check_and_cast<InternetPacket *>(msg);
		if (arrivedPkt==NULL) {
			error("Internet Module: received wrong message");
			return;
		}
		EV << "Received net packet from node address " << arrivedPkt->getSrcAddress() << ", sequence number " << arrivedPkt->getSeqNumber() << ", ";
		if(arrivedPkt->getDestAddress()==-1){
			EV << "packet is for broadcasting." << endl;
			for(int j=0;j<nConnectedNodes;j++){
				if(!lostPacket(arrivedPkt->getSrcAddress(),j)){
					InternetPacket *sForward = arrivedPkt->dup();
					sForward->setDestAddress(j);
					double delayTime = generateRandomDelay(arrivedPkt->getSrcAddress(),j);
					if(delayTime <= 0){

						//New
						delayOnInternet = 0.0;
						emit(delaySignal, delayOnInternet);

						EV << "no delay for packet for destination " << j << " send immediately." << endl;
						send(sForward,"toNode$o",j);
					} else {

						//New
						delayOnInternet = delayTime;
						emit(delaySignal, delayOnInternet);

						EV << "packet for destination " << j << " will be forwarded in " << delayTime << " seconds." << endl;
						packetQueue.push(TimstampedPktRef(simTime().dbl() + delayTime,sForward));
					}
				} else {
					EV << "packet for destination " << j << " is lost." << endl;
				}
			}
			cancelAndDelete(msg);
		} else {
			if(!lostPacket(arrivedPkt->getSrcAddress(),arrivedPkt->getDestAddress())){
				EV << "destination is node address " << arrivedPkt->getDestAddress() << "." << endl;
				double delayTime = generateRandomDelay(arrivedPkt->getSrcAddress(),arrivedPkt->getDestAddress());
				if(delayTime <= 0){

					//New
					delayOnInternet = 0.0;
					emit(delaySignal, delayOnInternet);

					EV << "no delay for this packet send immediately." << endl;
					send(arrivedPkt,"toNode$o",arrivedPkt->getDestAddress());
				} else {

					//New
					delayOnInternet = delayTime;
					emit(delaySignal, delayOnInternet);

					EV << "packet will be forwarded in " << delayTime << " seconds." << endl;
					packetQueue.push(TimstampedPktRef(simTime().dbl() + delayTime,arrivedPkt));
				}
			} else {
				EV << "packet is lost." << endl;
				cancelAndDelete(msg);
			}
		}
	cancelEvent(msgForwardingTimer);
	if(!packetQueue.empty()) scheduleAt(packetQueue.top().getTimestamp(),msgForwardingTimer);
	} else {
		SelfMessage *selfMsg = check_and_cast<SelfMessage *>(msg);
		if (selfMsg==NULL) {
			error("Internet Module: received wrong message");
			return;
		}
		switch(selfMsg->getType()){
			case DSP2_INTERNET_MESSAGE_FORWARD_TIMER:{
				EV << "Message Forwarding Timer was fired, sending the first package in the packet queue." << endl;
				InternetPacket *queuedPkt = check_and_cast<InternetPacket *>(packetQueue.top().getPkt());
				EV << "Source address of the packet is " << queuedPkt->getSrcAddress() <<
					  ", destination address of the packet is " << queuedPkt->getDestAddress() <<
					  ", sequence number of the packet is " << queuedPkt->getSeqNumber() << endl;
				send(queuedPkt,"toNode$o",queuedPkt->getDestAddress());
				packetQueue.pop();
				if(!packetQueue.empty()){
					scheduleAt(packetQueue.top().getTimestamp(),msgForwardingTimer);
				}
				break;
			}
			default:{
				EV << "Warning: received unknown type of self-message." << endl;
				break;
			}
		}
	}
	EV << "**********state of the module**********" << endl;
	EV << "Is Message Forwarding Timer Scheduled? " << (msgForwardingTimer->isScheduled()?" Yes ":" No ") << endl;
	EV << "Packet Queue now contains " << packetQueue.size() << " packages." << endl;
	EV << "Internet Module Event End." << endl << endl;
}

Internet::~Internet()
{
	for(int i=0;i<nParamArray;i++){
		for(int j=0;j<nConnectedNodes;j++){
			delete[] netConditionParam[i][j];
		}
		delete[] netConditionParam[i];
	}
	delete[] netConditionParam;
	cancelAndDelete(msgForwardingTimer);
	while (!packetQueue.empty()){
		delete(packetQueue.top().getPkt());
		packetQueue.pop();
	}
}


