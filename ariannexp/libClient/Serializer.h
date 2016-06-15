/* $Id: Serializer.h,v 1.8 2004/07/04 22:50:14 arianne_rpg Exp $ */
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

#ifndef ARIANNE_Serializer_H
#define ARIANNE_Serializer_H

#include "SDL_types.h"

#include <vector>
#include <string>
#include <list>

namespace arianne
  {
  class IOException: public std::exception
    {
    std::string message;
    
    public:
    IOException(std::string const& msg);
    ~IOException() throw();
    const char *what() const throw();
    };
  
  class shortString: public std::string
    {
    public:
    shortString(std::string value):
      std::string(value)
      {
      }
    };
    
  class Serializer
    {
    /** not copyable */
    Serializer(Serializer const&);
    /** not assignable */
    Serializer& operator=(Serializer const&);
 
    std::vector<Uint8> m_data;
    Uint32 m_index;

    public:
    Serializer();
    Serializer(std::vector<Uint8> const& data);
    ~Serializer();

    /** add a byte */
    void push(Uint8 c);
  
    /** retrieve a byte */
    Uint8 pop();

    /** retrieve the full data vector */
    void toVector(std::vector<Uint8> &data) const;
    };

  /** The following overloaded operators write or read one of the basic data-types.
      These are designed to do so in an endian-independant way. **/
  Serializer& operator<<(Serializer& s, Uint8 const& t);
  Serializer& operator>>(Serializer& s, Uint8& t);

  Serializer& operator<<(Serializer& s, Uint16 const& t);
  Serializer& operator>>(Serializer& s, Uint16& t);

  Serializer& operator<<(Serializer& s, Uint32 const& t);
  Serializer& operator>>(Serializer& s, Uint32& t);

  Serializer& operator<<(Serializer& s, std::string const& b);
  Serializer& operator>>(Serializer& s, std::string& b);

  Serializer& operator<<(Serializer& s, shortString const& b);
  Serializer& operator>>(Serializer& s, shortString& b);

  template<class T> inline Serializer& operator<<(Serializer& s, std::list<T> const& l)
    {
    s << (Uint32)l.size();
    typename std::list<T>::const_iterator last = l.end();
    for(typename std::list<T>::const_iterator it = l.begin(); it!=last; ++it)
      {
      s << *it;
      }

    return s;
    }

  template<class T> inline Serializer& operator>>(Serializer& s, std::list<T>& l)
    {
    Uint32 size;
    s >> size;
    l.resize(size);
    typename std::list<T>::iterator last = l.end();
    for(typename std::list<T>::iterator it=l.begin(); it!=last; ++it)
      {
      s >> *it;
      }
      
    return s;
    }

  template<class T> inline Serializer& operator<<(Serializer& s, std::vector<T> const& l)
    {
    s << (Uint32)l.size();
	typename std::vector<T>::const_iterator last = l.end();
    for(typename std::vector<T>::const_iterator it = l.begin(); it!=last; ++it)
      {
      s << *it;
      }
    
    return s;
    }

  template<class T> inline Serializer& operator>>(Serializer& s, std::vector<T>& l)
    {
    Uint32 size;
    s >> size;
	l.resize(size);
	typename std::vector<T>::iterator last = l.end();
	for(typename std::vector<T>::iterator it=l.begin(); it!=last; ++it)
	  {
	  s >> *it;
	  }

    return s;
    }
  };

#endif
