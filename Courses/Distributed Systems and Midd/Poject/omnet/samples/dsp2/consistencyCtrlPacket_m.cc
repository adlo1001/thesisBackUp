//
// Generated file, do not edit! Created by opp_msgc 4.1 from consistencyCtrlPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "consistencyCtrlPacket_m.h"

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




Register_Class(ConsistencyCtrlPacket);

ConsistencyCtrlPacket::ConsistencyCtrlPacket(const char *name, int kind) : cPacket(name,kind)
{
    this->enabled_var = 0;
    this->srcAddr_var = 0;
    this->destAddr_var = 0;
    vecTime_arraysize = 0;
    this->vecTime_var = 0;
}

ConsistencyCtrlPacket::ConsistencyCtrlPacket(const ConsistencyCtrlPacket& other) : cPacket()
{
    setName(other.getName());
    vecTime_arraysize = 0;
    this->vecTime_var = 0;
    operator=(other);
}

ConsistencyCtrlPacket::~ConsistencyCtrlPacket()
{
    delete [] vecTime_var;
}

ConsistencyCtrlPacket& ConsistencyCtrlPacket::operator=(const ConsistencyCtrlPacket& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->enabled_var = other.enabled_var;
    this->srcAddr_var = other.srcAddr_var;
    this->destAddr_var = other.destAddr_var;
    delete [] this->vecTime_var;
    this->vecTime_var = (other.vecTime_arraysize==0) ? NULL : new unsigned long[other.vecTime_arraysize];
    vecTime_arraysize = other.vecTime_arraysize;
    for (unsigned int i=0; i<vecTime_arraysize; i++)
        this->vecTime_var[i] = other.vecTime_var[i];
    return *this;
}

void ConsistencyCtrlPacket::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->enabled_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->destAddr_var);
    b->pack(vecTime_arraysize);
    doPacking(b,this->vecTime_var,vecTime_arraysize);
}

void ConsistencyCtrlPacket::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->enabled_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->destAddr_var);
    delete [] this->vecTime_var;
    b->unpack(vecTime_arraysize);
    if (vecTime_arraysize==0) {
        this->vecTime_var = 0;
    } else {
        this->vecTime_var = new unsigned long[vecTime_arraysize];
        doUnpacking(b,this->vecTime_var,vecTime_arraysize);
    }
}

bool ConsistencyCtrlPacket::getEnabled() const
{
    return enabled_var;
}

void ConsistencyCtrlPacket::setEnabled(bool enabled_var)
{
    this->enabled_var = enabled_var;
}

int ConsistencyCtrlPacket::getSrcAddr() const
{
    return srcAddr_var;
}

void ConsistencyCtrlPacket::setSrcAddr(int srcAddr_var)
{
    this->srcAddr_var = srcAddr_var;
}

int ConsistencyCtrlPacket::getDestAddr() const
{
    return destAddr_var;
}

void ConsistencyCtrlPacket::setDestAddr(int destAddr_var)
{
    this->destAddr_var = destAddr_var;
}

void ConsistencyCtrlPacket::setVecTimeArraySize(unsigned int size)
{
    unsigned long *vecTime_var2 = (size==0) ? NULL : new unsigned long[size];
    unsigned int sz = vecTime_arraysize < size ? vecTime_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        vecTime_var2[i] = this->vecTime_var[i];
    for (unsigned int i=sz; i<size; i++)
        vecTime_var2[i] = 0;
    vecTime_arraysize = size;
    delete [] this->vecTime_var;
    this->vecTime_var = vecTime_var2;
}

unsigned int ConsistencyCtrlPacket::getVecTimeArraySize() const
{
    return vecTime_arraysize;
}

unsigned long ConsistencyCtrlPacket::getVecTime(unsigned int k) const
{
    if (k>=vecTime_arraysize) throw cRuntimeError("Array of size %d indexed by %d", vecTime_arraysize, k);
    return vecTime_var[k];
}

void ConsistencyCtrlPacket::setVecTime(unsigned int k, unsigned long vecTime_var)
{
    if (k>=vecTime_arraysize) throw cRuntimeError("Array of size %d indexed by %d", vecTime_arraysize, k);
    this->vecTime_var[k]=vecTime_var;
}

class ConsistencyCtrlPacketDescriptor : public cClassDescriptor
{
  public:
    ConsistencyCtrlPacketDescriptor();
    virtual ~ConsistencyCtrlPacketDescriptor();

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

Register_ClassDescriptor(ConsistencyCtrlPacketDescriptor);

ConsistencyCtrlPacketDescriptor::ConsistencyCtrlPacketDescriptor() : cClassDescriptor("ConsistencyCtrlPacket", "cPacket")
{
}

ConsistencyCtrlPacketDescriptor::~ConsistencyCtrlPacketDescriptor()
{
}

bool ConsistencyCtrlPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ConsistencyCtrlPacket *>(obj)!=NULL;
}

const char *ConsistencyCtrlPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ConsistencyCtrlPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int ConsistencyCtrlPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *ConsistencyCtrlPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "enabled",
        "srcAddr",
        "destAddr",
        "vecTime",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int ConsistencyCtrlPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='e' && strcmp(fieldName, "enabled")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "destAddr")==0) return base+2;
    if (fieldName[0]=='v' && strcmp(fieldName, "vecTime")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ConsistencyCtrlPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "int",
        "int",
        "unsigned long",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *ConsistencyCtrlPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ConsistencyCtrlPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ConsistencyCtrlPacket *pp = (ConsistencyCtrlPacket *)object; (void)pp;
    switch (field) {
        case 3: return pp->getVecTimeArraySize();
        default: return 0;
    }
}

std::string ConsistencyCtrlPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ConsistencyCtrlPacket *pp = (ConsistencyCtrlPacket *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getEnabled());
        case 1: return long2string(pp->getSrcAddr());
        case 2: return long2string(pp->getDestAddr());
        case 3: return ulong2string(pp->getVecTime(i));
        default: return "";
    }
}

bool ConsistencyCtrlPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ConsistencyCtrlPacket *pp = (ConsistencyCtrlPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setEnabled(string2bool(value)); return true;
        case 1: pp->setSrcAddr(string2long(value)); return true;
        case 2: pp->setDestAddr(string2long(value)); return true;
        case 3: pp->setVecTime(i,string2ulong(value)); return true;
        default: return false;
    }
}

const char *ConsistencyCtrlPacketDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *ConsistencyCtrlPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ConsistencyCtrlPacket *pp = (ConsistencyCtrlPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


