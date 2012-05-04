//
// Generated file, do not edit! Created by opp_msgc 4.1 from sysReply.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "sysReply_m.h"

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
    cEnum *e = cEnum::find("DSP2_SYSTEM_REPLY_TYPES");
    if (!e) enums.getInstance()->add(e = new cEnum("DSP2_SYSTEM_REPLY_TYPES"));
    e->insert(DSP2_BROKER_NOT_READY, "DSP2_BROKER_NOT_READY");
    e->insert(DSP2_PUBLISHED_MESSAGE, "DSP2_PUBLISHED_MESSAGE");
);

Register_Class(SysReply);

SysReply::SysReply(const char *name, int kind) : cMessage(name,kind)
{
    this->repType_var = 0;
    this->originator_var = 0;
    this->topic_var = 0;
    this->content_var = 0;
}

SysReply::SysReply(const SysReply& other) : cMessage()
{
    setName(other.getName());
    operator=(other);
}

SysReply::~SysReply()
{
}

SysReply& SysReply::operator=(const SysReply& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    this->repType_var = other.repType_var;
    this->originator_var = other.originator_var;
    this->topic_var = other.topic_var;
    this->content_var = other.content_var;
    return *this;
}

void SysReply::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->repType_var);
    doPacking(b,this->originator_var);
    doPacking(b,this->topic_var);
    doPacking(b,this->content_var);
}

void SysReply::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->repType_var);
    doUnpacking(b,this->originator_var);
    doUnpacking(b,this->topic_var);
    doUnpacking(b,this->content_var);
}

int SysReply::getRepType() const
{
    return repType_var;
}

void SysReply::setRepType(int repType_var)
{
    this->repType_var = repType_var;
}

int SysReply::getOriginator() const
{
    return originator_var;
}

void SysReply::setOriginator(int originator_var)
{
    this->originator_var = originator_var;
}

const char * SysReply::getTopic() const
{
    return topic_var.c_str();
}

void SysReply::setTopic(const char * topic_var)
{
    this->topic_var = topic_var;
}

const char * SysReply::getContent() const
{
    return content_var.c_str();
}

void SysReply::setContent(const char * content_var)
{
    this->content_var = content_var;
}

class SysReplyDescriptor : public cClassDescriptor
{
  public:
    SysReplyDescriptor();
    virtual ~SysReplyDescriptor();

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

Register_ClassDescriptor(SysReplyDescriptor);

SysReplyDescriptor::SysReplyDescriptor() : cClassDescriptor("SysReply", "cMessage")
{
}

SysReplyDescriptor::~SysReplyDescriptor()
{
}

bool SysReplyDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SysReply *>(obj)!=NULL;
}

const char *SysReplyDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SysReplyDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int SysReplyDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *SysReplyDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "repType",
        "originator",
        "topic",
        "content",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int SysReplyDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "repType")==0) return base+0;
    if (fieldName[0]=='o' && strcmp(fieldName, "originator")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "topic")==0) return base+2;
    if (fieldName[0]=='c' && strcmp(fieldName, "content")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SysReplyDescriptor::getFieldTypeString(void *object, int field) const
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
        "string",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *SysReplyDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "DSP2_SYSTEM_REPLY_TYPES";
            return NULL;
        default: return NULL;
    }
}

int SysReplyDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SysReply *pp = (SysReply *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SysReplyDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SysReply *pp = (SysReply *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getRepType());
        case 1: return long2string(pp->getOriginator());
        case 2: return oppstring2string(pp->getTopic());
        case 3: return oppstring2string(pp->getContent());
        default: return "";
    }
}

bool SysReplyDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SysReply *pp = (SysReply *)object; (void)pp;
    switch (field) {
        case 0: pp->setRepType(string2long(value)); return true;
        case 1: pp->setOriginator(string2long(value)); return true;
        case 2: pp->setTopic((value)); return true;
        case 3: pp->setContent((value)); return true;
        default: return false;
    }
}

const char *SysReplyDescriptor::getFieldStructName(void *object, int field) const
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

void *SysReplyDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SysReply *pp = (SysReply *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


