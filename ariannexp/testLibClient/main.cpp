/* $Id: main.cpp,v 1.6 2004/01/26 08:27:57 arianne_rpg Exp $ */
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
#include <cppunit/ui/text/TestRunner.h>

#include "Test_Serializer.h"
#include "Test_Attributes.h"
#include "Test_RPSlot.h"
#include "Test_RPObject.h"
#include "Test_Messages.h"
#include "Test_NetworkClientManager.h"


#include "SDL.h"

int main(int argc, char** argv)
  {
  CPPUNIT_NS::TextUi::TestRunner runner;

  runner.addTest( Test_Serializer::suite() );
  runner.addTest( Test_Attributes::suite() );
  runner.addTest( Test_RPSlot::suite() );
  runner.addTest( Test_RPObject::suite() );
  runner.addTest( Test_Messages::suite() );
  runner.addTest( Test_NetworkClientManager::suite() );

  bool wasSuccessful = runner.run();
  return wasSuccessful ? 0 : 1;
  }
