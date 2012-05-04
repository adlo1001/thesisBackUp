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

#ifndef __DSP2_APPBROKER_H_
#define __DSP2_APPBROKER_H_

#include <omnetpp.h>
#include "selfMessage_m.h"
#include "appPacket_m.h"
#include "userCommand_m.h"
#include <vector>
#include <algorithm>
#include <string>
#include <map>

#define DSP2_MASTER_BROKER 0
#define WATCH_MMAP(m)         createStdMMapWatcher(#m,(m))

enum DSP2_BROKER_STATES {
    DSP2_BROKER_IDLE = 0,
    DSP2_BROKER_CONNECTED = 1,
};

class AppBroker : public cSimpleModule
{
  private:
	int myState;
	int myAddr;
	SelfMessage* powerOnTimer;
	int topologyIdx;
	std::vector<int> connectedBrokers;
	std::multimap<std::string, int> subscriptions;

	std::map<int,cDoubleHistogram> subLatency;
	std::map<int,cDoubleHistogram> unsubLatency;
  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);

  public:
    ~AppBroker();
};

//for multimap water
template<class KeyT, class ValueT, class CmpT>
class cStdMMapWatcher : public cStdVectorWatcherBase
{
  protected:
    std::multimap<KeyT,ValueT,CmpT>& m;
    mutable typename std::multimap<KeyT,ValueT,CmpT>::iterator it;
    mutable int itPos;
    std::string classname;
  public:
    cStdMMapWatcher(const char *name, std::multimap<KeyT,ValueT,CmpT>& var) : cStdVectorWatcherBase(name), m(var) {
        itPos=-1;
        classname = std::string("std::multimap<")+opp_typename(typeid(KeyT))+","+opp_typename(typeid(ValueT))+">";
    }
    const char *getClassName() const {return classname.c_str();}
    virtual const char *getElemTypeName() const {return "struct pair<*,*>";}
    virtual int size() const {return m.size();}
    virtual std::string at(int i) const {
        if (i==0) {
            it=m.begin(); itPos=0;
        } else if (i==itPos+1 && it!=m.end()) {
            ++it; ++itPos;
        } else {
            it=m.begin();
            for (int k=0; k<i && it!=m.end(); k++) ++it;
            itPos=i;
        }
        if (it==m.end()) {
            return std::string("out of bounds");
        }
        return atIt();
    }
    virtual std::string atIt() const {
        std::stringstream out;
        out << it->first << " ==> " << it->second;
        return out.str();
    }
};

template <class KeyT, class ValueT, class CmpT>
void createStdMMapWatcher(const char *varname, std::multimap<KeyT,ValueT,CmpT>& m)
{
    new cStdMMapWatcher<KeyT,ValueT,CmpT>(varname, m);
}

#endif
