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

#ifndef __DSP2_USER_H_
#define __DSP2_USER_H_

#include <omnetpp.h>
#include "selfMessage_m.h"
#include "userCommand_m.h"
#include "sysReply_m.h"
#include <string>

enum DSP2_USER_ACTIONS{
	DSP2_CLIENT_PUBLISH_OPERATION = 0,
	DSP2_CLIENT_SUBSCRIBE_OPERATION = 1,
	DSP2_CLIENT_UNSUBSCRIBE_OPERATION = 2,
};

class User : public cSimpleModule
{
  private:
	//New
	simsignal_t arrivalSignal;

	int nPerformedAction;
	int nTotalAction;
	SelfMessage *startTimer;
	SelfMessage *nextActionTimer;
	std::string *topics;

	void doRandomAction();

  protected:
	virtual void finish();
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

  public:
    ~User();

};

#endif
