//
// Generated file, do not edit! Created by opp_msgc 4.1 from internetPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "internetPacket_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("DSP2_INTERNET_PACKET_TYPES");
    if (!e) enums.getInstance()->add(e = new cEnum("DSP2_INTERNET_PACKET_TYPES"));
    e->insert(DSP2_INTERNET_DATA_PACKET, "DSP2_INTERNET_DATA_PACKET");
    e->insert(DSP2_INTERNET_NACK_PACKET, "DSP2_INTERNET_NACK_PACKET");
);

Register_Class(InternetPacket);

InternetPacket::InternetPacket(const char *name, int kind) : cPacket(name,kind)
{
    this->pktType_var = 0;
    this->srcAddress_var = 0;
    this->destAddress_var = 0;
    this->seqNumber_var = 0;
    this->ackSeqNumber_var = 0;
}

InternetPacket::InternetPacket(const InternetPacket& other) : cPacket()
{
    setName(other.getName());
    operator=(other);
}

InternetPacket::~InternetPacket()
{
}

InternetPacket& InternetPacket::operator=(const InternetPacket& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->pktType_var = other.pktType_var;
    this->srcAddress_var = other.srcAddress_var;
    this->destAddress_var = other.destAddress_var;
    this->seqNumber_var = other.seqNumber_var;
    this->ackSeqNumber_var = other.ackSeqNumber_var;
    return *this;
}

void InternetPacket::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->pktType_var);
    doPacking(b,this->srcAddress_var);
    doPacking(b,this->destAddress_var);
    doPacking(b,this->seqNumber_var);
    doPacking(b,this->ackSeqNumber_var);
}

void InternetPacket::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->pktType_var);
    doUnpacking(b,this->srcAddress_var);
    doUnpacking(b,this->destAddress_var);
    doUnpacking(b,this->seqNumber_var);
    doUnpacking(b,this->ackSeqNumber_var);
}

int InternetPacket::getPktType() const
{
    return pktType_var;
}

void InternetPacket::setPktType(int pktType_var)
{
    this->pktType_var = pktType_var;
}

int InternetPacket::getSrcAddress() const
{
    return srcAddress_var;
}

void InternetPacket::setSrcAddress(int srcAddress_var)
{
    this->srcAddress_var = srcAddress_var;
}

int InternetPacket::getDestAddress() const
{
    return destAddress_var;
}

void InternetPacket::setDestAddress(int destAddress_var)
{
    this->destAddress_var = destAddress_var;
}

unsigned long InternetPacket::getSeqNumber() const
{
    return seqNumber_var;
}

void InternetPacket::setSeqNumber(unsigned long seqNumber_var)
{
    this->seqNumber_var = seqNumber_var;
}

unsigned long InternetPacket::getAckSeqNumber() const
{
    return ackSeqNumber_var;
}

void InternetPacket::setAckSeqNumber(unsigned long ackSeqNumber_var)
{
    this->ackSeqNumber_var = ackSeqNumber_var;
}

class InternetPacketDescriptor : public cClassDescriptor
{
  public:
    InternetPacketDescriptor();
    virtual ~InternetPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(InternetPacketDescriptor);

InternetPacketDescriptor::InternetPacketDescriptor() : cClassDescriptor("InternetPacket", "cPacket")
{
}

InternetPacketDescriptor::~InternetPacketDescriptor()
{
}

bool InternetPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<InternetPacket *>(obj)!=NULL;
}

const char *InternetPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int InternetPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int InternetPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *InternetPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "pktType",
        "srcAddress",
        "destAddress",
        "seqNumber",
        "ackSeqNumber",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int InternetPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktType")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddress")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAddress")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNumber")==0) return base+3;
    if (fieldName[0]=='a' && strcmp(fieldName, "ackSeqNumber")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *InternetPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "unsigned long",
        "unsigned long",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *InternetPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "DSP2_INTERNET_PACKET_TYPES";
            return NULL;
        default: return NULL;
    }
}

int InternetPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    InternetPacket *pp = (InternetPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string InternetPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    InternetPacket *pp = (InternetPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPktType());
        case 1: return long2string(pp->getSrcAddress());
        case 2: return long2string(pp->getDestAddress());
        case 3: return ulong2string(pp->getSeqNumber());
        case 4: return ulong2string(pp->getAckSeqNumber());
        default: return "";
    }
}

bool InternetPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    InternetPacket *pp = (InternetPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPktType(string2long(value)); return true;
        case 1: pp->setSrcAddress(string2long(value)); return true;
        case 2: pp->setDestAddress(string2long(value)); return true;
        case 3: pp->setSeqNumber(string2ulong(value)); return true;
        case 4: pp->setAckSeqNumber(string2ulong(value)); return true;
        default: return false;
    }
}

const char *InternetPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *InternetPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    InternetPacket *pp = (InternetPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


