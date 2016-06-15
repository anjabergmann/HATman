#include "S2CMessages.h"
#include "Trace.h"
#include "zlib.h"
#include <sstream>

arianne::MessageS2CActionACK::MessageS2CActionACK():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_ACTION_ACK;
  this->actionid=0;
  }

arianne::MessageS2CActionACK::~MessageS2CActionACK()
  {
  }

void arianne::MessageS2CActionACK::write(Serializer &s) const
  {
  Message::write(s);
  s << actionid;
  }
  
void arianne::MessageS2CActionACK::read(Serializer &s)
  {
  Message::read(s);
  s >> actionid;
  }

arianne::MessageS2CCharacterList::MessageS2CCharacterList():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_CHARACTERLIST;
  }

arianne::MessageS2CCharacterList::~MessageS2CCharacterList()
  {
  }

void arianne::MessageS2CCharacterList::write(Serializer &s) const
  {
  Message::write(s);  
  s << characters;
  }
  
void arianne::MessageS2CCharacterList::read(Serializer &s)
  {
  Message::read(s);
  s >> characters;
  }

arianne::MessageS2CChooseCharacterACK::MessageS2CChooseCharacterACK():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_CHOOSECHARACTER_ACK;
  }

arianne::MessageS2CChooseCharacterACK::~MessageS2CChooseCharacterACK()
  {
  }

void arianne::MessageS2CChooseCharacterACK::write(Serializer &s) const
  {
  Message::write(s);
  s << id;
  }
  
void arianne::MessageS2CChooseCharacterACK::read(Serializer &s)
  {
  Message::read(s);
  s >> id;
  }

arianne::MessageS2CChooseCharacterNACK::MessageS2CChooseCharacterNACK():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_CHOOSECHARACTER_NACK;
  }

arianne::MessageS2CChooseCharacterNACK::~MessageS2CChooseCharacterNACK()
  {
  }

void arianne::MessageS2CChooseCharacterNACK::write(Serializer &s) const
  {
  Message::write(s);
  }
  
void arianne::MessageS2CChooseCharacterNACK::read(Serializer &s)
  {
  Message::read(s);
  }

arianne::MessageS2CLoginACK::MessageS2CLoginACK():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_LOGIN_ACK;
  }

arianne::MessageS2CLoginACK::~MessageS2CLoginACK()
  {
  }

void arianne::MessageS2CLoginACK::write(Serializer &s) const
  {
  Message::write(s);
  }
  
void arianne::MessageS2CLoginACK::read(Serializer &s)
  {
  Message::read(s);
  }

arianne::MessageS2CLoginNACK::MessageS2CLoginNACK():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_LOGIN_NACK;
  }

arianne::MessageS2CLoginNACK::~MessageS2CLoginNACK()
  {
  }

void arianne::MessageS2CLoginNACK::write(Serializer &s) const
  {
  Message::write(s);
  s << reason;
  }
  
void arianne::MessageS2CLoginNACK::read(Serializer &s)
  {
  Message::read(s);
  s >> reason;
  }

std::string arianne::MessageS2CLoginNACK::toString() const
  {
  switch(reason)
    {
    case 0:
      return "Login rejected for unknown reason";
    case 1:
      return "Username/Password wrong";
    case 2:
      return "Server is full";
    default:
      throw IOException("Invalid protocol version");
    }
  }

arianne::MessageS2CLogoutACK::MessageS2CLogoutACK():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_LOGOUT_ACK;
  }

arianne::MessageS2CLogoutACK::~MessageS2CLogoutACK()
  {
  }

void arianne::MessageS2CLogoutACK::write(Serializer &s) const
  {
  Message::write(s);
  }
  
void arianne::MessageS2CLogoutACK::read(Serializer &s)
  {
  Message::read(s);
  }

arianne::MessageS2CLogoutNACK::MessageS2CLogoutNACK():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_LOGOUT_NACK;
  }

arianne::MessageS2CLogoutNACK::~MessageS2CLogoutNACK()
  {
  }

void arianne::MessageS2CLogoutNACK::write(Serializer &s) const
  {
  Message::write(s);
  }
  
void arianne::MessageS2CLogoutNACK::read(Serializer &s)
  {
  Message::read(s);
  }

arianne::MessageS2CPerception::MessageS2CPerception():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_PERCEPTION;
  }

arianne::MessageS2CPerception::~MessageS2CPerception()
  {
  }

void arianne::MessageS2CPerception::write(Serializer &s) const
  {
  /* TODO: Fix to support zlib
   * Look at Marauroa for implementation details */
  global_trace.add("MessageS2CPerception::write",">");
  global_trace.add("MessageS2CPerception::write","<");
  throw arianne::IOException("Not implemented");
  }
  
