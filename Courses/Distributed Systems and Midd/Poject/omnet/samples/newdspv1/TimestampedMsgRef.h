/*
 * TimestampedMsgRef.h
 *
 *  Created on: Dec 17, 2011
 *      Author: xu
 */


#ifndef TIMESTAMPEDMSGREF_H_
#define TIMESTAMPEDMSGREF_H_

#include <omnetpp.h>

class TimestampedMsgRef {
  private:
    double timestamp;
    cMessage *msg;
public:
    TimestampedMsgRef();
    TimestampedMsgRef(double ts,cMessage *ms);
    virtual ~TimestampedMsgRef();
    cMessage *getMsg() const;
    double getTimestamp() const;

    bool operator<(const TimestampedMsgRef& a) const;
};

#endif /* TIMESTAMPEDMSGREF_H_ */
