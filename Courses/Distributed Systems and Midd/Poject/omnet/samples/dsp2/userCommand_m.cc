//
// Generated file, do not edit! Created by opp_msgc 4.1 from userCommand.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "userCommand_m.h"

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
    cEnum *e = cEnum::find("DSP2_USER_COMMAND_TYPES");
    if (!e) enums.getInstance()->add(e = new cEnum("DSP2_USER_COMMAND_TYPES"));
    e->insert(DSP2_USER_SET_BROKER, "DSP2_USER_SET_BROKER");
    e->insert(DSP2_USER_SUBSCRIBE_COMMAND, "DSP2_USER_SUBSCRIBE_COMMAND");
    e->insert(DSP2_USER_UNSUBSCRIBE_COMMAND, "DSP2_USER_UNSUBSCRIBE_COMMAND");
    e->insert(DSP2_USER_PUBLISH_COMMAND, "DSP2_USER_PUBLISH_COMMAND");
);

Register_Class(UserCommand);

UserCommand::UserCommand(const char *name, int kind) : cMessage(name,kind)
{
    this->cmdType_var = 0;
    this->broker_var = 0;
    this->topic_var = 0;
    this->content_var = 0;
}

UserCommand::UserCommand(const UserCommand& other) : cMessage()
{
    setName(other.getName());
    operator=(other);
}

UserCommand::~UserCommand()
{
}

UserCommand& UserCommand::operator=(const UserCommand& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    this->cmdType_var = other.cmdType_var;
    this->broker_var = other.broker_var;
    this->topic_var = other.topic_var;
    this->content_var = other.content_var;
    return *this;
}

void UserCommand::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->cmdType_var);
    doPacking(b,this->broker_var);
    doPacking(b,this->topic_var);
    doPacking(b,this->content_var);
}

void UserCommand::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->cmdType_var);
    doUnpacking(b,this->broker_var);
    doUnpacking(b,this->topic_var);
    doUnpacking(b,this->content_var);
}

int UserCommand::getCmdType() const
{
    return cmdType_var;
}

void UserCommand::setCmdType(int cmdType_var)
{
    this->cmdType_var = cmdType_var;
}

int UserCommand::getBroker() const
{
    return broker_var;
}

void UserCommand::setBroker(int broker_var)
{
    this->broker_var = broker_var;
}

const char * UserCommand::getTopic() const
{
    return topic_var.c_str();
}

void UserCommand::setTopic(const char * topic_var)
{
    this->topic_var = topic_var;
}

const char * UserCommand::getContent() const
{
    return content_var.c_str();
}

void UserCommand::setContent(const char * content_var)
{
    this->content_var = content_var;
}

class UserCommandDescriptor : public cClassDescriptor
{
  public:
    UserCommandDescriptor();
    virtual ~UserCommandDescriptor();

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

Register_ClassDescriptor(UserCommandDescriptor);

UserCommandDescriptor::UserCommandDescriptor() : cClassDescriptor("UserCommand", "cMessage")
{
}

UserCommandDescriptor::~UserCommandDescriptor()
{
}

bool UserCommandDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<UserCommand *>(obj)!=NULL;
}

const char *UserCommandDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int UserCommandDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int UserCommandDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *UserCommandDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "cmdType",
        "broker",
        "topic",
        "content",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int UserCommandDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cmdType")==0) return base+0;
    if (fieldName[0]=='b' && strcmp(fieldName, "broker")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "topic")==0) return base+2;
    if (fieldName[0]=='c' && strcmp(fieldName, "content")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *UserCommandDescriptor::getFieldTypeString(void *object, int field) const
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

const char *UserCommandDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "DSP2_USER_COMMAND_TYPES";
            return NULL;
        default: return NULL;
    }
}

int UserCommandDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    UserCommand *pp = (UserCommand *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string UserCommandDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    UserCommand *pp = (UserCommand *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getCmdType());
        case 1: return long2string(pp->getBroker());
        case 2: return oppstring2string(pp->getTopic());
        case 3: return oppstring2string(pp->getContent());
        default: return "";
    }
}

bool UserCommandDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    UserCommand *pp = (UserCommand *)object; (void)pp;
    switch (field) {
        case 0: pp->setCmdType(string2long(value)); return true;
        case 1: pp->setBroker(string2long(value)); return true;
        case 2: pp->setTopic((value)); return true;
        case 3: pp->setContent((value)); return true;
        default: return false;
    }
}

const char *UserCommandDescriptor::getFieldStructName(void *object, int field) const
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

void *UserCommandDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    UserCommand *pp = (UserCommand *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


