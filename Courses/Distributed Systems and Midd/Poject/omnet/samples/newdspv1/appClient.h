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

#ifndef __DSP1_APPCLIENT_H_
#define __DSP1_APPCLIENT_H_

#include <omnetpp.h>
#include "internetMsg_m.h"
#include <string>
#include <list>
#include "SubscriptionSeq.h"

/**
 * TODO - Generated class
 */
class AppClient : public cSimpleModule
{
	private:
	long subSeqNum;
	std::list<SubscriptionSeq> notConfirmedSub;
	std::list<SubscriptionSeq>::iterator it_notConfirmedSub;
	int currBroker;
	std::string *topics;
	int currStat;
	int addr;
	InternetMsg *joinTimeout;
	InternetMsg *leaveTimeout;
	InternetMsg *subscribeTimeout;
	InternetMsg *nextActionScheduler;

	double agreedlocaltime;
	double clockdriftrate;
	double globaltime;

	void randomActionGenerator();
protected:
  virtual void initialize();
  virtual void handleMessage(cMessage *msg);
};

#endif
