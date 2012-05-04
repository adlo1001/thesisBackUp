//
// Generated file, do not edit! Created by opp_msgc 4.1 from appPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "appPacket_m.h"

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
    cEnum *e = cEnum::find("DSP2_APPLICATION_MESSAGE_TYPES");
    if (!e) enums.getInstance()->add(e = new cEnum("DSP2_APPLICATION_MESSAGE_TYPES"));
    e->insert(DSP2_CLIENT_PUBLISH_MESSAGE, "DSP2_CLIENT_PUBLISH_MESSAGE");
    e->insert(DSP2_CLIENT_SUBSCRIBE_MESSAGE, "DSP2_CLIENT_SUBSCRIBE_MESSAGE");
    e->insert(DSP2_CLIENT_UNSUBSCRIBE_MESSAGE, "DSP2_CLIENT_UNSUBSCRIBE_MESSAGE");
    e->insert(DSP2_BROKER_CONNECT_MESSAGE, "DSP2_BROKER_CONNECT_MESSAGE");
    e->insert(DSP2_BROKER_NET_UPDATE_MESSAGE, "DSP2_BROKER_NET_UPDATE_MESSAGE");
    e->insert(DSP2_BROKER_NOT_READY_MESSAGE, "DSP2_BROKER_NOT_READY_MESSAGE");
    e->insert(DSP2_PUBLISHED_CONTENT_MESSAGE, "DSP2_PUBLISHED_CONTENT_MESSAGE");
);

Register_Class(AppPacket);

AppPacket::AppPacket(const char *name, int kind) : cPacket(name,kind)
{
    this->pktType_var = 0;
    this->srcAddress_var = 0;
    this->destAddress_var = 0;
    this->originator_var = 0;
    this->topic_var = 0;
    this->content_var = 0;
    this->topologyIdx_var = 0;
    connectedBrokers_arraysize = 0;
    this->connectedBrokers_var = 0;
}

AppPacket::AppPacket(const AppPacket& other) : cPacket()
{
    setName(other.getName());
    connectedBrokers_arraysize = 0;
    this->connectedBrokers_var = 0;
    operator=(other);
}

AppPacket::~AppPacket()
{
    delete [] connectedBrokers_var;
}

AppPacket& AppPacket::operator=(const AppPacket& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->pktType_var = other.pktType_var;
    this->srcAddress_var = other.srcAddress_var;
    this->destAddress_var = other.destAddress_var;
    this->originator_var = other.originator_var;
    this->topic_var = other.topic_var;
    this->content_var = other.content_var;
    this->topologyIdx_var = other.topologyIdx_var;
    delete [] this->connectedBrokers_var;
    this->connectedBrokers_var = (other.connectedBrokers_arraysize==0) ? NULL : new int[other.connectedBrokers_arraysize];
    connectedBrokers_arraysize = other.connectedBrokers_arraysize;
    for (unsigned int i=0; i<connectedBrokers_arraysize; i++)
        this->connectedBrokers_var[i] = other.connectedBrokers_var[i];
    return *this;
}

void AppPacket::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->pktType_var);
    doPacking(b,this->srcAddress_var);
    doPacking(b,this->destAddress_var);
    doPacking(b,this->originator_var);
    doPacking(b,this->topic_var);
    doPacking(b,this->content_var);
    doPacking(b,this->topologyIdx_var);
    b->pack(connectedBrokers_arraysize);
    doPacking(b,this->connectedBrokers_var,connectedBrokers_arraysize);
}

void AppPacket::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->pktType_var);
    doUnpacking(b,this->srcAddress_var);
    doUnpacking(b,this->destAddress_var);
    doUnpacking(b,this->originator_var);
    doUnpacking(b,this->topic_var);
    doUnpacking(b,this->content_var);
    doUnpacking(b,this->topologyIdx_var);
    delete [] this->connectedBrokers_var;
    b->unpack(connectedBrokers_arraysize);
    if (connectedBrokers_arraysize==0) {
        this->connectedBrokers_var = 0;
    } else {
        this->connectedBrokers_var = new int[connectedBrokers_arraysize];
        doUnpacking(b,this->connectedBrokers_var,connectedBrokers_arraysize);
    }
}

int AppPacket::getPktType() const
{
    return pktType_var;
}

void AppPacket::setPktType(int pktType_var)
{
    this->pktType_var = pktType_var;
}

int AppPacket::getSrcAddress() const
{
    return srcAddress_var;
}

void AppPacket::setSrcAddress(int srcAddress_var)
{
    this->srcAddress_var = srcAddress_var;
}

