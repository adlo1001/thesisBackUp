/*
 * TimestampedMsgRef.cc
 *
 *  Created on: Dec 17, 2011
 *      Author: xu
 */

#include "TimestampedMsgRef.h"

TimestampedMsgRef::TimestampedMsgRef() {

}

cMessage *TimestampedMsgRef::getMsg() const
{
    return msg;
}

TimestampedMsgRef::TimestampedMsgRef(double ts, cMessage *ms)
{
	this->msg = ms;
	this->timestamp = ts;
}

double TimestampedMsgRef::getTimestamp() const
{
    return timestamp;
}

TimestampedMsgRef::~TimestampedMsgRef() {

}

bool TimestampedMsgRef::operator <(const TimestampedMsgRef & a) const
{
	return (this->timestamp > a.timestamp);
}



