//
// Generated file, do not edit! Created by opp_msgc 4.1 from internetMsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "internetMsg_m.h"

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




Register_Class(InternetMsg);

InternetMsg::InternetMsg(const char *name, int kind) : cMessage(name,kind)
{
    this->source_var = 0;
    this->destination_var = 0;
    this->content_var = 0;
    this->msgType_var = 0;
    this->topic_var = 0;
    this->seqNumber_var = 0;
    this->timestamp_var = 0;
    connectedBrokers_arraysize = 0;
    this->connectedBrokers_var = 0;
    connectedBrokersTimeStamp_arraysize = 0;
    this->connectedBrokersTimeStamp_var = 0;
    this->globaltime_var = 0;
    this->agreedlocaltime_var = 0;
    this->clockdriftrate_var = 0;
}

InternetMsg::InternetMsg(const InternetMsg& other) : cMessage()
{
    setName(other.getName());
    connectedBrokers_arraysize = 0;
    this->connectedBrokers_var = 0;
    connectedBrokersTimeStamp_arraysize = 0;
    this->connectedBrokersTimeStamp_var = 0;
    operator=(other);
}

InternetMsg::~InternetMsg()
{
    delete [] connectedBrokers_var;
    delete [] connectedBrokersTimeStamp_var;
}

InternetMsg& InternetMsg::operator=(const InternetMsg& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    this->source_var = other.source_var;
    this->destination_var = other.destination_var;
    this->content_var = other.content_var;
    this->msgType_var = other.msgType_var;
    this->topic_var = other.topic_var;
    this->seqNumber_var = other.seqNumber_var;
    this->timestamp_var = other.timestamp_var;
    delete [] this->connectedBrokers_var;
    this->connectedBrokers_var = (other.connectedBrokers_arraysize==0) ? NULL : new int[other.connectedBrokers_arraysize];
    connectedBrokers_arraysize = other.connectedBrokers_arraysize;
    for (unsigned int i=0; i<connectedBrokers_arraysize; i++)
        this->connectedBrokers_var[i] = other.connectedBrokers_var[i];
    delete [] this->connectedBrokersTimeStamp_var;
    this->connectedBrokersTimeStamp_var = (other.connectedBrokersTimeStamp_arraysize==0) ? NULL : new double[other.connectedBrokersTimeStamp_arraysize];
    connectedBrokersTimeStamp_arraysize = other.connectedBrokersTimeStamp_arraysize;
    for (unsigned int i=0; i<connectedBrokersTimeStamp_arraysize; i++)
        this->connectedBrokersTimeStamp_var[i] = other.connectedBrokersTimeStamp_var[i];
    this->globaltime_var = other.globaltime_var;
    this->agreedlocaltime_var = other.agreedlocaltime_var;
    this->clockdriftrate_var = other.clockdriftrate_var;
    return *this;
}

void InternetMsg::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->source_var);
    doPacking(b,this->destination_var);
    doPacking(b,this->content_var);
    doPacking(b,this->msgType_var);
    doPacking(b,this->topic_var);
    doPacking(b,this->seqNumber_var);
    doPacking(b,this->timestamp_var);
    b->pack(connectedBrokers_arraysize);
    doPacking(b,this->connectedBrokers_var,connectedBrokers_arraysize);
    b->pack(connectedBrokersTimeStamp_arraysize);
    doPacking(b,this->connectedBrokersTimeStamp_var,connectedBrokersTimeStamp_arraysize);
    doPacking(b,this->globaltime_var);
    doPacking(b,this->agreedlocaltime_var);
    doPacking(b,this->clockdriftrate_var);
}

void InternetMsg::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->source_var);
    doUnpacking(b,this->destination_var);
    doUnpacking(b,this->content_var);
    doUnpacking(b,this->msgType_var);
    doUnpacking(b,this->topic_var);
    doUnpacking(b,this->seqNumber_var);
    doUnpacking(b,this->timestamp_var);
    delete [] this->connectedBrokers_var;
    b->unpack(connectedBrokers_arraysize);
    if (connectedBrokers_arraysize==0) {
        this->connectedBrokers_var = 0;
    } else {
        this->connectedBrokers_var = new int[connectedBrokers_arraysize];
        doUnpacking(b,this->connectedBrokers_var,connectedBrokers_arraysize);
    }
    delete [] this->connectedBrokersTimeStamp_var;
    b->unpack(connectedBrokersTimeStamp_arraysize);
    if (connectedBrokersTimeStamp_arraysize==0) {
        this->connectedBrokersTimeStamp_var = 0;
    } else {
        this->connectedBrokersTimeStamp_var = new double[connectedBrokersTimeStamp_arraysize];
        doUnpacking(b,this->connectedBrokersTimeStamp_var,connectedBrokersTimeStamp_arraysize);
    }
    doUnpacking(b,this->globaltime_var);
    doUnpacking(b,this->agreedlocaltime_var);
    doUnpacking(b,this->clockdriftrate_var);
}

