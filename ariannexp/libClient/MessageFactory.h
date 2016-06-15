/* $Id: MessageFactory.h,v 1.5 2004/03/31 15:28:47 arianne_rpg Exp $ */
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

#ifndef ARIANNE_MessageFactory_H
#define ARIANNE_MessageFactory_H

#include "Message.h"
#include "Trace.h"
#include <map>

namespace arianne
  {
  template<class T> Message *newInstance()
    {
    return new T();
    };
    
  class InvalidVersionException: public std::exception
    {
    std::string version;

    public:
    InvalidVersionException(std::string const& version);
    ~InvalidVersionException() throw();
    const char *what() const throw();
    };
    
  class MessageFactory
    {
    typedef arianne::Message *(*newInstanceFunction)();
    std::map<Uint8,newInstanceFunction> factory;
    
    void registerFunctions();
    
    public:
	MessageFactory();
	~MessageFactory();
	
	Message *getMessage(std::vector<Uint8> const& content,IPaddress source);
    };
  };

#endif