int AppPacket::getDestAddress() const
{
    return destAddress_var;
}

void AppPacket::setDestAddress(int destAddress_var)
{
    this->destAddress_var = destAddress_var;
}

int AppPacket::getOriginator() const
{
    return originator_var;
}

void AppPacket::setOriginator(int originator_var)
{
    this->originator_var = originator_var;
}

const char * AppPacket::getTopic() const
{
    return topic_var.c_str();
}

void AppPacket::setTopic(const char * topic_var)
{
    this->topic_var = topic_var;
}

const char * AppPacket::getContent() const
{
    return content_var.c_str();
}

void AppPacket::setContent(const char * content_var)
{
    this->content_var = content_var;
}

int AppPacket::getTopologyIdx() const
{
    return topologyIdx_var;
}

void AppPacket::setTopologyIdx(int topologyIdx_var)
{
    this->topologyIdx_var = topologyIdx_var;
}

void AppPacket::setConnectedBrokersArraySize(unsigned int size)
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

unsigned int AppPacket::getConnectedBrokersArraySize() const
{
    return connectedBrokers_arraysize;
}

int AppPacket::getConnectedBrokers(unsigned int k) const
{
    if (k>=connectedBrokers_arraysize) throw cRuntimeError("Array of size %d indexed by %d", connectedBrokers_arraysize, k);
    return connectedBrokers_var[k];
}

void AppPacket::setConnectedBrokers(unsigned int k, int connectedBrokers_var)
{
    if (k>=connectedBrokers_arraysize) throw cRuntimeError("Array of size %d indexed by %d", connectedBrokers_arraysize, k);
    this->connectedBrokers_var[k]=connectedBrokers_var;
}

class AppPacketDescriptor : public cClassDescriptor
{
  public:
    AppPacketDescriptor();
    virtual ~AppPacketDescriptor();

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

Register_ClassDescriptor(AppPacketDescriptor);

AppPacketDescriptor::AppPacketDescriptor() : cClassDescriptor("AppPacket", "cPacket")
{
}

AppPacketDescriptor::~AppPacketDescriptor()
{
}

bool AppPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<AppPacket *>(obj)!=NULL;
}

const char *AppPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int AppPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount(object) : 8;
}

unsigned int AppPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *AppPacketDescriptor::getFieldName(void *object, int field) const
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
        "originator",
        "topic",
        "content",
        "topologyIdx",
        "connectedBrokers",
    };
    return (field>=0 && field<8) ? fieldNames[field] : NULL;
}

int AppPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktType")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddress")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAddress")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "originator")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "topic")==0) return base+4;
    if (fieldName[0]=='c' && strcmp(fieldName, "content")==0) return base+5;
    if (fieldName[0]=='t' && strcmp(fieldName, "topologyIdx")==0) return base+6;
    if (fieldName[0]=='c' && strcmp(fieldName, "connectedBrokers")==0) return base+7;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *AppPacketDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
        "string",
        "string",
        "int",
        "int",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : NULL;
}

const char *AppPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "DSP2_APPLICATION_MESSAGE_TYPES";
            return NULL;
        default: return NULL;
    }
}

int AppPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    AppPacket *pp = (AppPacket *)object; (void)pp;
    switch (field) {
        case 7: return pp->getConnectedBrokersArraySize();
        default: return 0;
    }
}

std::string AppPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    AppPacket *pp = (AppPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPktType());
        case 1: return long2string(pp->getSrcAddress());
        case 2: return long2string(pp->getDestAddress());
        case 3: return long2string(pp->getOriginator());
        case 4: return oppstring2string(pp->getTopic());
        case 5: return oppstring2string(pp->getContent());
        case 6: return long2string(pp->getTopologyIdx());
        case 7: return long2string(pp->getConnectedBrokers(i));
        default: return "";
    }
}

bool AppPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    AppPacket *pp = (AppPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPktType(string2long(value)); return true;
        case 1: pp->setSrcAddress(string2long(value)); return true;
        case 2: pp->setDestAddress(string2long(value)); return true;
        case 3: pp->setOriginator(string2long(value)); return true;
        case 4: pp->setTopic((value)); return true;
        case 5: pp->setContent((value)); return true;
        case 6: pp->setTopologyIdx(string2long(value)); return true;
        case 7: pp->setConnectedBrokers(i,string2long(value)); return true;
        default: return false;
    }
}

const char *AppPacketDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<8) ? fieldStructNames[field] : NULL;
}

void *AppPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    AppPacket *pp = (AppPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


