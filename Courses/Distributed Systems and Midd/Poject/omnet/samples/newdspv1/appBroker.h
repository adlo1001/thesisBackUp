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

#ifndef __DSP1_APPBROKER_H_
#define __DSP1_APPBROKER_H_

#include <omnetpp.h>
#include "dsp_type_def.h"
#include <list>
#include "internetMsg_m.h"
#include <algorithm>
#include <iterator>
#include <string>
#include <map>
#include "TimestampedId.h"


class AppBroker : public cSimpleModule
{
  private:
	unsigned long netVersion;
	int currStat;
	int addr;
	std::list<TimestampedId> clients;
	std::list<TimestampedId> connectedBrokers;
	std::list<TimestampedId>::iterator it_clients;
	std::list<TimestampedId>::iterator it_brokers;
	std::multimap<std::string, TimestampedId> subscriptions;
	std::multimap<std::string,TimestampedId>::iterator it_subscriptions;
	std::pair<std::multimap<std::string,TimestampedId>::iterator,std::multimap<std::string,TimestampedId>::iterator> range_subscriptions;
	std::map<int,InternetMsg*> unconfirmedNetUpdate;
	std::map<int,InternetMsg*>::iterator it_unconfirmedNetUpdate;
	InternetMsg *nextActionScheduler;
	InternetMsg *joinTimeout;
	InternetMsg *netUpdateTimeout;

	double agreedlocaltime;
	double clockdriftrate;
	double globaltime;

	void joinActionGenerator();

protected:
  virtual void initialize();
  virtual void handleMessage(cMessage *msg);
};



#endif
