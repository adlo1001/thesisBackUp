
#include "TimestampedId.h"

TimestampedId::TimestampedId() {

}

TimestampedId::~TimestampedId() {
}

TimestampedId::TimestampedId(int client,double timestamp)
{
    this->client = client;
    this->timestamp = timestamp;
}
/*
std::ostream& operator<<(std::ostream &out, TimestampedId &tr){
    out << "(Client address: " << tr.client << " TimeStamp: " << tr.timestamp << " )" ;
    return out;
}
*/
int TimestampedId::getClient() const{
	return this->client;
}

bool TimestampedId::operator < (const TimestampedId & an) const
{
	return (this->client < an.getClient());
	/*
	if (this->client < an.getClient()){
		return true;
	} else if (this->client > an.getClient()){
		return false;
	}
	return (this->timestamp < an.getTimestamp());
	*/
}

bool TimestampedId::operator ==(const TimestampedId & an) const
{
	if(this->client==an.getClient()){
		return(this->timestamp >= an.getTimestamp());
	}
	return false;
}

double TimestampedId::getTimestamp() const{
	return this->timestamp;
}

