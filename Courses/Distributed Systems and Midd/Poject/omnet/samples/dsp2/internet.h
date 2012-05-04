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

#ifndef __DSP2_INTERNET_H_
#define __DSP2_INTERNET_H_

#include <omnetpp.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <iterator>

#include "selfMessage_m.h"
#include "internetPacket_m.h"
#include "TimstampedPktRef.h"

class Internet : public cSimpleModule
{
  private:

	//New
	simsignal_t delaySignal;

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

	std::priority_queue<TimstampedPktRef> packetQueue;
	SelfMessage *msgForwardingTimer;

  protected:
	virtual void finish();
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

  public:
    ~Internet();
};

enum Dsp_Random_Distribution_Type{
	DSP_OMNET_EXPONENTIAL_DISTRIBUTION = 1,
	DSP_OMNET_UNIFORM_DISTRIBUTION = 2,
	DSP_OMNET_NORMAL_DISTRIBUTION = 3,
	DSP_OMNET_TRUNCNORMAL_DISTRIBUTION = 4,
	DSP_OMNET_SHIFTED_PARETO_DISTRIBUTION = 5,

	DSP_UNKNOWN_DISTRIBUTION = 99

};

enum Dsp_Rng_Id{
	DSP_NET_PARAM_RNG_ID_OFFSET =50,
};
#endif
