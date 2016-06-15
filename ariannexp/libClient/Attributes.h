/* $Id: Attributes.h,v 1.9 2004/07/04 22:50:14 arianne_rpg Exp $ */
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

#ifndef ARIANNE_Attributes_H
#define ARIANNE_Attributes_H

#include <SDL_types.h>
#include <map>
#include <string>

#include "Serializer.h"

namespace arianne
  {
  class AttributeNotFoundException: public std::exception
    {
    std::string attribute;

    public:
    AttributeNotFoundException(std::string const& attribute);
    ~AttributeNotFoundException() throw();
    const char *what() const throw();
    };
  
  class RPClass;
      
  class Attributes
    {
    protected:    
    std::map<std::string,std::string> m_content;
    arianne::RPClass *rpclass;
    
    public:
	Attributes(arianne::RPClass *rpclass);
	Attributes(Attributes const& copy);
	virtual ~Attributes(void);
	
	void setRPClass(RPClass *rpclass);
	RPClass *getRPClass();
	
	bool has(std::string const& attribute) const;	
	void put(std::string const& attribute, std::string const& value);
	void put(std::string const& attribute, Uint32 value);	
	std::string get(std::string const& attribute) const;
	Uint32 getInt(std::string const& attribute) const;
	void remove(std::string const& attribute);
	Uint32 size() const;
	void clear();	
	
	virtual void write(arianne::Serializer &s) const;
	virtual void read(arianne::Serializer &s);

    bool equals(Attributes const& b) const;
    std::string toString() const;
    };
  }

bool operator==(arianne::Attributes const& a, arianne::Attributes const& b);
std::ostream& operator<<(std::ostream& s, arianne::Attributes const& attributes);

#endif