int InternetMsg::getSource() const
{
    return source_var;
}

void InternetMsg::setSource(int source_var)
{
    this->source_var = source_var;
}

int InternetMsg::getDestination() const
{
    return destination_var;
}

void InternetMsg::setDestination(int destination_var)
{
    this->destination_var = destination_var;
}

const char * InternetMsg::getContent() const
{
    return content_var.c_str();
}

void InternetMsg::setContent(const char * content_var)
{
    this->content_var = content_var;
}

int InternetMsg::getMsgType() const
{
    return msgType_var;
}

void InternetMsg::setMsgType(int msgType_var)
{
    this->msgType_var = msgType_var;
}

const char * InternetMsg::getTopic() const
{
    return topic_var.c_str();
}

void InternetMsg::setTopic(const char * topic_var)
{
    this->topic_var = topic_var;
}

unsigned long InternetMsg::getSeqNumber() const
{
    return seqNumber_var;
}

void InternetMsg::setSeqNumber(unsigned long seqNumber_var)
{
    this->seqNumber_var = seqNumber_var;
}

double InternetMsg::getTimestamp() const
{
    return timestamp_var;
}

void InternetMsg::setTimestamp(double timestamp_var)
{
    this->timestamp_var = timestamp_var;
}

void InternetMsg::setConnectedBrokersArraySize(unsigned int size)
{
    int *connectedBrokers_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = connectedBrokers_arraysize < size ? connectedBrokers_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        connectedBrokers_var2[i] = this->connectedBrokers_var[i];
    for (unsigned int i=sz; i<size; i++)
        connectedBrokers_var2[i] = 0;
    connectedBrokers_arraysize = size;
    delete [] this->connectedBrokers_var;
    this->connectedBrokers_var = connectedBrokers_var2;
}

unsigned int InternetMsg::getConnectedBrokersArraySize() const
{
    return connectedBrokers_arraysize;
}

int InternetMsg::getConnectedBrokers(unsigned int k) const
{
    if (k>=connectedBrokers_arraysize) throw cRuntimeError("Array of size %d indexed by %d", connectedBrokers_arraysize, k);
    return connectedBrokers_var[k];
}

void InternetMsg::setConnectedBrokers(unsigned int k, int connectedBrokers_var)
{
    if (k>=connectedBrokers_arraysize) throw cRuntimeError("Array of size %d indexed by %d", connectedBrokers_arraysize, k);
    this->connectedBrokers_var[k]=connectedBrokers_var;
}

void InternetMsg::setConnectedBrokersTimeStampArraySize(unsigned int size)
{
    double *connectedBrokersTimeStamp_var2 = (size==0) ? NULL : new double[size];
    unsigned int sz = connectedBrokersTimeStamp_arraysize < size ? connectedBrokersTimeStamp_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        connectedBrokersTimeStamp_var2[i] = this->connectedBrokersTimeStamp_var[i];
    for (unsigned int i=sz; i<size; i++)
        connectedBrokersTimeStamp_var2[i] = 0;
    connectedBrokersTimeStamp_arraysize = size;
    delete [] this->connectedBrokersTimeStamp_var;
    this->connectedBrokersTimeStamp_var = connectedBrokersTimeStamp_var2;
}

unsigned int InternetMsg::getConnectedBrokersTimeStampArraySize() const
{
    return connectedBrokersTimeStamp_arraysize;
}

double InternetMsg::getConnectedBrokersTimeStamp(unsigned int k) const
{
    if (k>=connectedBrokersTimeStamp_arraysize) throw cRuntimeError("Array of size %d indexed by %d", connectedBrokersTimeStamp_arraysize, k);
    return connectedBrokersTimeStamp_var[k];
}

void InternetMsg::setConnectedBrokersTimeStamp(unsigned int k, double connectedBrokersTimeStamp_var)
{
    if (k>=connectedBrokersTimeStamp_arraysize) throw cRuntimeError("Array of size %d indexed by %d", connectedBrokersTimeStamp_arraysize, k);
    this->connectedBrokersTimeStamp_var[k]=connectedBrokersTimeStamp_var;
}

double InternetMsg::getGlobaltime() const
{
    return globaltime_var;
}

void InternetMsg::setGlobaltime(double globaltime_var)
{
    this->globaltime_var = globaltime_var;
}

