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

#include "TimstampedPktRef.h"

TimstampedPktRef::TimstampedPktRef() {

}

TimstampedPktRef::~TimstampedPktRef() {

}

TimstampedPktRef::TimstampedPktRef(double ts, cPacket *ms)
{
	this->pkt = ms;
	this->timestamp = ts;
}

cPacket *TimstampedPktRef::getPkt() const
{
	return pkt;
}



double TimstampedPktRef::getTimestamp() const
{
	return timestamp;
}



bool TimstampedPktRef::operator <(const TimstampedPktRef & a) const
{
	return (this->timestamp > a.timestamp);
}

std::ostream & operator <<(std::ostream & out, const TimstampedPktRef & tr)
{
	const TimstampedPktRef * trp = &tr;
	if(trp!=NULL){
		out << "Time: " << tr.timestamp << " Packet: " << *(tr.pkt) << " " << (void *)(tr.pkt);
	} else {
		out << "n.a.";
	}
	return out;
}





