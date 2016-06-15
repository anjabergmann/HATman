/* $Id: main.cpp,v 1.3 2004/05/31 13:54:13 arianne_rpg Exp $ */
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

#include "Test_LibGladiator.h"

#include "SDL.h"

int main(int argc, char** argv)
  {
  CPPUNIT_NS::TextUi::TestRunner runner;

  runner.addTest( Test_LibGladiator::suite() );

  bool wasSuccessful = runner.run();
  return wasSuccessful ? 0 : 1;
  }
