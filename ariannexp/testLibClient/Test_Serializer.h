/* $Id: Test_Serializer.h,v 1.5 2004/05/03 21:53:40 arianne_rpg Exp $ */
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
#ifndef __TEST_Serializer_H__
#define __TEST_Serializer_H__

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

#include "../libClient/Serializer.h"

class Test_Serializer : public CPPUNIT_NS::TestFixture 
  {
  CPPUNIT_TEST_SUITE( Test_Serializer );
  CPPUNIT_TEST( testUint8 );
  CPPUNIT_TEST( testUint16 );
  CPPUNIT_TEST( testUint32 );
  CPPUNIT_TEST( testString );
  CPPUNIT_TEST( testListAndVector );
  CPPUNIT_TEST( testToVector );
  CPPUNIT_TEST_SUITE_END();
  
  public:
  Test_Serializer();

  void testUint8();
  void testUint16();
  void testUint32();
  void testString();
  void testListAndVector();
  void testToVector();
  };

#endif
