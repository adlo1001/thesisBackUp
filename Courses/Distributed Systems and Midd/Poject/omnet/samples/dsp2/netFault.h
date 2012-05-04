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

#ifndef __DSP2_NETFAULT_H_
#define __DSP2_NETFAULT_H_

#include <omnetpp.h>
#include <map>
#include "selfMessage_m.h"
#include "internetPacket_m.h"
#include "consistencyCtrlPacket_m.h"

typedef std::map<unsigned long, cPacket *> CachedPacketsEntry;

std::ostream& operator<< (std::ostream &out, CachedPacketsEntry &tr);

class NetFault : public cSimpleModule
{
  private:
	std::map<int,unsigned long> rcvSeqNumber;
	std::map<int,unsigned long> sendSeqNumber;
	std::map<int, CachedPacketsEntry > rcvCachedPackets;
	std::map<int, CachedPacketsEntry > sendCachedPackets;
	int myAddr;

	unsigned long nDataPktSent;
	unsigned long nNackPktSent;
  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);

  public:
    ~NetFault();
};

#endif
