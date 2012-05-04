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

#include "consistency.h"
#include "dsp_type_def.h"
#include "internetMsg_m.h"
#include <sstream>
using namespace std;

#include <math.h>

Define_Module(Consistency);

void Consistency::initialize()
{
	cModule::GateIterator i(this->getParentModule());
			cGate *gate = i();
			cGate *otherGate = gate->getType()==cGate::OUTPUT ? gate->getNextGate() :
				                                                gate->getPreviousGate();
			addr = otherGate->getIndex();
			WATCH(addr);

            globaltimeaftersyn = 0;
            agreedlocaltime = 0;

			currStat = DSP_CONSIS_IDLE;

			nthsyn = 1;

			prevtoappmsgtimestamp = 0;

			SynTimeout = new InternetMsg();
			SynTimeout ->setMsgType(DSP_SYN_TIMEOUT);

			nextSyn = new InternetMsg();
			nextSyn->setMsgType(DSP_NEXT_SYN);
			toleratedClockDiff= par("toleratedClockDiff");
			if(addr != 0) {
			localclockdriftrate = par("localclockdriftrate");}
			else {localclockdriftrate = 1;}
			EV<<"The drift var in consistency module of node "<<addr<<" is "<<localclockdriftrate<<endl;
			if(localclockdriftrate!=1){
			synperiod = toleratedClockDiff/fabs(localclockdriftrate-1);
			EV<<"The syn period in consistency module of node "<<addr<<" is "<<synperiod<<endl;
		 	scheduleAt(nthsyn*synperiod,nextSyn);
			}
			else {EV<<"The  consistency module of node "<<addr<<" doesn't need synchronization "<<endl;}

			sendlocalclocktoapp(globaltimeaftersyn,agreedlocaltime);
}

void Consistency::handleMessage(cMessage *msg)
{
	InternetMsg *tempmsg = check_and_cast<InternetMsg *>(msg);
	switch (tempmsg->getMsgType()) {
	case DSP_NEXT_SYN: {
		synchronizing();
		cancelEvent(msg);
		nthsyn++;
		scheduleAt(nthsyn*synperiod,nextSyn);
		break;
	}
	case DSP_SYN_REQUEST: {
		if(currStat==DSP_CONSIS_IDLE) {
		currStat = DSP_CONSIS_TRANSIENT;
		EV<<"Sending My Global Time: "<<simTime().dbl()<<" to "<<tempmsg->getSource()<<endl;
		InternetMsg *syn_msg = new InternetMsg();
		syn_msg->setSource(addr);
		syn_msg->setDestination(tempmsg->getSource());
		syn_msg->setTimestamp(simTime().dbl());
		syn_msg->setMsgType(DSP_SYN_REPLY);
		stringstream ss;
		string result;
		ss << simTime().dbl()-tempmsg->getTimestamp();
		ss >>result;
		syn_msg->setContent(result.c_str());
		send(syn_msg,"tointernet");}
		else {EV<<"I broker "<<addr<<" is synchronizing with other nodes, please wait"<<endl;}
		currStat=DSP_CONSIS_IDLE;
		break;
	}
	case DSP_SYN_TIMEOUT: {
		if(currStat==DSP_CONSIS_TRANSIENT) {
		InternetMsg *retry_msg = new InternetMsg();
		retry_msg->setSource(addr);
		retry_msg->setDestination(DSP_MASTER_BROKER);
		retry_msg->setTimestamp((simTime().dbl()-globaltimeaftersyn)*localclockdriftrate+agreedlocaltime);
		retry_msg->setMsgType(DSP_SYN_REQUEST);
		send(retry_msg,"tointernet");
		globaltimebeforesyn = simTime().dbl();
		localtimebeforesyn = (simTime().dbl()-globaltimeaftersyn)*localclockdriftrate+agreedlocaltime;
		EV<<"Syn TimingOut,Now Resending Syn Request with its local time "<<localtimebeforesyn<<" and global time "<<globaltimebeforesyn<<endl;
		scheduleAt(simTime()+par("syntimeout").doubleValue(),SynTimeout);
		}
		else {EV<<"Wrong Status";cancelAndDelete(msg);}
		break;
	}
	case DSP_SYN_REPLY: {
		if(currStat==DSP_CONSIS_TRANSIENT){
		EV<<"Receving Syn Message From Broker 0: "<<tempmsg->getTimestamp()<<endl;
		double di = atof(tempmsg->getContent())-tempmsg->getTimestamp()+((simTime().dbl()-globaltimeaftersyn)*localclockdriftrate+agreedlocaltime);
		EV<<"Di = "<<di<<endl;
		if(di<=1) {
		double oi = (atof(tempmsg->getContent())+tempmsg->getTimestamp()-((simTime().dbl()-globaltimeaftersyn)*localclockdriftrate+agreedlocaltime))/2;
		globaltimeaftersyn = simTime().dbl();
		EV<<"Receving syn reply with timestamp : "<<tempmsg->getTimestamp()<<"and previous agreed local time: "<<agreedlocaltime<<endl;
		agreedlocaltime = tempmsg->getTimestamp()-oi+di/2;
		EV<<"Global Time after syn: "<< globaltimeaftersyn <<endl;
		EV<<"Local Time after syn: "<< agreedlocaltime <<endl;
		}
		else {EV<<"Di is too big, I wanna resynchro next time"<<endl;}
		}
		else {EV<<"Already Receive Global Time ";cancelAndDelete(msg);}
		cancelEvent(SynTimeout);
		currStat = DSP_CONSIS_IDLE;
		sendallmsginqueues();
		sendlocalclocktoapp(globaltimeaftersyn,agreedlocaltime);
		break;
	}
	default: {  //I shall check how to set timestamp
		if(strcmp(tempmsg->getArrivalGate()->getName(),"fromapp") == NULL){
                tempmsg->setTimestamp((simTime().dbl()-globaltimeaftersyn)*localclockdriftrate+agreedlocaltime);
				if(currStat==DSP_CONSIS_IDLE) {send(tempmsg,"tointernet");}
				else {queuemsgtointernet(*tempmsg);}
				}
		else{
				if(currStat == DSP_CONSIS_IDLE) {
					if(tempmsg->getTimestamp()>=prevtoappmsgtimestamp) {send(tempmsg,"toapp");prevtoappmsgtimestamp = tempmsg->getTimestamp();}
					else {cancelAndDelete(tempmsg);EV<<"Violates Causal Order , Drop this msg"<<endl;}
					}
				else {queuemsgtoapp(*tempmsg);}
				}
		break;
	}
	}
}

