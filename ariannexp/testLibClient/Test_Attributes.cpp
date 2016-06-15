/* $Id: Test_Attributes.cpp,v 1.6 2004/07/04 22:50:20 arianne_rpg Exp $ */
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
#include "Test_Attributes.h"
#include "../libClient/RPObject.h"

using namespace std;
using namespace arianne;

Test_Attributes::Test_Attributes()
  {
  }
  
void Test_Attributes::testNormal()
  {
  Attributes attr(defaultRPClass::getDefaultRPClass());
  
  CPPUNIT_ASSERT(!attr.has("test"));
  attr.put("test","value");
  CPPUNIT_ASSERT(attr.has("test"));
  CPPUNIT_ASSERT_EQUAL(attr.get("test"),string("value"));
  attr.remove("test");
  CPPUNIT_ASSERT(!attr.has("test"));
  
  attr.put("test","foo");    
  attr.put("another test","baaar");    
  
  Attributes copy=attr;
  
  CPPUNIT_ASSERT_EQUAL(attr,copy); 
  
  attr.put("foo",1);
  CPPUNIT_ASSERT_EQUAL(attr.getInt("foo"),(Uint32)1);
  }
  
void Test_Attributes::testExceptions()
  {
  try
    {
    Attributes attr(defaultRPClass::getDefaultRPClass());
    
    attr.get("test");
    
    CPPUNIT_FAIL("It should have thrown exception");
    }
  catch(AttributeNotFoundException e)
    {
    CPPUNIT_ASSERT(true);
    }
  }
  
void Test_Attributes::testSerialization()
  {
  Serializer s;
  Attributes attr(defaultRPClass::getDefaultRPClass());
  
  attr.put("test","foo");    
  attr.put("another test","baaar");    
  attr.put("another test",1);    
  
  attr.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  
  Attributes value(defaultRPClass::getDefaultRPClass());
  value.read(s);
  
  CPPUNIT_ASSERT_EQUAL(attr,value);
  }
  
  

