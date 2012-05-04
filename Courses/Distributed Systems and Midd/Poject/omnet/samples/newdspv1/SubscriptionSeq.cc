/*
 * SubscriptionSeq.cc
 *
 *  Created on: Dec 13, 2011
 *      Author: xu
 */

#include "SubscriptionSeq.h"

SubscriptionSeq::SubscriptionSeq() {

}

SubscriptionSeq::SubscriptionSeq(double timestamp, long seqNum, std::string topic){
	this->seqNum = seqNum;
	this->timestamp = timestamp;
	this->topic = topic;
}

SubscriptionSeq::SubscriptionSeq(long seqNum){
	this->seqNum = seqNum;
	this->timestamp = 0;
}

long SubscriptionSeq::getSeqNum() const
{
    return seqNum;
}

double SubscriptionSeq::getTimestamp() const
{
    return timestamp;
}

void SubscriptionSeq::setSeqNum(long  seqNum)
{
    this->seqNum = seqNum;
}

std::string SubscriptionSeq::getTopic() const
{
    return topic;
}

void SubscriptionSeq::setTopic(std::string topic)
{
    this->topic = topic;
}

void SubscriptionSeq::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

SubscriptionSeq::~SubscriptionSeq() {

}

bool SubscriptionSeq::operator==(const SubscriptionSeq & a) const
{
	return (a.getSeqNum()==this->getSeqNum());
}



