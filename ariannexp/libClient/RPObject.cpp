#include "RPObject.h"
#include "Trace.h"
#include <sstream>

void arianne::RPClass::AttributeDesc::write(arianne::Serializer &s) const
  {
  s << (Uint16)code;
  s << name;
  s << (Uint8)type;
  s << (Uint8)visibility;
  }
  
void arianne::RPClass::AttributeDesc::read(arianne::Serializer &s)
  {
  Uint16 tmp16;
  s >> tmp16;
  code=tmp16;
  s >> name;
  
  Uint8 tmp;
  s >> tmp;
  type=(TYPE_ATTRIBUTE)tmp;
  
  s >> tmp;
  visibility=(VISIBILITY)tmp;  

  std::ostringstream os;
  os << "Code: "<< tmp16<< " ("<< name <<") type/visibility: "<< type << visibility;
  global_trace.add("AttributesDesc::read","D",os.str());
  }

std::map<std::string, arianne::RPClass*> arianne::RPClass::rpclasses;

void arianne::RPClass::isA(RPClass *parent)
  {
  this->parent=parent;
  }
  
std::string const& arianne::RPClass::getName() const
  {
  return name;
  }
  
Sint16 arianne::RPClass::getCode(std::string const& name) const
  {
  std::map<std::string,AttributeDesc>::const_iterator it;
  it=attributes.find(name);
  
  if(it!=attributes.end())
    {
    return it->second.code;
    }
  else
    {    
    if(parent)
      {
      return parent->getCode(name);
      }
    else
      {      
      return -1;
      }
    }
  }
  
std::string arianne::RPClass::getName(Uint16 code) const
  {
  std::map<std::string,AttributeDesc>::const_iterator it;
  for(it=attributes.begin();it!=attributes.end();++it)
    {
    if(it->second.code==code)
      {
      return it->second.name;
      }
    }
  
  if(parent)
    {
    return parent->getName(code);
    }
  else
    {      
    return "";
    }
  }
  
arianne::RPClass::TYPE_ATTRIBUTE arianne::RPClass::getType(std::string const& name) const
  {
  std::map<std::string,AttributeDesc>::const_iterator it;
  it=attributes.find(name);
  
  if(it!=attributes.end())
    {
    return it->second.type;
    }
  else
    {
    if(parent)
      {
      return parent->getType(name);
      }
    else
      {      
      return STRING;
      }
    }
  }
  
arianne::RPClass::VISIBILITY arianne::RPClass::getVisibility(std::string const& name) const
  {
  std::map<std::string,AttributeDesc>::const_iterator it;
  it=attributes.find(name);
  
  if(it!=attributes.end())
    {
    return it->second.visibility;
    }
  else
    {
    if(parent)
      {
      return parent->getVisibility(name);
      }
    else
      {      
      return VISIBLE;
      }
    }
  }
  
bool arianne::RPClass::hasAttribute(std::string const& name) const
  {
  std::map<std::string,AttributeDesc>::const_iterator it;
  it=attributes.find(name);
  
  if(it!=attributes.end())
    {
    return true;
    }
  else
    {
    if(parent)
      {
      return parent->hasAttribute(name);
      }
    else
      {      
      return false;
      }
    }
  }
 
void arianne::RPClass::read(arianne::Serializer &s)
  {
  Uint32 sizeAttributes;
  s >> sizeAttributes;
  s >> name;
  
  Uint8 hasParent;
  s >> hasParent;
  if(hasParent)
    {
    std::string nameParent;
    s >> nameParent;
    parent=getRPClass(nameParent);
    }
  else
    {
    parent=0;
    }
  
  std::ostringstream os;
  os << "name: "<< name << " ("<< name;
  global_trace.add("RPClass::read","D",os.str());

  for(Uint32 i=0;i<sizeAttributes;++i)
    {
    RPClass::AttributeDesc desc;
    desc.read(s);
    attributes[desc.name]=desc;
    }
  
  addRPClass(this);
  }
  
bool arianne::RPClass::hasRPClass(std::string const& name)
  {
  std::map<std::string,RPClass*>::const_iterator it;
  it=rpclasses.find(name);
  
  if(it!=rpclasses.end())
    {
    return true;
    }
  else
    {
    return false;
    }
  }
  
