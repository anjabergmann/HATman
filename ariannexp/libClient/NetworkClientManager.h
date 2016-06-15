/* $Id: NetworkClientManager.h,v 1.11 2004/05/03 21:23:53 arianne_rpg Exp $ */
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

#ifndef ARIANNE_NetworkClientManager_H
#define ARIANNE_NetworkClientManager_H

#include "S2CMessages.h"
#include "C2SMessages.h"
#include "MessageFactory.h"
#include "Trace.h"

#include <map>

#include <SDL_net.h>

namespace arianne
  {
  const int MARAUROA_PORT=3214;
  const int NETWORK_PACKET_SIZE=1500;
  
  const int CLIENT_NETWORK_NUM_READ=5;
  const int CLIENT_MESSAGE_DROPPED_TIMEOUT=60000;
  
  class NetworkClientManager
    {
    Sint32 clientid;
    UDPsocket socket;
    IPaddress address;

    struct PacketContainer
      {
      Uint8 signature;
      Sint8 remaining;
      std::vector<Uint8> content;
      IPaddress address;    
      Uint32 timestamp;
      };    
    
    std::map<Uint8,NetworkClientManager::PacketContainer> pendingPackets;
    MessageFactory msgFactory;
    
    public:
	NetworkClientManager(std::string const& host,Uint16 port);
	~NetworkClientManager();
	
	Message *getMessage();
	void addMessage(Message& message);
	};
  };

#endif
