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

#ifndef __DSP1_INTERNET_H_
#define __DSP1_INTERNET_H_

#include <omnetpp.h>
#include "internetMsg_m.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <iterator>
#include <list>
#include "TimestampedMsgRef.h"

/**
 * TODO - Generated class
 */
class Internet : public cSimpleModule
{
private:
	int nConnectedNodes;
	const char* confFileLocation;
	/**
	 * Matrixes of parameters that defines the link condition.
	 * index 0 = delay time distribution time
	 * index 1 = package loss ratio
	 * meanings of other index depends on the particular distribution
	 */
	double ***netConditionParam;
	int nParamArray;
	double generateRandomDelay(int sourceAddr, int destAddr);
	bool lostPacket(int sourceAddr, int destAddr);

	std::list<TimestampedMsgRef> messageQueue;
	std::list<TimestampedMsgRef>::iterator ite;
	InternetMsg *msg_forward_timeout;

	void insertMsg(TimestampedMsgRef t); //according to its timestamp+delay

  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);

};

#endif
