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

#ifndef __NEWDSPV1_CONSISTENCY_H_
#define __NEWDSPV1_CONSISTENCY_H_

#include <omnetpp.h>
#include "internetMsg_m.h"

/**
 * TODO - Generated class
 */
class Consistency : public cSimpleModule
{
  private:
	int addr;

	InternetMsg *SynTimeout;
	InternetMsg *nextSyn;
	double synperiod;
	double toleratedClockDiff ;
	double localclockdriftrate ;
	std::list<InternetMsg> tointernetmessageQueue;
	std::list<InternetMsg> toappmessageQueue;
	int currStat;
	double globaltimeaftersyn;
	double agreedlocaltime;
	double localtimebeforesyn;
	double globaltimebeforesyn;
	int nthsyn;
	double prevtoappmsgtimestamp;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void synchronizing();
    virtual void sendallmsginqueues();
    virtual void queuemsgtointernet(InternetMsg msg);
    virtual void queuemsgtoapp(InternetMsg msg);
    virtual void insertMsg(InternetMsg msg,std::list<InternetMsg> messageQueue);
    virtual void sendlocalclocktoapp(double globaltime,double agreedlocaltime);
};

#endif