void Consistency::synchronizing() {
	if(currStat==DSP_CONSIS_IDLE) {
    currStat = DSP_CONSIS_TRANSIENT;
	EV<<"Start Sync"<<endl;
	EV<<"Before Syn the local clock for node "<<addr<<" is "<<simTime()*localclockdriftrate<<" while global time is "<<simTime()<<endl;
	InternetMsg *syn_msg = new InternetMsg();
	syn_msg->setSource(addr);
	syn_msg->setDestination(DSP_MASTER_BROKER);
	globaltimebeforesyn = simTime().dbl();
	localtimebeforesyn = (simTime().dbl()-globaltimeaftersyn)*localclockdriftrate+agreedlocaltime;
	syn_msg->setTimestamp(localtimebeforesyn);
	syn_msg->setMsgType(DSP_SYN_REQUEST);
	send(syn_msg,"tointernet");
	scheduleAt(nthsyn*synperiod+par("syntimeout").doubleValue(),SynTimeout);
	}
	else {EV<<"ERROE DURING SYN,The previous Syn process is still not finished";}
}

void Consistency::sendallmsginqueues() {
  while (!tointernetmessageQueue.empty()) {
	  send(&(*tointernetmessageQueue.begin()),"tointernet");
	  tointernetmessageQueue.pop_front();
  }
  while (!toappmessageQueue.empty()) {
	  send(&(*toappmessageQueue.begin()),"toapp");
	  toappmessageQueue.pop_front();
  }
}

void Consistency::queuemsgtointernet(InternetMsg msg) {
     EV<<"During Syn, this msg will be queued"<<endl;
     insertMsg(msg,tointernetmessageQueue);
}

void Consistency::queuemsgtoapp(InternetMsg msg) {
	 EV<<"During Syn, this msg will be queued"<<endl;
	 insertMsg(msg,toappmessageQueue);
}

void Consistency::insertMsg(InternetMsg msg,std::list<InternetMsg> messageQueue){
	if(messageQueue.empty()) {messageQueue.push_back(msg);return;}
    list<InternetMsg>::iterator ite=messageQueue.begin();
    while(ite!=messageQueue.end()) {
    	if(ite->getTimestamp()>=msg.getTimestamp()) {messageQueue.insert(ite,msg);return;}
    	else ite++;}
}

void Consistency::sendlocalclocktoapp(double globaltime,double agreedlocaltime) {
	InternetMsg *toapp_msg = new InternetMsg();
    toapp_msg->setGlobaltime(globaltime);
    toapp_msg->setAgreedlocaltime(agreedlocaltime);
    toapp_msg->setClockdriftrate(localclockdriftrate);
    toapp_msg->setMsgType(CONSIS_APP_SYN);
	send(toapp_msg,"toapp");
}

