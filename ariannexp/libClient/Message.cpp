/* $Id: Message.cpp,v 1.8 2004/05/02 21:27:50 arianne_rpg Exp $ */
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
#include "Message.h"

arianne::Message::Message(IPaddress source)
  {
  this->type=TYPE_INVALID;
  this->clientid=arianne::CLIENTID_INVALID;
  this->source=source;
  }

arianne::Message::~Message()
  {
  }
  
void arianne::Message::write(Serializer &s) const
  {
  s << arianne::NETWORK_PROTOCOL;
  s << type;
  s << clientid;
  s << timestampMessage;
  }

void arianne::Message::read(Serializer &s)
  {
  Uint8 networkProtocol;  
  s >>  networkProtocol;
  if(networkProtocol!=arianne::NETWORK_PROTOCOL)
    {
    throw arianne::IOException("Network Protocol version incompatible");
    }
  
  s >> type;
  s >> clientid;
  s >> timestampMessage;
  }
  
  