double InternetMsg::getAgreedlocaltime() const
{
    return agreedlocaltime_var;
}

void InternetMsg::setAgreedlocaltime(double agreedlocaltime_var)
{
    this->agreedlocaltime_var = agreedlocaltime_var;
}

double InternetMsg::getClockdriftrate() const
{
    return clockdriftrate_var;
}

void InternetMsg::setClockdriftrate(double clockdriftrate_var)
{
    this->clockdriftrate_var = clockdriftrate_var;
}

class InternetMsgDescriptor : public cClassDescriptor
{
  public:
    InternetMsgDescriptor();
    virtual ~InternetMsgDescriptor();

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

Register_ClassDescriptor(InternetMsgDescriptor);

InternetMsgDescriptor::InternetMsgDescriptor() : cClassDescriptor("InternetMsg", "cMessage")
{
}

InternetMsgDescriptor::~InternetMsgDescriptor()
{
}

bool InternetMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<InternetMsg *>(obj)!=NULL;
}

const char *InternetMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int InternetMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount(object) : 12;
}

unsigned int InternetMsgDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
}

const char *InternetMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "source",
        "destination",
        "content",
        "msgType",
        "topic",
        "seqNumber",
        "timestamp",
        "connectedBrokers",
        "connectedBrokersTimeStamp",
        "globaltime",
        "agreedlocaltime",
        "clockdriftrate",
    };
    return (field>=0 && field<12) ? fieldNames[field] : NULL;
}

int InternetMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "source")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destination")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "content")==0) return base+2;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgType")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "topic")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqNumber")==0) return base+5;
    if (fieldName[0]=='t' && strcmp(fieldName, "timestamp")==0) return base+6;
    if (fieldName[0]=='c' && strcmp(fieldName, "connectedBrokers")==0) return base+7;
    if (fieldName[0]=='c' && strcmp(fieldName, "connectedBrokersTimeStamp")==0) return base+8;
    if (fieldName[0]=='g' && strcmp(fieldName, "globaltime")==0) return base+9;
    if (fieldName[0]=='a' && strcmp(fieldName, "agreedlocaltime")==0) return base+10;
    if (fieldName[0]=='c' && strcmp(fieldName, "clockdriftrate")==0) return base+11;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *InternetMsgDescriptor::getFieldTypeString(void *object, int field) const
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
        "string",
        "int",
        "string",
        "unsigned long",
        "double",
        "int",
        "double",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : NULL;
}

const char *InternetMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int InternetMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    InternetMsg *pp = (InternetMsg *)object; (void)pp;
    switch (field) {
        case 7: return pp->getConnectedBrokersArraySize();
        case 8: return pp->getConnectedBrokersTimeStampArraySize();
        default: return 0;
    }
}

std::string InternetMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    InternetMsg *pp = (InternetMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSource());
        case 1: return long2string(pp->getDestination());
        case 2: return oppstring2string(pp->getContent());
        case 3: return long2string(pp->getMsgType());
        case 4: return oppstring2string(pp->getTopic());
        case 5: return ulong2string(pp->getSeqNumber());
        case 6: return double2string(pp->getTimestamp());
        case 7: return long2string(pp->getConnectedBrokers(i));
        case 8: return double2string(pp->getConnectedBrokersTimeStamp(i));
        case 9: return double2string(pp->getGlobaltime());
        case 10: return double2string(pp->getAgreedlocaltime());
        case 11: return double2string(pp->getClockdriftrate());
        default: return "";
    }
}

bool InternetMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    InternetMsg *pp = (InternetMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSource(string2long(value)); return true;
        case 1: pp->setDestination(string2long(value)); return true;
        case 2: pp->setContent((value)); return true;
        case 3: pp->setMsgType(string2long(value)); return true;
        case 4: pp->setTopic((value)); return true;
        case 5: pp->setSeqNumber(string2ulong(value)); return true;
        case 6: pp->setTimestamp(string2double(value)); return true;
        case 7: pp->setConnectedBrokers(i,string2long(value)); return true;
        case 8: pp->setConnectedBrokersTimeStamp(i,string2double(value)); return true;
        case 9: pp->setGlobaltime(string2double(value)); return true;
        case 10: pp->setAgreedlocaltime(string2double(value)); return true;
        case 11: pp->setClockdriftrate(string2double(value)); return true;
        default: return false;
    }
}

const char *InternetMsgDescriptor::getFieldStructName(void *object, int field) const
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
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<12) ? fieldStructNames[field] : NULL;
}

void *InternetMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    InternetMsg *pp = (InternetMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


