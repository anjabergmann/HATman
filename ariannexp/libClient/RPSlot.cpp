#include "RPSlot.h"
#include "RPObject.h"
#include "Trace.h"
#include <sstream>

arianne::ObjectNotFoundException::ObjectNotFoundException(Uint32 id):
  std::exception(),
  id(id)
  {  
  }

arianne::ObjectNotFoundException::~ObjectNotFoundException() throw()
  {
  }
  
const char *arianne::ObjectNotFoundException::what() const throw()
  {
  return "Object not found";
  }

  

arianne::RPSlot::RPSlot()
  {
  }

arianne::RPSlot::RPSlot(RPSlot const& slot):
  name(slot.name),
  m_objects()
  {
  std::map<Uint32,RPObject>::const_iterator it;
  for(it=slot.m_objects.begin();it!=slot.m_objects.end();++it)
    {
    add(it->second);
    }
  }

arianne::RPSlot::RPSlot(std::string const& slot):
  name(slot)
  {
  }
  
arianne::RPSlot::~RPSlot()
  {
  clear();
  }

void arianne::RPSlot::clear()
  {
  //std::map<Uint32,RPObject>::iterator it;
  //for(it=m_objects.begin();it!=m_objects.end();++it)
  //  {
  //  delete it->second;
  //  it->second=0;
  //  }
  }

void arianne::RPSlot::add(arianne::RPObject const& object)
  {
  std::pair<std::map<Uint32,RPObject>::iterator,bool> result;
  result=m_objects.insert(std::pair<Uint32,arianne::RPObject>(object.getInt("id"),object));
  if(!result.second)
    {
    //delete m_objects[object.getInt("id")];
    m_objects[object.getInt("id")]=object;
    }
  }
  
arianne::RPObject& arianne::RPSlot::get(Uint32 id)
  {
  std::map<Uint32,RPObject>::iterator it;
  it=m_objects.find(id);
  
  if(it!=m_objects.end())
    {
    return it->second;
    }
  else
    {
    throw arianne::ObjectNotFoundException(id);
    }
  }

arianne::RPObject& arianne::RPSlot::getByPosition(Uint32 i)
  {
  std::map<Uint32,RPObject>::iterator it;
  for(it=m_objects.begin();it!=m_objects.end();++it)
    {
    if(i==0)
      {
      return it->second;
      }
    --i;
    }

  throw arianne::ObjectNotFoundException(i);
  }
 
arianne::RPObject& arianne::RPSlot::get()
  {  
  std::map<Uint32,RPObject>::iterator it;
  it=m_objects.begin();
  
  if(it!=m_objects.end())
    {
    return it->second;
    }
  else
    {
    throw arianne::ObjectNotFoundException(0);
    }
  }
arianne::RPObject arianne::RPSlot::get(Uint32 id) const
  {  
  std::map<Uint32,RPObject>::const_iterator it;
  it=m_objects.find(id);
  
  if(it!=m_objects.end())
    {
    return (it->second);
    }
  else
    {
    throw arianne::ObjectNotFoundException(id);
    }
  }

  
bool arianne::RPSlot::has(Uint32 id) const
  {
  std::map<Uint32,RPObject>::const_iterator it;
  it=m_objects.find(id);
  
  if(it!=m_objects.end())
    {
    return true;
    }
  else
    {
    return false;
    }
  }
  
void arianne::RPSlot::remove(Uint32 id)
  {
  std::map<Uint32,RPObject>::iterator it;
  it=m_objects.find(id);
  
  if(it!=m_objects.end())
    {
    //delete it->second;
    m_objects.erase(id);
    }
  else
    {
    throw arianne::ObjectNotFoundException(id);
    }
  }
  
Uint32 arianne::RPSlot::size() const
  {
  return (Uint32)m_objects.size();
  }
  

bool arianne::RPSlot::equals(arianne::RPSlot const& b) const
  {  
  std::map<Uint32,RPObject>::const_iterator it;  
  for(it=m_objects.begin();it!=m_objects.end();++it)
    {
    if(!(b.has(it->first) && it->second.equals(b.get(it->first))))
      {
      return false;
      }      
    }
    
  return name==b.name;
  }

void arianne::RPSlot::write(arianne::Serializer &s) const
  {
  global_trace.add("RPSlot::write",">");
  s << name;
  
  s << (Uint32)m_objects.size();
  
  std::map<Uint32,RPObject>::const_iterator it;  
  for(it=m_objects.begin();it!=m_objects.end();++it)
    {
    it->second.write(s);        
    }

  global_trace.add("RPSlot::write","<");
  }
  
void arianne::RPSlot::read(arianne::Serializer &s)
  {
  global_trace.add("RPSlot::read",">");
  s >> name;

  global_trace.add("RPSlot::read","D",name);
  
  Uint32 size;
  s >> size;
  
  for(Uint32 i=0;i<size;++i)
    {
    RPObject object;
    object.read(s);
    add(object);    
    }
  global_trace.add("RPSlot::read","<");
  }

std::string arianne::RPSlot::toString() const
  {
  std::ostringstream s;
  
  s << "RPSlot (" << name << ") containing [";
  
  std::map<Uint32,RPObject>::const_iterator it;  
  for(it=m_objects.begin();it!=m_objects.end();++it)
    {
    s << it->second.toString();
    }
  
  s << "]";
  
  return s.str();
  }

std::map<Uint32,arianne::RPObject>::iterator arianne::RPSlot::begin()
  {
  return m_objects.begin();
  }  

std::map<Uint32,arianne::RPObject>::iterator arianne::RPSlot::end()
  {
  return m_objects.end();
  }  
  
std::map<Uint32,arianne::RPObject>::iterator arianne::RPSlot::find(Uint32 id)
  {
  return m_objects.find(id);
  }  
  
  
  
bool arianne::operator==(arianne::RPSlot const& a, arianne::RPSlot const& b)
  {
  return a.equals(b);
  }

std::ostream& arianne::operator<<(std::ostream& s, arianne::RPSlot const& slot)
  {
  return s << slot.toString();
  }
