#include "C2SMessages.h"

arianne::MessageC2SAction::MessageC2SAction():
  Message(IPaddress())
  {
  this->type=TYPE_C2S_ACTION;
  }

arianne::MessageC2SAction::~MessageC2SAction()
  {
  }

void arianne::MessageC2SAction::write(Serializer &s) const
  {
  Message::write(s);
  action.write(s);
  }
  
void arianne::MessageC2SAction::read(Serializer &s)
  {
  Message::read(s);
  action.read(s);
  }

arianne::MessageC2SChooseCharacter::MessageC2SChooseCharacter():
  Message(IPaddress())
  {
  this->type=TYPE_C2S_CHOOSECHARACTER;
  this->character=std::string();
  }

arianne::MessageC2SChooseCharacter::~MessageC2SChooseCharacter()
  {
  }

void arianne::MessageC2SChooseCharacter::write(Serializer &s) const
  {
  Message::write(s);
  s << character;
  }
  
void arianne::MessageC2SChooseCharacter::read(Serializer &s)
  {
  Message::read(s);
  s >> character;
  }

arianne::MessageC2SLogin::MessageC2SLogin():
  Message(IPaddress())
  {
  this->type=TYPE_C2S_LOGIN;
  this->username=std::string();
  this->password=std::string();
  }

arianne::MessageC2SLogin::~MessageC2SLogin()
  {
  }

void arianne::MessageC2SLogin::write(Serializer &s) const
  {
  Message::write(s);
  s << username << password;
  }
  
void arianne::MessageC2SLogin::read(Serializer &s)
  {
  Message::read(s);
  s >> username >> password;
  }

arianne::MessageC2SLogout::MessageC2SLogout():
  Message(IPaddress())
  {
  this->type=TYPE_C2S_LOGOUT;
  }

arianne::MessageC2SLogout::~MessageC2SLogout()
  {
  }

void arianne::MessageC2SLogout::write(Serializer &s) const
  {
  Message::write(s);
  }
  
void arianne::MessageC2SLogout::read(Serializer &s)
  {
  Message::read(s);
  }

arianne::MessageC2SPerceptionACK::MessageC2SPerceptionACK():
  Message(IPaddress())
  {
  this->type=TYPE_C2S_PERCEPTION_ACK;
  }

arianne::MessageC2SPerceptionACK::~MessageC2SPerceptionACK()
  {
  }

void arianne::MessageC2SPerceptionACK::write(Serializer &s) const
  {
  Message::write(s);
  }
  
void arianne::MessageC2SPerceptionACK::read(Serializer &s)
  {
  Message::read(s);
  }
 
 
arianne::MessageC2SOutOfSync::MessageC2SOutOfSync():
  Message(IPaddress())
  {
  this->type=TYPE_C2S_OUTOFSYNC;
  }

arianne::MessageC2SOutOfSync::~MessageC2SOutOfSync()
  {
  }

void arianne::MessageC2SOutOfSync::write(Serializer &s) const
  {
  Message::write(s);
  }
  
void arianne::MessageC2SOutOfSync::read(Serializer &s)
  {
  Message::read(s);
  }
