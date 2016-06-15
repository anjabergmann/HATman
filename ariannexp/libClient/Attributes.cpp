#include <sstream>
#include "Attributes.h"
#include "RPObject.h"
#include "Trace.h"

arianne::AttributeNotFoundException::AttributeNotFoundException(std::string const& attr):
  exception(),
  attribute("Attribute["+attr+"] not found")
  {
  }

arianne::AttributeNotFoundException::~AttributeNotFoundException() throw()
  {
  }
 
const char * arianne::AttributeNotFoundException::what() const throw()
  {
  return attribute.c_str();
  }

arianne::Attributes::Attributes(RPClass *rpclass)
  {
  this->rpclass=rpclass;
  }

arianne::Attributes::Attributes(Attributes const& copy)
  {
  rpclass=copy.rpclass;
  m_content=copy.m_content;
  }

arianne::Attributes::~Attributes()
  {
  }

void arianne::Attributes::setRPClass(arianne::RPClass *rpclass)
  {
  this->rpclass=rpclass;
  }
  
arianne::RPClass *arianne::Attributes::getRPClass()
  {
  return rpclass;
  }

bool arianne::Attributes::has(std::string const& attribute) const
  {
  return (m_content.find(attribute)!=m_content.end());
  }
  
void arianne::Attributes::put(std::string const& attribute, std::string const& value)
  {
  std::pair<std::map<std::string,std::string>::iterator,bool> result;
  result=m_content.insert(std::pair<std::string,std::string>(attribute,value));
  if(attribute=="type")
    {
    if(RPClass::hasRPClass(value))
      {
      setRPClass(RPClass::getRPClass(value));
      }
    else
      {
      setRPClass(defaultRPClass::getDefaultRPClass());
      }
    }
    
  if(!result.second)
    {
    m_content[attribute]=value;
    }  
  }
  
void arianne::Attributes::put(std::string const& attribute, Uint32 value)
  {
  std::ostringstream os;
  os << value;

  put(attribute,os.str());
  }
  
std::string arianne::Attributes::get(std::string const& attribute) const
  {
  std::map<std::string,std::string>::const_iterator it;
  it=m_content.find(attribute);
  
  if(it!=m_content.end())
    {
    return it->second;
    }
  else
    {
    throw arianne::AttributeNotFoundException(attribute);
    }
  }
  
Uint32 arianne::Attributes::getInt(std::string const& attribute) const
  {
  std::map<std::string,std::string>::const_iterator it;
  it=m_content.find(attribute);
  
  if(it!=m_content.end())
    {
    std::istringstream is(it->second);
    Uint32 value;
    is>>value;
    
    return value;
    }
  else
    {
    throw arianne::AttributeNotFoundException(attribute);
    }
  }
  
void arianne::Attributes::remove(std::string const& attribute)
  {
  m_content.erase(attribute);
  }

Uint32 arianne::Attributes::size() const
  {
  return (Uint32)m_content.size();
  }

void arianne::Attributes::clear()
  {
  m_content.clear();
  }
  
void arianne::Attributes::write(Serializer &s) const
  {
  global_trace.add("Attributes::write",">");
  s << rpclass->getName();
  s << (Uint32)m_content.size();
  
  std::map<std::string,std::string>::const_iterator it;
  for(it=m_content.begin();it!=m_content.end();++it)
    {
    Sint16 code=-1;
    std::ostringstream os1;
    os1 << rpclass->getName() <<" - (code:"<< (Sint32) code << ") " << it->first << " " <<it->second;
    global_trace.add("Attributes::write","D",os1.str());

    if(rpclass->hasAttribute(it->first))
      {
      code=rpclass->getCode(it->first);
      }
    
    std::ostringstream os;
    os << rpclass->getName() <<" - (code:"<< (Sint32) code << ") " << it->first << " " <<it->second;
    global_trace.add("Attributes::write","D",os.str());
    
    s << (Uint16)code;
    if(-1==code)
      {
      s << it->first;
      }

    if(rpclass->getType(it->first)==RPClass::STRING)
      {
      s << it->second;
      }
    else if(rpclass->getType(it->first)==RPClass::SHORT_STRING)
      {
      shortString sstring(it->second);
      s << sstring;
      }
    else if(rpclass->getType(it->first)==RPClass::INT)
      {
      Uint32 tmp=atoi(it->second.c_str());
      s << tmp;
      }
    else if(rpclass->getType(it->first)==RPClass::SHORT)
      {
      Uint16 tmp=atoi(it->second.c_str());
      s << tmp;
      }
    else if(rpclass->getType(it->first)==RPClass::BYTE)
      {
      Uint8 tmp=atoi(it->second.c_str());
      s << tmp;
      }
    else if(rpclass->getType(it->first)==RPClass::FLAG)
      {
      }
    else
      {
      /** WTF **/
      }
    }
  global_trace.add("Attributes::write","<");
  }
  
void arianne::Attributes::read(Serializer &s)
  {
  global_trace.add("Attributes::read",">");
  std::string rpclassName;
  s >> rpclassName;
  rpclass=RPClass::getRPClass(rpclassName);
  
  Uint32 size;
  s >> size;
  
  for(Uint32 i=0;i<size;++i)
    {
    Uint16 code;
    std::string key;
    std::string value;
    
    s >> code;
    if(((Sint16)code)==-1)
      {
      s >> key;
      }
    else
      {
      key=rpclass->getName(code);
      }
    
    if(rpclass->getType(key)==RPClass::STRING)
      {
      s >> value;
      put(key,value);
      }
    else if(rpclass->getType(key)==RPClass::SHORT_STRING)
      {
      shortString sstring("");
      s >> sstring;
      put(key,sstring);
      }
    else if(rpclass->getType(key)==RPClass::INT)
      {
      Uint32 tmp;
      s >> tmp;

      std::ostringstream os;
      os << (Uint32)tmp;

	  put(key,os.str());
      }
    else if(rpclass->getType(key)==RPClass::SHORT)
      {
      Uint16 tmp;
      s >> tmp;

      std::ostringstream os;
      os << (Uint32)tmp;

	  put(key,os.str());
      }
    else if(rpclass->getType(key)==RPClass::BYTE)
      {
      Uint8 tmp;
      s >> tmp;

      std::ostringstream os;
      os << (Uint32)tmp;

	  put(key,os.str());
      }
    else if(rpclass->getType(key)==RPClass::FLAG)
      {
      put(key,"");
      }
    else
      {
      /** WTF **/
      }
    //global_trace.add("Attributes::read","D",key+"="+value);
    }
  global_trace.add("Attributes::read","<");
  }

std::string arianne::Attributes::toString() const
  {
  std::ostringstream s;
  
  s << "[";
  std::map<std::string,std::string>::const_iterator it;
  for(it=m_content.begin();it!=m_content.end();++it)
    {
    s << "(" << it->first << "=" << it->second << ")";
    }
  s << "]";
  
  return s.str();
  }

bool arianne::Attributes::equals(arianne::Attributes const& b) const
  {
  return this->m_content==b.m_content;
  }

bool operator==(arianne::Attributes const& a, arianne::Attributes const& b)
  {
  return a.equals(b);
  }

std::ostream& operator<<(std::ostream& s, arianne::Attributes const& attributes)
  {
  return s << attributes.toString();
  }
  
