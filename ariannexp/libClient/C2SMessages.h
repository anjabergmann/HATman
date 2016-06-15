/* $Id: C2SMessages.h,v 1.8 2004/05/14 15:52:53 arianne_rpg Exp $ */
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

#ifndef ARIANNE_C2SMessages_H
#define ARIANNE_C2SMessages_H

#include "Message.h"
#include "RPAction.h"

namespace arianne
  {
  class MessageC2SAction: public Message
    {
    public:
    MessageC2SAction();
    ~MessageC2SAction();
    
    RPAction action;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };
    
  class MessageC2SChooseCharacter: public Message
    {
    public:
    MessageC2SChooseCharacter();
    ~MessageC2SChooseCharacter();
    
    std::string character;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageC2SLogin: public Message
    {
    public:
    MessageC2SLogin();
    ~MessageC2SLogin();
    
    std::string username;
    std::string password;

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageC2SLogout: public Message
    {
    public:
    MessageC2SLogout();
    ~MessageC2SLogout();
    
	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageC2SPerceptionACK: public Message
    {
    public:
    MessageC2SPerceptionACK();
    ~MessageC2SPerceptionACK();
    
	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };

  class MessageC2SOutOfSync: public Message
    {
    public:
    MessageC2SOutOfSync();
    ~MessageC2SOutOfSync();
    
	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };
  };

#endif  
