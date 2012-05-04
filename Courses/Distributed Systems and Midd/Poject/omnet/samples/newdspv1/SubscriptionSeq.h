/*
 * SubscriptionSeq.h
 *
 *  Created on: Dec 13, 2011
 *      Author: xu
 */

#ifndef SUBSCRIPTIONSEQ_H_
#define SUBSCRIPTIONSEQ_H_
#include <string>

class SubscriptionSeq
{
private:
    double timestamp;
    long seqNum;
    std::string topic;
public:
    SubscriptionSeq();
    SubscriptionSeq(double timestamp, long  seqNum, std::string topic);
    SubscriptionSeq(long  seqNum);
    virtual ~SubscriptionSeq();
    long getSeqNum() const;
    double getTimestamp() const;
    void setSeqNum(long  seqNum);
    void setTimestamp(double timestamp);
    std::string getTopic() const;
    void setTopic(std::string topic);

    bool operator==(const SubscriptionSeq& a) const;
};

#endif /* SUBSCRIPTIONSEQ_H_ */
