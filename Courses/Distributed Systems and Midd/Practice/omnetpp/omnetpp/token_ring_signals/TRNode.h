#ifndef TRNODE_H_
#define TRNODE_H_

#include <omnetpp.h>

class TRNode : public cSimpleModule {
private:
  long numSent;
  simtime_t startWaitingTime;
  simsignal_t timeWaitingSig;
  cPacket *token;
  cMessage *needResEvt;
  cMessage *endProcEvt;
public:
  TRNode();
  virtual ~TRNode();
protected:
  virtual void initialize();
  virtual void finish();
  virtual void handleMessage(cMessage *msg);
};

#endif /* TRNODE_H_ */
