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

#include "User.h"

Define_Module(User);

void User::initialize()
{
	//New
	arrivalSignal = registerSignal("arrival");

	EV << "Start User Module Initialization: " << endl;

	startTimer = new SelfMessage();
	startTimer->setType(DSP2_USER_START_TIMER);
	nextActionTimer = new SelfMessage();
	nextActionTimer->setType(DSP2_USER_NEXT_ACTION_TIMER);

	nPerformedAction = 0;
	nTotalAction = (int) par("totalActionCount");
	EV << "This user will perform " << nTotalAction << " actions" << endl;

	scheduleAt(simTime().dbl() + par("startTime").doubleValue(),startTimer);
	EV << "Action Timer will be activated at " << simTime().dbl() + par("startTime").doubleValue() << "s." << endl;

	//initialize the topics
	topics = new std::string[4];
	topics[0] = "Mary";
	topics[1] = "John";
	topics[2] = "Bill";
	topics[3] = "Smith";

	EV << "End User Module Initialization. " << endl << endl;
}

void User::finish()
{

}

void User::handleMessage(cMessage *msg)
{
	EV << "Start User Module Event. " << endl;

	if (msg->isSelfMessage()){
		SelfMessage* sMsg = check_and_cast<SelfMessage *>(msg);
		switch (sMsg->getType()){
			case DSP2_USER_NEXT_ACTION_TIMER:{
				EV << "Next Action Timer Fired, perform an action." << endl;
				doRandomAction();
				nPerformedAction++;
				if(nPerformedAction < nTotalAction ){
					double nextActionTime;
					nextActionTime = simTime().dbl() + par("actionInterval").doubleValue();
					scheduleAt(nextActionTime,nextActionTimer);
					EV << "Next action will be performed at " << nextActionTime << "s." << endl;
				} else {
					EV << "This client has finished all his actions." << endl;
				}
				break;
			}
			case DSP2_USER_START_TIMER:{
				EV << "Start Timer Fired, set the next action timer." << endl;
				double nextActionTime;
				nextActionTime = simTime().dbl() + par("actionInterval").doubleValue();
				scheduleAt(nextActionTime,nextActionTimer);
				EV << "Next action will be performed at " << nextActionTime << "s." << endl;
				UserCommand *uCmd = new UserCommand();
				uCmd->setBroker((int)par("brokerAddr"));
				uCmd->setCmdType(DSP2_USER_SET_BROKER);
				send(uCmd,"application$o");
				EV << "Informing application module to send request to broker " << (int)par("brokerAddr") << endl;
				break;
			}
			default:{
				error("User Module: Unknown Self-message type");
				break;
			}
		}
	} else {
		SysReply* sRep = check_and_cast<SysReply *>(msg);
		switch (sRep->getRepType()){
			case DSP2_PUBLISHED_MESSAGE:{
				EV << "!! PUBLISHED MESSAGE FROM SYSTEM !!" << endl;
				EV << " ORIGINATOR ADDRESS: " << sRep->getOriginator() << endl;
				EV << " TOPIC: " << sRep->getTopic() << endl;
				EV << " CONTENT: " << sRep->getContent() << endl;
				cancelAndDelete(msg);
				break;
			}
			case DSP2_BROKER_NOT_READY:{
				EV << "Received broker not ready message, reset client state." << endl;
				cancelEvent(startTimer);
				cancelEvent(nextActionTimer);
				nPerformedAction = 0;
				scheduleAt(simTime().dbl() + par("startTime").doubleValue(),startTimer);
				EV << "This client will restart at " << simTime().dbl() + par("startTime").doubleValue() << "s." << endl;
				cancelAndDelete(msg);
				break;
			}
			default:{
				error("User Module: Received Unknown Message");
				break;
			}
		}
	}
	EV << "End User Module Event. " << endl << endl;
}

void User::doRandomAction()
{
	//New
	int requestType = 0;

	int dice = intrand(2000);
	int choice;
	if (dice>=0 && dice<=1799){
		choice = DSP2_CLIENT_PUBLISH_OPERATION;

		//New send a signal
		requestType = 1;
		emit(arrivalSignal, requestType);

	} else if (dice>=1800 && dice<=1899){
		choice = DSP2_CLIENT_SUBSCRIBE_OPERATION;

		//New send a signal
		requestType = 2;
		emit(arrivalSignal, requestType);

	} else {
		choice = DSP2_CLIENT_UNSUBSCRIBE_OPERATION;

		//New send a signal
		requestType = 3;
		emit(arrivalSignal, requestType);
	}

	int actionTopicIdx = intrand(topics->size());

	switch (choice){
		case DSP2_CLIENT_PUBLISH_OPERATION:{
			std::stringstream cntss;
			std::string content;
			cntss << "Action number: " << nPerformedAction + 1;
			content = cntss.str();

			EV << "Publishing a message for topic \"" << topics[actionTopicIdx] << "\", content is \"" << content << "\"" << endl;
			UserCommand *uCmd = new UserCommand();
			uCmd->setCmdType(DSP2_USER_PUBLISH_COMMAND);
			uCmd->setTopic(topics[actionTopicIdx].c_str());
			uCmd->setContent(content.c_str());
			send(uCmd,"application$o");
			break;
		}
		case DSP2_CLIENT_SUBSCRIBE_OPERATION:{

			EV << "Subscribe for topic \"" << topics[actionTopicIdx] << "\"." << endl;
			UserCommand *uCmd = new UserCommand();
			uCmd->setCmdType(DSP2_USER_SUBSCRIBE_COMMAND);
			uCmd->setTopic(topics[actionTopicIdx].c_str());
			send(uCmd,"application$o");
			break;
		}
		case DSP2_CLIENT_UNSUBSCRIBE_OPERATION:{

			EV << "Un-Subscribe for topic \"" << topics[actionTopicIdx] << "\"." << endl;
			UserCommand *uCmd = new UserCommand();
			uCmd->setCmdType(DSP2_USER_UNSUBSCRIBE_COMMAND);
			uCmd->setTopic(topics[actionTopicIdx].c_str());
			send(uCmd,"application$o");
			break;
		}
		default:{
			error("User Module: Error in action Selection");
			break;
		}
	}
}

User::~User()
{
	cancelAndDelete(startTimer);
	cancelAndDelete(nextActionTimer);
	delete[](topics);
}
