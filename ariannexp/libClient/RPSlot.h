/* $Id: RPSlot.h,v 1.16 2004/04/30 20:50:26 arianne_rpg Exp $ */
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

#ifndef ARIANNE_RPSlot_H
#define ARIANNE_RPSlot_H

#include "Serializer.h"
#include <map>

namespace arianne
  {
  class RPObject;
  
  class ObjectNotFoundException: public std::exception
    {
    Uint32 id;
    
    public:
    ObjectNotFoundException(Uint32 id);    
    ~ObjectNotFoundException() throw();
    const char *what() const throw();
    };
  
  class RPSlot
    {
    std::map<Uint32,RPObject> m_objects;
    
    public:
    RPSlot();
    RPSlot(arianne::RPSlot const& slot);
    RPSlot(std::string const& slot);
    ~RPSlot();
    
    void add(RPObject const& object);
    RPObject get(Uint32 id) const;
    RPObject& get(Uint32 id);
    RPObject& getByPosition(Uint32 i);
    RPObject& get();
    bool has(Uint32 id) const;
    void remove(Uint32 id);
    Uint32 size() const;        
    void clear();
    
    std::string name;    
    
	virtual void write(arianne::Serializer &s) const;
	virtual void read(arianne::Serializer &s);

	std::map<Uint32,RPObject>::iterator begin();
	std::map<Uint32,RPObject>::iterator end();
	std::map<Uint32,RPObject>::iterator find(Uint32 id);

    bool equals(arianne::RPSlot const& b) const;
    std::string toString() const;
    };

  bool operator==(arianne::RPSlot const& a, arianne::RPSlot const& b);
  std::ostream& operator<<(std::ostream& s, arianne::RPSlot const& slot);  
  }

#endif