void arianne::MessageS2CPerception::read(Serializer &s)
  {
  global_trace.add("MessageS2CPerception::read",">");
  Message::read(s);

  Uint32 source_length;
  s >> source_length;
  Uint8 *source=new Uint8[source_length];
  std::ostringstream os;

  for(Uint32 i=0;i<source_length;++i)
    {
    s >> source[i];
    }
  
  uLong dest_length=64*1024;
  Uint8 *dest=new Uint8[dest_length];
  
  int retcode=uncompress(dest,&dest_length,source,source_length);
  if(retcode==Z_DATA_ERROR)
    {
    throw arianne::IOException("S2C Perception error: DATA ERROR");
    }
  if(retcode==Z_BUF_ERROR)
    {
    throw arianne::IOException("S2C Perception error: BUFFER ERROR");
    }
  if(retcode==Z_MEM_ERROR)
    {
    throw arianne::IOException("S2C Perception error: MEMORY ERROR");
    }
  
  std::vector<Uint8> uncarray(dest_length);
  for(uLong i=0;i<dest_length;++i)
    {
    uncarray[i]=dest[i];
    }
  
  delete[] dest;
  delete[] source;
    
  Serializer ser(uncarray);
  ser >> typePerception;
  ser >> addedObjects;
  ser >> modifiedAddedObjects;
  ser >> modifiedDeletedObjects;
  ser >> deletedObjects;
  
  std::vector<Uint8> dynamicPart;
  s >> dynamicPart;
  Serializer umcompressed_ser(dynamicPart);
  
  /** Dynamic part */
  umcompressed_ser >> timestamp;

  Uint8 modifiedmyRPObject;
  umcompressed_ser >> modifiedmyRPObject;
  if(modifiedmyRPObject==1)
    {
    umcompressed_ser >> myRPObject;
    }
  else
    {
    myRPObject=arianne::RPObject();
    }

  global_trace.add("MessageS2CPerception::read","<");
  }

arianne::MessageS2CServerInfo::MessageS2CServerInfo():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_SERVERINFO;
  }

arianne::MessageS2CServerInfo::~MessageS2CServerInfo()
  {
  }


void arianne::MessageS2CServerInfo::write(Serializer &s) const
  {
  /** TODO Implement */
  }
  
void arianne::MessageS2CServerInfo::read(Serializer &s)
  {
  Message::read(s);
  s >> contents;
  
  Uint32 sizeRPClasses;
  s >> sizeRPClasses;
  for(Uint32 i=0;i<sizeRPClasses;i++)
    {
    arianne::RPClass *rpclass=new RPClass();
    rpclass->read(s);
    
    RPClass::addRPClass(rpclass);
    }  
  }

arianne::MessageS2CInvalidMessage::MessageS2CInvalidMessage():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_INVALIDMESSAGE;
  }

arianne::MessageS2CInvalidMessage::~MessageS2CInvalidMessage()
  {
  }

void arianne::MessageS2CInvalidMessage::write(Serializer &s) const
  {
  Message::write(s);
  s << reason;
  }
  
void arianne::MessageS2CInvalidMessage::read(Serializer &s)
  {
  Message::read(s);
  s >> reason;
  }

arianne::MessageS2CRPMap::MessageS2CRPMap():
  Message(IPaddress())
  {
  this->type=TYPE_S2C_MAP;
  }

arianne::MessageS2CRPMap::~MessageS2CRPMap()
  {
  }

void arianne::MessageS2CRPMap::write(Serializer &s) const
  {
  /** TODO: It is only useful at server side */
  }
  
void arianne::MessageS2CRPMap::read(Serializer &s)
  {
  Message::read(s);

  Uint32 source_length;
  s >> source_length;
  Uint8 *source=new Uint8[source_length];
  std::ostringstream os;

  for(Uint32 i=0;i<source_length;++i)
    {
    s >> source[i];
    }
  
  uLong dest_length=64*1024;
  Uint8 *dest=new Uint8[dest_length];
  
  int retcode=uncompress(dest,&dest_length,source,source_length);
  if(retcode==Z_DATA_ERROR)
    {
    throw arianne::IOException("S2C Perception error: DATA ERROR");
    }
  if(retcode==Z_BUF_ERROR)
    {
    throw arianne::IOException("S2C Perception error: BUFFER ERROR");
    }
  if(retcode==Z_MEM_ERROR)
    {
    throw arianne::IOException("S2C Perception error: MEMORY ERROR");
    }
  
  std::vector<Uint8> uncarray(dest_length);
  for(uLong i=0;i<dest_length;++i)
    {
    uncarray[i]=dest[i];
    }
  
  delete[] dest;
  delete[] source;
    
  Serializer ser(uncarray);

  ser >> content;	
  }
