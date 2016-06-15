/* $Id: S2CMessages.h,v 1.12 2004/05/20 18:45:20 arianne_rpg Exp $ */
/***************************************************************************
    copyright            : (C) 2004 by Miguel Angel Blanch Lardin
 ***************************************************************************
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ARIANNE_S2CMessages_H
#define ARIANNE_S2CMessages_H

#include "Message.h"
#include "RPObject.h"

namespace arianne
  {
  const Uint8 DELTA_PERCEPTION=0;
  const Uint8 TOTAL_PERCEPTION=1;

  class MessageS2CActionACK: public Message
    {
    public:
    MessageS2CActionACK();
    ~MessageS2CActionACK();
    
    Uint32 actionid;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageS2CCharacterList: public Message
    {
    public:
    MessageS2CCharacterList();
    ~MessageS2CCharacterList();
    
    std::list<std::string> characters;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageS2CChooseCharacterACK: public Message
    {
    public:
    MessageS2CChooseCharacterACK();
    ~MessageS2CChooseCharacterACK();
    
    Uint32 id;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageS2CChooseCharacterNACK: public Message
    {
    public:
    MessageS2CChooseCharacterNACK();
    ~MessageS2CChooseCharacterNACK();

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageS2CLoginACK: public Message
    {
    public:
    MessageS2CLoginACK();
    ~MessageS2CLoginACK();

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageS2CLoginNACK: public Message
    {
    public:
    MessageS2CLoginNACK();
    ~MessageS2CLoginNACK();
    
    Uint8 reason;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
	std::string toString() const;
    };

  class MessageS2CLogoutACK: public Message
    {
    public:
    MessageS2CLogoutACK();
    ~MessageS2CLogoutACK();

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageS2CLogoutNACK: public Message
    {
    public:
    MessageS2CLogoutNACK();
    ~MessageS2CLogoutNACK();

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageS2CPerception: public Message
    {
    public:
    MessageS2CPerception();
    ~MessageS2CPerception();
    
    Uint32 timestamp;
    Uint8 typePerception;
    std::list<arianne::RPObject> addedObjects;
    std::list<arianne::RPObject> modifiedAddedObjects;
    std::list<arianne::RPObject> modifiedDeletedObjects;
    std::list<arianne::RPObject> deletedObjects;
    arianne::RPObject myRPObject;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageS2CServerInfo: public Message
    {
    public:
    MessageS2CServerInfo();
    ~MessageS2CServerInfo();
    
    std::list<std::string> contents;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };
    
  class MessageS2CInvalidMessage: public Message
    {
    public:
    MessageS2CInvalidMessage();
    ~MessageS2CInvalidMessage();
    
    std::string reason;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };
  
  class MessageS2CRPMap: public Message
    {
    public:
    MessageS2CRPMap();
    ~MessageS2CRPMap();
    
    std::list<arianne::RPObject> content;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };
  };

#endif   
