/*
 * TimestampedId.h
 *
 *  Created on: Dec 11, 2011
 *      Author: xu
 */

#ifndef TIMESTAMPEDID_H_
#define TIMESTAMPEDID_H_
#include <iostream>
#include <omnetpp.h>

class TimestampedId {
  private:
    int client;
    double timestamp;

  public:
	TimestampedId();
	virtual ~TimestampedId();
	TimestampedId(int,double);
	//friend std::ostream& operator<< (std::ostream &out, TimestampedId &tr);
	bool operator<(const TimestampedId &an) const;
	bool operator==(const TimestampedId &an) const;
	int getClient() const;
	double getTimestamp() const;
};

#endif /* TIMESTAMPEDID_H_ */
