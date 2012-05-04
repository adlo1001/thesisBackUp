#include "TRNode.h"

Define_Module(TRNode);

TRNode::TRNode() {
	token = NULL;
	needResEvt = NULL;
	endProcEvt = NULL;
}

TRNode::~TRNode() {
	delete token;
	cancelAndDelete(needResEvt);
	cancelAndDelete(endProcEvt);
}

void TRNode::initialize() {
	// Initialize variables.
	numSent = 0;
	WATCH(numSent);
	startWaitingTime = 0;
	WATCH(startWaitingTime);
	timeWaitingSig = registerSignal("TimeWaiting");
	needResEvt = new cMessage("needResEvt");
	endProcEvt = new cMessage("endProcEvt");

	// Schedule needResEvt
	scheduleAt(par("requestPeriod"), needResEvt);

	// Generate and send initial message.
	if (this->getIndex() == 0) {
		EV<< "Creating initial token and sending it\n";
		token = new cPacket("token");
		token->setByteLength(100);
		scheduleAt(0.0, token);
	}
}

void TRNode::handleMessage(cMessage *msg) {
	if (msg == needResEvt) {
		startWaitingTime = simTime();
	} else if (msg == endProcEvt) {
		EV<< "Finished using the resource, resending token\n";
		bubble("Releasing the resource");
		scheduleAt(simTime()+par("requestPeriod"),needResEvt);
		numSent++;
		send(token, "next");
		token = NULL;
	} else {
		EV << "Received token... ";
		token = (cPacket *) msg;
		if(startWaitingTime!=0) {
			EV << "accessing the resource after waiting for "
			<< SIMTIME_STR(simTime()-startWaitingTime) << endl;
			bubble("Accessing the resource");
			emit(timeWaitingSig,simTime()-startWaitingTime);
			startWaitingTime = 0;
			scheduleAt(simTime()+par("usePeriod"), endProcEvt);
		} else {
			EV << "resending token\n";
			numSent++;
			send(token, "next");
			token = NULL;
		}
	}
}

void TRNode::finish() {
	recordScalar("#sent", numSent);
}
