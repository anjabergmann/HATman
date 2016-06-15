/* $Id: Test_LibGladiator.cpp,v 1.6 2004/05/31 13:54:13 arianne_rpg Exp $ */
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
#include "Test_LibGladiator.h"
#include <SDL.h>
#include <iostream>

using namespace std;

Test_LibGladiator::Test_LibGladiator()
  {
  }
  
void Test_LibGladiator::testNormal()
  {
  cout << "Trying to connect to server..." << endl;
  string result=ConnectToMarauroaServer("127.0.0.1",3214,"Another Test Player","Test Password");
  if(result!="OK")
    {
    CPPUNIT_FAIL(result);
    }
  else
    {
    CPPUNIT_ASSERT(AvailableCharacters()>0);
    ResetCharacter();    
    while(HasNextCharacter()==1)
      {
      CPPUNIT_ASSERT(NextCharacter()!="");
      }
    
    cout << "Choosing character..." << endl;
    result=ChooseCharacter("MrBean");
    CPPUNIT_ASSERT_EQUAL(result,string("OK"));
    
    cout << "Starting perception loop..." << endl;
    Uint32 startTime=SDL_GetTicks();
    bool enter=false;
    
    while(SDL_GetTicks()<startTime+1000000)
      {
      int iresult=HasNewPerception();
      cout << SDL_GetTicks() << " - " << iresult << endl;
      if(iresult==1)
        {
        cout << "Number of Players: " << NumPlayers() << endl;
        
        ResetPlayer();      
        while(HasNextPlayer()==1)
          {        
          cout << "Player name: " << PlayerGet("name") << endl;
          NextPlayer();
          }        
        
        if(Player()==1)
          {  
          if(PlayerHas("fighting")==false)
            {
            actionRequestFight(1);
            }
          }
        }
      
      SDL_Delay(1000);
      }    
    
    DisconnectFromMarauroaServer();
    } 
  }
/*  

Test_LibFramework::Test_LibFramework()
  {
  }

void donothing(void *data)
  {
  }
  
void Test_LibFramework::testNormal()
  {
  setIdleCFunction(donothing);

  connectToArianne("127.0.0.1",3214);
  if(login("miguel","qwerty"))
    {
    list<string> chars=availableCharacters();
    string character=chars.front();
    if(chooseCharacter(character))
      {
      int i=0;
      while(i<10)
        {
        if(hasPerception())
          {
          Perception perception=getPerception();
          i++;
          }
        }
      
      logout();
      }
    }
  }

*/
