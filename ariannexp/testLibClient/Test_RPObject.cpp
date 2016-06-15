/* $Id: Test_RPObject.cpp,v 1.6 2004/04/03 17:42:51 arianne_rpg Exp $ */
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
#include "Test_RPObject.h"

using namespace std;
using namespace arianne;

Test_RPObject::Test_RPObject()
  {
  }

void Test_RPObject::testNormal()
  {
  RPObject object;
  
  CPPUNIT_ASSERT(!object.has("test"));
  object.put("test","value");
  CPPUNIT_ASSERT(object.has("test"));
  CPPUNIT_ASSERT_EQUAL(object.get("test"),string("value"));
  object.remove("test");
  CPPUNIT_ASSERT(!object.has("test"));

  CPPUNIT_ASSERT(!object.hasSlot("hand"));
  RPSlot slot("hand");
  object.addSlot(slot);
  CPPUNIT_ASSERT(object.hasSlot("hand"));
  CPPUNIT_ASSERT_EQUAL(*object.getSlot("hand"),slot);
  }
  
void Test_RPObject::testExceptions()
  {
  try
    {
    RPObject object;
    
    object.getSlot("hand");
    
    CPPUNIT_FAIL("It should have thrown exception");
    }
  catch(SlotNotFoundException e)
    {
    CPPUNIT_ASSERT(true);
    }
  }
  
void Test_RPObject::testSerialization()
  {
  Serializer s;
  RPObject object;
  
  object.put("test","foo");    
  object.put("another test","baaar");    
  object.put("another test",1);
  object.addSlot(RPSlot("hand"));    
  
  object.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  
  RPObject value;
  value.read(s);
  
  CPPUNIT_ASSERT_EQUAL(object,value);  
  }
  
void Test_RPObject::testDeltaDelta()
  {
  RPObject object;
  
  CPPUNIT_ASSERT(!object.has("test"));
  object.put("test","value");
  CPPUNIT_ASSERT(object.has("test"));
  CPPUNIT_ASSERT_EQUAL(object.get("test"),string("value"));
  object.remove("test");
  CPPUNIT_ASSERT(!object.has("test"));

  CPPUNIT_ASSERT(!object.hasSlot("hand"));
  RPSlot slot("hand");
  object.addSlot(slot);
  CPPUNIT_ASSERT(object.hasSlot("hand"));
  CPPUNIT_ASSERT_EQUAL(*object.getSlot("hand"),slot);
  }

  

