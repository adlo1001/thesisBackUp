//
// Generated file, do not edit! Created by opp_msgc 4.1 from consistencyCtrlPacket.msg.
//

#ifndef _CONSISTENCYCTRLPACKET_M_H_
#define _CONSISTENCYCTRLPACKET_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0401
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>consistencyCtrlPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet ConsistencyCtrlPacket {
 *     bool enabled;
 *     int srcAddr;
 *     int destAddr;
 * 	unsigned long vecTime[];
 * }
 * </pre>
 */
class ConsistencyCtrlPacket : public ::cPacket
{
  protected:
    bool enabled_var;
    int srcAddr_var;
    int destAddr_var;
    unsigned long *vecTime_var; // array ptr
    unsigned int vecTime_arraysize;

    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ConsistencyCtrlPacket&);

  public:
    ConsistencyCtrlPacket(const char *name=NULL, int kind=0);
    ConsistencyCtrlPacket(const ConsistencyCtrlPacket& other);
    virtual ~ConsistencyCtrlPacket();
    ConsistencyCtrlPacket& operator=(const ConsistencyCtrlPacket& other);
    virtual ConsistencyCtrlPacket *dup() const {return new ConsistencyCtrlPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual bool getEnabled() const;
    virtual void setEnabled(bool enabled_var);
    virtual int getSrcAddr() const;
    virtual void setSrcAddr(int srcAddr_var);
    virtual int getDestAddr() const;
    virtual void setDestAddr(int destAddr_var);
    virtual void setVecTimeArraySize(unsigned int size);
    virtual unsigned int getVecTimeArraySize() const;
    virtual unsigned long getVecTime(unsigned int k) const;
    virtual void setVecTime(unsigned int k, unsigned long vecTime_var);
};

inline void doPacking(cCommBuffer *b, ConsistencyCtrlPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, ConsistencyCtrlPacket& obj) {obj.parsimUnpack(b);}


#endif // _CONSISTENCYCTRLPACKET_M_H_
