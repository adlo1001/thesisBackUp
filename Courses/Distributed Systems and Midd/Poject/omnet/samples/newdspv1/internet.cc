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


//Here Xu use generate randomdelay, but why not using a customized channel, later maybe we can improve this
#include "internet.h"
#include "dsp_type_def.h"

Define_Module(Internet);

void Internet::initialize()
{
	msg_forward_timeout = new InternetMsg();
	msg_forward_timeout ->setMsgType(DSP_SELF_INTERNET_MESSAGE_FORWARD_TIMER);
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

	//print out the initialized array
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
		EV << "**************************************************************************" << endl << endl;
	}
}

void Internet::finish()
{
	for(int i=0;i<nParamArray;i++){
		for(int j=0;j<nConnectedNodes;j++){
			delete[] netConditionParam[i][j];
		}
		delete[] netConditionParam[i];
	}
	delete[] netConditionParam;
	cancelAndDelete(msg_forward_timeout);
	while (!messageQueue.empty()){
		messageQueue.pop_front();
	}
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

void Internet::insertMsg(TimestampedMsgRef t){
	if(messageQueue.empty()) {messageQueue.push_back(t);return;}
    ite=messageQueue.begin();
    while(ite!=messageQueue.end()) {
    	if(ite->getTimestamp()>=t.getTimestamp()) {messageQueue.insert(ite,t);return;}
    	else ite++;
							}
}

void Internet::handleMessage(cMessage *msg)
{
	InternetMsg *tempmsg = check_and_cast<InternetMsg *>(msg);
	if (tempmsg==NULL) { error("Unknown message type"); return; }
	switch(tempmsg->getMsgType()){
		case DSP_SELF_INTERNET_MESSAGE_FORWARD_TIMER:{
			InternetMsg *queuedMsg =check_and_cast<InternetMsg *>(messageQueue.begin()->getMsg());
			send(queuedMsg,"out",queuedMsg->getDestination());
	        messageQueue.pop_front();
			if(!messageQueue.empty()){
			cancelEvent(msg_forward_timeout);
			scheduleAt(messageQueue.begin()->getTimestamp(),msg_forward_timeout);break;
			}
			/* queue debug
			std::priority_queue<TimestampedMsgRef> temp;
			while (!messageQueue.empty()){
				TimestampedMsgRef tref = messageQueue.top();
				messageQueue.pop();
				temp.push(tref);
				EV << "time: " << tref.getTimestamp() << " message: " << (void *)tref.getMsg() << endl;;
			}
			while(!temp.empty()){
				messageQueue.push(temp.top());
				temp.pop();
			}
			*/
			else cancelEvent(msg_forward_timeout);break;
		}
		default:{
			if(tempmsg->getDestination()==-1){
				for(int j=0;j<nConnectedNodes;j++){
					if(!lostPacket(tempmsg->getSource(),j)){
						InternetMsg *sForward = tempmsg->dup();
						sForward->setDestination(j);
						double delayTime = generateRandomDelay(tempmsg->getSource(),j);
						if(delayTime <= 0){
							send(sForward,"out",j);
						} else {
							TimestampedMsgRef *temptimestampedmsgref = new TimestampedMsgRef(simTime().dbl()+delayTime,tempmsg);
							insertMsg(*temptimestampedmsgref);
							//messageQueue.push(TimestampedMsgRef(simTime().dbl() + delayTime,sForward));
							/* queue debug
							std::priority_queue<TimestampedMsgRef> temp;
							while (!messageQueue.empty()){
								TimestampedMsgRef tref = messageQueue.top();
								messageQueue.pop();
								temp.push(tref);
								EV << "time: " << tref.getTimestamp() << " message: " << (void *)tref.getMsg() << endl;;
								}
								while(!temp.empty()){
									messageQueue.push(temp.top());
									temp.pop();
								}
							*/
							cancelEvent(msg_forward_timeout);
							scheduleAt(messageQueue.begin()->getTimestamp(),msg_forward_timeout);
						}
					}
				}
				cancelAndDelete(msg);
			} else {
				if(!lostPacket(tempmsg->getSource(),tempmsg->getDestination())){
					double delayTime = generateRandomDelay(tempmsg->getSource(),tempmsg->getDestination());
					if(delayTime <= 0){
						send(tempmsg,"out",tempmsg->getDestination());
					} else {
						TimestampedMsgRef *temptimestampedmsgref = new TimestampedMsgRef(simTime().dbl()+delayTime,tempmsg);
						insertMsg(*temptimestampedmsgref);
						//messageQueue.push(TimestampedMsgRef(simTime().dbl() + delayTime,tempmsg));
						/* queue debug
						std::priority_queue<TimestampedMsgRef> temp;
						while (!messageQueue.empty()){
							TimestampedMsgRef tref = messageQueue.top();
							messageQueue.pop();
							temp.push(tref);
							EV << "time: " << tref.getTimestamp() << " message: " << (void *)tref.getMsg() << endl;;
							}
							while(!temp.empty()){
								messageQueue.push(temp.top());
								temp.pop();
							}
						*/
						cancelEvent(msg_forward_timeout);
						scheduleAt(messageQueue.begin()->getTimestamp(),msg_forward_timeout);
					}
				} else {
					cancelAndDelete(msg);
				}
			}
			break;
		}
	}

	return;
}