arianne::RPClass *arianne::RPClass::getRPClass(std::string const& name)
  {
  std::map<std::string,RPClass*>::const_iterator it;
  it=rpclasses.find(name);
  
  if(it!=rpclasses.end())
    {
    return it->second;
    }
  else
    {
    return 0;
    }
  }
  
void arianne::RPClass::addRPClass(arianne::RPClass *rpclass)
  {
  rpclasses[rpclass->name]=rpclass;
  }
  
int arianne::RPClass::size()
  {
  return (int)rpclasses.size();
  }

arianne::defaultRPClass *arianne::defaultRPClass::defaultrpclass=0; 

arianne::defaultRPClass::defaultRPClass():
  RPClass()
  {
  name="";
  }

arianne::defaultRPClass *arianne::defaultRPClass::getDefaultRPClass()
  {
  if(!defaultrpclass)
    {
    defaultrpclass=new defaultRPClass();
    RPClass::addRPClass(defaultrpclass);
    }

  return defaultrpclass;
  }
  
Sint16 arianne::defaultRPClass::getCode(std::string const& name) const
  {
  return -1;
  }
  
std::string arianne::defaultRPClass::getName(Uint16 code) const
  {
  return "";  
  }
  
arianne::defaultRPClass::TYPE_ATTRIBUTE arianne::defaultRPClass::getType(std::string const& name) const
  {
  return STRING;
  }
  
arianne::defaultRPClass::VISIBILITY arianne::defaultRPClass::getVisibility(std::string const& name) const
  {
  return VISIBLE;
  }
  
bool arianne::defaultRPClass::hasAttribute(std::string const& name) const
  {
  return true;
  }



arianne::SlotNotFoundException::SlotNotFoundException(std::string n):
  exception(),
  name("Slot ("+n+") not found")
  {
  }

arianne::SlotNotFoundException::~SlotNotFoundException() throw()
  {
  }

const char * arianne::SlotNotFoundException::what() const throw()
  {
  return name.c_str();
  } 


arianne::RPObject::RPObject():
  Attributes(defaultRPClass::getDefaultRPClass())
  {
  }
  
arianne::RPObject::~RPObject()
  {  
  }

void arianne::RPObject::addSlot(RPSlot const& slot)
  {
  std::pair<std::map<std::string,RPSlot>::iterator,bool> result;
  result=m_slots.insert(std::pair<std::string,RPSlot>(slot.name,slot));
  if(!result.second)
    {
    m_slots[slot.name]=slot;
    }

  
  }
  
arianne::RPSlot* arianne::RPObject::getSlot(std::string const& name)
  {
  std::map<std::string,RPSlot>::iterator it;
  it=m_slots.find(name);
  
  if(it!=m_slots.end())
    {
    return &(it->second);
    }
  else
    {
    throw SlotNotFoundException(name);
    }
  }

void arianne::RPObject::removeSlot(std::string const& name)
  {
  std::map<std::string,RPSlot>::iterator it;
  it=m_slots.find(name);
  
  if(it!=m_slots.end())
    {
    m_slots.erase(it);
    }
  else
    {
    throw SlotNotFoundException(name);
    }
  }
  
bool arianne::RPObject::hasSlot(std::string const& name) const
  {
  std::map<std::string,RPSlot>::const_iterator it;
  it=m_slots.find(name);
  
  if(it!=m_slots.end())
    {
    return true;
    }
  else
    {
    return false;
    }
  }
  

void arianne::RPObject::write(arianne::Serializer &s) const
  {
  global_trace.add("RPObject::write",">");
  Attributes::write(s);  

  s << (Uint32)m_slots.size();
  
  std::map<std::string,RPSlot>::const_iterator it;  
  for(it=m_slots.begin();it!=m_slots.end();++it)
    {
    it->second.write(s);        
    }
    
  global_trace.add("RPObject::write","<");
  }
  
void arianne::RPObject::read(arianne::Serializer &s)
  {
  global_trace.add("RPObject::read",">");
  Attributes::read(s);
  
  Uint32 size;
  s >> size;
  
  for(Uint32 i=0;i<size;++i)
    {
    RPSlot slot("");
    slot.read(s);
    addSlot(slot);    
    }

  global_trace.add("RPObject::read","<");
  }

bool arianne::RPObject::equals(arianne::RPObject const& b) const
  {
  return Attributes::equals(b) && m_slots==b.m_slots;
  }

