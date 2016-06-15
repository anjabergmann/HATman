/* $Id: Message.h,v 1.14 2004/07/04 22:50:14 arianne_rpg Exp $ */
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

#ifndef ARIANNE_Message_H
#define ARIANNE_Message_H

#include <SDL_types.h>
#include <SDL_net.h>

#include "Serializer.h"

namespace arianne
  {
  enum MessageType
    {
    TYPE_INVALID=0,
    TYPE_C2S_LOGIN=1,
    TYPE_S2C_LOGIN_ACK=10,
    TYPE_S2C_LOGIN_NACK=11,  
    TYPE_S2C_CHARACTERLIST=2,  
    TYPE_C2S_CHOOSECHARACTER=3,
    TYPE_S2C_CHOOSECHARACTER_ACK=30,
    TYPE_S2C_CHOOSECHARACTER_NACK=31,  
    TYPE_C2S_LOGOUT=4,
    TYPE_S2C_LOGOUT_ACK=40,
    TYPE_S2C_LOGOUT_NACK=41,  
    TYPE_C2S_ACTION=5,
    TYPE_S2C_ACTION_ACK=50,  
    TYPE_S2C_PERCEPTION=6,
    TYPE_C2S_PERCEPTION_ACK=61,  
    TYPE_S2C_MAP=62,  
    TYPE_C2S_OUTOFSYNC=63,
    TYPE_S2C_SERVERINFO=7, 
    TYPE_S2C_INVALIDMESSAGE=8    
    };

  const Uint32 CLIENTID_INVALID=(Uint32)-1;
  
  const Uint8 NETWORK_PROTOCOL=15;

  class Message
    {
    public:
    Message(IPaddress source);
    virtual ~Message();

    /** The type of message */
    Uint8 type;
    /** Code that identify the client */
    Uint32 clientid;
    /** Timestamp of the message */
    Uint32 timestampMessage;
    /** Host:Port of the source of the message */
    IPaddress source;  

	virtual void write(Serializer &s) const;
	virtual void read(Serializer &s);
    };
  };

#endif //ARIANNE_Message_H
