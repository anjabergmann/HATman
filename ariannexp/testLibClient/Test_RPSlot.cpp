/* $Id: Test_RPSlot.cpp,v 1.9 2004/05/03 21:53:40 arianne_rpg Exp $ */
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
#include "Test_RPSlot.h"
#include "../libClient/RPObject.h"

using namespace std;
using namespace arianne;

Test_RPSlot::Test_RPSlot()
  {
  }
  
void Test_RPSlot::testNormal()
  {
  RPSlot slot("name");
  
  RPObject object;
  object.put("id",100);
  object.put("test","name");
  
  slot.add(object);
  CPPUNIT_ASSERT(slot.has(100));
  
  RPObject value=slot.get(100);
  CPPUNIT_ASSERT_EQUAL(object,value);
  
  slot.remove(100);
  CPPUNIT_ASSERT(!slot.has(100));
  }
  
void Test_RPSlot::testExceptions()
  {
  try
    {
    RPSlot slot("hand");
    
    slot.get(13123);
    
    CPPUNIT_FAIL("It should have thrown exception");
    }
  catch(ObjectNotFoundException e)
    {
    CPPUNIT_ASSERT(true);
    }

  try
    {
    RPSlot slot("hand");
    
    slot.remove(13123);
    
    CPPUNIT_FAIL("It should have thrown exception");
    }
  catch(ObjectNotFoundException e)
    {
    CPPUNIT_ASSERT(true);
    }
  }
  
void Test_RPSlot::testSerialization()
  {
  Serializer s;
  RPSlot slot("name");
  
  RPObject object;
  object.put("id",100);
  object.put("test","name");
  
  slot.add(object);
  slot.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  
  RPSlot value;
  value.read(s);
  
  CPPUNIT_ASSERT_EQUAL(slot,value);  
  }
  
  

