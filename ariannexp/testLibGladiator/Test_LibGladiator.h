/* $Id: Test_LibGladiator.h,v 1.4 2004/05/31 13:54:13 arianne_rpg Exp $ */
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
#ifndef __TEST_LibGladiator_H__
#define __TEST_LibGladiator_H__

#include <cppunit/extensions/HelperMacros.h>
#include "../libGladiator/libGladiator.h"

class Test_LibGladiator : public CPPUNIT_NS::TestFixture 
  {
  CPPUNIT_TEST_SUITE( Test_LibGladiator );
  CPPUNIT_TEST( testNormal );
  CPPUNIT_TEST_SUITE_END();
  
  public:
  Test_LibGladiator();

  void testNormal();
  };
/*
class Test_LibFramework : public CPPUNIT_NS::TestFixture 
  {
  CPPUNIT_TEST_SUITE( Test_LibFramework );
  CPPUNIT_TEST( testNormal );
  CPPUNIT_TEST_SUITE_END();
  
  public:
  Test_LibFramework();

  void testNormal();
  };
*/  
#endif

