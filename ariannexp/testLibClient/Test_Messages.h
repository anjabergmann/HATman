/* $Id: Test_Messages.h,v 1.6 2004/05/03 21:53:40 arianne_rpg Exp $ */
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
#ifndef __TEST_Messages_H__
#define __TEST_Messages_H__

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

#include "../libClient/Serializer.h"
#include "../libClient/Message.h"
#include "../libClient/C2SMessages.h"
#include "../libClient/S2CMessages.h"

class Test_Messages : public CPPUNIT_NS::TestFixture 
  {
  CPPUNIT_TEST_SUITE( Test_Messages );
  CPPUNIT_TEST( testC2SAction );
  CPPUNIT_TEST( testC2SChooseCharacter );
  CPPUNIT_TEST( testC2SLogin );
  CPPUNIT_TEST( testC2SLogout );
  CPPUNIT_TEST( testC2SPerceptionACK );
  CPPUNIT_TEST( testS2CActionACK );  
  CPPUNIT_TEST( testS2CCharacterList );  
  CPPUNIT_TEST( testS2CChooseCharacterACK );  
  CPPUNIT_TEST( testS2CChooseCharacterNACK );  
  CPPUNIT_TEST( testS2CLoginACK );  
  CPPUNIT_TEST( testS2CLoginNACK );  
  CPPUNIT_TEST( testS2CLogoutACK );  
  CPPUNIT_TEST( testS2CLogoutNACK );  
  CPPUNIT_TEST( testS2CPerception );  
  CPPUNIT_TEST( testS2CServerInfo );  
  CPPUNIT_TEST_SUITE_END();
  
  public:
  Test_Messages();

  void testC2SAction();
  void testC2SChooseCharacter();
  void testC2SLogin();
  void testC2SLogout();
  void testC2SPerceptionACK();

  void testS2CActionACK();  
  void testS2CCharacterList();  
  void testS2CChooseCharacterACK();  
  void testS2CChooseCharacterNACK();  
  void testS2CLoginACK();  
  void testS2CLoginNACK();  
  void testS2CLogoutACK();  
  void testS2CLogoutNACK();  
  void testS2CPerception();  
  void testS2CServerInfo();  
  };

#endif
