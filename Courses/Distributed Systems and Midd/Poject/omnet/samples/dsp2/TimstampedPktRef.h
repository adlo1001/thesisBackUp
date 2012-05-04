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

#ifndef TIMSTAMPEDPKTREF_H_
#define TIMSTAMPEDPKTREF_H_

#include <omnetpp.h>

class TimstampedPktRef {
  private:
	double timestamp;
	cPacket *pkt;
  public:
	TimstampedPktRef();
	TimstampedPktRef(double ts,cPacket *ms);
	virtual ~TimstampedPktRef();
	cPacket *getPkt() const;
	double getTimestamp() const;

	bool operator<(const TimstampedPktRef& a) const;
	friend std::ostream& operator<< (std::ostream &out, const TimstampedPktRef &tr);
};

#endif /* TIMSTAMPEDPKTREF_H_ */
