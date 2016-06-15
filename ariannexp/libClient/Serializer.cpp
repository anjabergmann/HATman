/* $Id: Serializer.cpp,v 1.8 2004/07/04 22:50:14 arianne_rpg Exp $ */
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
#include "Serializer.h"
#include <algorithm>
#include <cassert>

using namespace std;

arianne::IOException::IOException(std::string const& msg): 
  exception(),
  message(msg)
  {
  }
arianne::IOException::~IOException() throw()
  {
  }

const char *arianne::IOException::what() const throw()
  {
  return message.c_str();
  }

arianne::Serializer::Serializer() : m_index(0)
  {
  }

arianne::Serializer::Serializer(std::vector<Uint8> const& data): m_data(data), m_index(0)
  {
  }

arianne::Serializer::~Serializer()
  {
  }

void arianne::Serializer::push(Uint8 c)
  {
  m_data.push_back(c);
  }

Uint8 arianne::Serializer::pop()
  {
  if(m_index < m_data.size()) 
    {
    return m_data[m_index++];
	} 
  else 
    {
    throw IOException("Index out of bounds");
	}
  }

void arianne::Serializer::toVector(vector<Uint8> &data) const
  {
  size_t size=m_data.size()-m_index;

  if (size<0)
    {
    throw IOException("Index out of bounds");
    }

  data.resize(size);
  if(size!=0)
    {
	copy (m_data.begin() + m_index, m_data.end(), data.begin());
	}
  }

arianne::Serializer& arianne::operator<<(arianne::Serializer& s, Uint8 const& t)
  {
  s.push(t);
  return s;
  }

arianne::Serializer& arianne::operator<<(arianne::Serializer& s, Uint16 const& t)
  {
  s.push(Uint8(t&255));
  s.push(Uint8((t>>8)&255));
  return s;
  }

arianne::Serializer& arianne::operator<<(arianne::Serializer& s, Uint32 const& t)
  {
  s.push(Uint8(t&255));
  s.push(Uint8((t>>8)&255));
  s.push(Uint8((t>>16)&255));
  s.push(Uint8((t>>24)&255));
  return s;
  }

arianne::Serializer& arianne::operator>>(arianne::Serializer& s, Uint8& t)
  {
  t=s.pop();
  return s;
  }

arianne::Serializer& arianne::operator>>(arianne::Serializer& s, Uint16& t)
  {
  t=s.pop();
  t+=s.pop()<<8;
  return s;
  }

arianne::Serializer& arianne::operator>>(arianne::Serializer& s, Uint32& t)
  {
  t=s.pop();
  t+=s.pop()<<8;
  t+=s.pop()<<16;
  t+=s.pop()<<24;
  return s;
  }

arianne::Serializer& arianne::operator<<(arianne::Serializer& s, string const& l)
  {
  s << (Uint32)l.size();

  string::const_iterator last = l.end();
  for(string::const_iterator it = l.begin(); it!=last; ++it)
    {
	s.push(*it);
	}

  return s;
  }

arianne::Serializer& arianne::operator>>(arianne::Serializer& s, string& l)
  {
  Uint32 size;
  s >> size;

  l.resize(size);

  string::iterator last = l.end();
  for(string::iterator it = l.begin(); it!=last; ++it)
    {
 	*it = s.pop();
 	}
    
  return s;
  }
  
arianne::Serializer& arianne::operator<<(arianne::Serializer& s, shortString const& l)
  {
  s << (Uint8)l.size();

  string::const_iterator last = l.end();
  for(string::const_iterator it = l.begin(); it!=last; ++it)
    {
	s.push(*it);
	}

  return s;
  }

arianne::Serializer& arianne::operator>>(arianne::Serializer& s, shortString& l)
  {
  Uint8 size;
  s >> size;

  l.resize(size);

  string::iterator last = l.end();
  for(string::iterator it = l.begin(); it!=last; ++it)
    {
 	*it = s.pop();
 	}
    
  return s;
  }
