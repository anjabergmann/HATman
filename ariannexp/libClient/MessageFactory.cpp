#include "MessageFactory.h"
#include "C2SMessages.h"
#include "S2CMessages.h"

#include <sstream>
#include <iostream>

using namespace arianne;

arianne::InvalidVersionException::InvalidVersionException(std::string const& ver): std::exception()
  {
  version=ver;
  }
arianne::InvalidVersionException::~InvalidVersionException() throw ()
  {
  }
const char * arianne::InvalidVersionException::what() const throw()
  {
  return version.c_str();
  }
 
arianne::MessageFactory::MessageFactory()  
  {
  registerFunctions();
  }

arianne::MessageFactory::~MessageFactory()
  {
  }
  
void arianne::MessageFactory::registerFunctions()
  {
/*
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_C2S_ACTION,newInstance<MessageC2SAction>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_C2S_CHOOSECHARACTER,newInstance<MessageC2SChooseCharacter>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_C2S_LOGIN,newInstance<MessageC2SLogin>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_C2S_LOGOUT,newInstance<MessageC2SLogout>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_C2S_OUTOFSYNC,newInstance<MessageC2SOutOfSync>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_C2S_PERCEPTION_ACK,newInstance<MessageC2SPerceptionACK>));
*/
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_ACTION_ACK,newInstance<MessageS2CActionACK>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_CHARACTERLIST,newInstance<MessageS2CCharacterList>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_CHOOSECHARACTER_ACK,newInstance<MessageS2CChooseCharacterACK>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_CHOOSECHARACTER_NACK,newInstance<MessageS2CChooseCharacterNACK>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_LOGIN_ACK,newInstance<MessageS2CLoginACK>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_LOGIN_NACK,newInstance<MessageS2CLoginNACK>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_LOGOUT_ACK,newInstance<MessageS2CLogoutACK>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_LOGOUT_NACK,newInstance<MessageS2CLogoutNACK>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_PERCEPTION,newInstance<MessageS2CPerception>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_MAP,newInstance<MessageS2CRPMap>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_SERVERINFO,newInstance<MessageS2CServerInfo>));
  factory.insert(std::pair<Uint8,newInstanceFunction>(TYPE_S2C_INVALIDMESSAGE,newInstance<MessageS2CInvalidMessage>));
  }  

arianne::Message *arianne::MessageFactory::getMessage(std::vector<Uint8> const& content,IPaddress source)
  {
  if(content[0]==NETWORK_PROTOCOL)
    {
    std::map<Uint8,newInstanceFunction>::iterator it=factory.find(content[1]);
    if(it!=factory.end())
      {
      arianne::Serializer s(content);
      arianne::Message *msg=it->second();
      msg->read(s);
      
      return msg;
      }
    else
      {
      std::ostringstream out;
      out <<"Message[" << content[1] <<"] is not registered in the factory";
      arianne::global_trace.add("MessageFactory::getMessage","X",out.str());
      throw IOException("Message is not registered in the factory");
      }
    }
  else
    {
    arianne::global_trace.add("MessageFactory::getMessage","X","Message has incorrect protocol version");
    arianne::global_trace.add("MessageFactory::getMessage","X","Update this client to the newest version");
    throw InvalidVersionException("Message has incorrect protocol version");
    std::cout << int(content[0]) << std::endl;
    }

  return 0;
  }