std::string arianne::RPObject::toString() const
  {
  std::ostringstream s;
  s << "RP Object with ";
  s << Attributes::toString();
  s << " and Slots [";

  std::map<std::string,RPSlot>::const_iterator it;
  for(it=m_slots.begin();it!=m_slots.end();++it)
    {
    /* TODO: Why does "s << it->second" does not work? */
    s << it->second.toString();
    }
  s << "]";
  
  return s.str();
  }  
  
void arianne::RPObject::applyAddedDifferences(RPObject added)
  {
  applyDifferences(&added,0);
  }
  
void arianne::RPObject::applyDeletedDifferences(RPObject deleted)
  {
  applyDifferences(0,&deleted);
  }

void arianne::RPObject::applyDifferences(arianne::RPObject *added, arianne::RPObject *deleted)
  {
  if(deleted)
    {
    std::map<std::string,std::string>::iterator it;
    for(it=deleted->m_content.begin();it!=deleted->m_content.end();++it)
      {
      if(it->first!="id")
        {
        remove(it->first);
        }
      }
    
    std::map<std::string,RPSlot>::iterator sit;
    for(sit=deleted->m_slots.begin();sit!=deleted->m_slots.end();++sit)
      {
      if(sit->second.size()==0)
        {
        removeSlot(sit->second.name);
        }
      else
        {        
        for(Uint32 i=0;i<sit->second.size();++i)
          {
          RPObject copy=sit->second.getByPosition(i);
          
          if(copy.size()==1)
            {
            getSlot(sit->second.name)->remove(copy.getInt("id"));
            }
          else
            {
            RPObject actualObject=getSlot(sit->second.name)->get(copy.getInt("id"));
            actualObject.applyDifferences(0,&copy);
            getSlot(sit->second.name)->add(actualObject);
            }
          }
        }
      }
    }

  if(added)
    {
    std::map<std::string,std::string>::iterator it;
    for(it=added->m_content.begin();it!=added->m_content.end();++it)
      {
      put(it->first,it->second);
      }
    
    std::map<std::string,RPSlot>::iterator sit;
    for(sit=added->m_slots.begin();sit!=added->m_slots.end();++sit)
      {
      if(!hasSlot(sit->second.name))
        {
        addSlot(RPSlot(sit->second.name));
        }

      for(Uint32 i=0;i<sit->second.size();++i)
        {
        RPObject copy=sit->second.getByPosition(i);
        if(getSlot(sit->second.name)->has(copy.getInt("id")))
          {
          getSlot(sit->second.name)->get(copy.getInt("id")).applyDifferences(&copy,0);
          }
        else
          {
          getSlot(sit->second.name)->add(copy);
          }
        }
      }
    }
  }
  
Uint32 arianne::RPObject::size()
  {
  int total=arianne::Attributes::size();

  std::map<std::string,RPSlot>::iterator sit;
  for(sit=m_slots.begin();sit!=m_slots.end();++sit)
    {
    for(Uint32 i=0;i<sit->second.size();++i)
      {
      total+=sit->second.getByPosition(i).size();
      }
    }
  
  return total;
  }

void arianne::RPObject::removeAllHidden()
  {
  std::map<std::string,std::string>::iterator it=m_content.begin();
  while(it!=m_content.end())
    {
    if(it->first[0]=='!')
      {
      remove(it->first);
      it=m_content.begin();
      }
      
    ++it;
    }
    
  std::map<std::string,RPSlot>::iterator sit=m_slots.begin();
  while(sit!=m_slots.end())
    {
    if(sit->first[0]=='!')
      {
      removeSlot(sit->first);
      sit=m_slots.begin();
      }
      
    ++sit;
    }
  }
  
  
bool arianne::operator==(arianne::RPObject const& a, arianne::RPObject const& b)
  {
  return a.equals(b);
  }

bool arianne::operator<(arianne::RPObject const& a, arianne::RPObject const& b)
  {
  return false;
  }
 
std::ostream& arianne::operator<<(std::ostream& s, arianne::RPObject const& object)
  {
  return s << object.toString();
  }

arianne::Serializer& arianne::operator<<(arianne::Serializer& s, arianne::RPObject const& b)
  {
  b.write(s);
  return s;
  }
  
arianne::Serializer& arianne::operator>>(arianne::Serializer& s, arianne::RPObject& b)
  {
  b.read(s);
  return s;
  }
  
