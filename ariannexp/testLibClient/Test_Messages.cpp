/* $Id: Test_Messages.cpp,v 1.7 2004/04/03 17:42:51 arianne_rpg Exp $ */
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
#include "Test_Messages.h"

using namespace std;
using namespace arianne;

Test_Messages::Test_Messages()
  {
  }

void Test_Messages::testC2SAction()
  {
  Serializer s;
  
  RPAction action;
  action.put("test","value");
  action.put("!hidden","attrib");
  
  MessageC2SAction message;
  message.action=action;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageC2SAction value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  CPPUNIT_ASSERT_EQUAL(message.action,value.action);
  }

void Test_Messages::testC2SChooseCharacter()
  {
  Serializer s;
  
  MessageC2SChooseCharacter message;
  message.character="miguel";
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageC2SChooseCharacter value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  CPPUNIT_ASSERT_EQUAL(message.character,value.character);
  }

void Test_Messages::testC2SLogin()
  {
  Serializer s;
  
  MessageC2SLogin message;
  message.username="miguel";
  message.password="pazzworz";
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageC2SLogin value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  CPPUNIT_ASSERT_EQUAL(message.username,value.username);
  CPPUNIT_ASSERT_EQUAL(message.password,value.password);
  }

void Test_Messages::testC2SLogout()
  {
  Serializer s;
  
  MessageC2SLogout message;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageC2SLogout value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  }

void Test_Messages::testC2SPerceptionACK()
  {
  Serializer s;
  
  MessageC2SPerceptionACK message;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageC2SPerceptionACK value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  }

void Test_Messages::testS2CActionACK()
  {
  Serializer s;
  
  MessageS2CActionACK message;
  message.actionid=1231;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CActionACK value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  CPPUNIT_ASSERT_EQUAL(message.actionid,value.actionid);
  }

void Test_Messages::testS2CCharacterList()  
  {
  Serializer s;
  
  list<string> characters;
  characters.push_back("miguel");
  characters.push_back("arianne_rpg");
  characters.push_back("mblanch");
  
  MessageS2CCharacterList message;
  message.characters=characters;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CCharacterList value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  CPPUNIT_ASSERT(message.characters==value.characters);
  }

void Test_Messages::testS2CChooseCharacterACK()  
  {
  Serializer s;
  
  MessageS2CChooseCharacterACK message;
  message.id=12342;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CChooseCharacterACK value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  CPPUNIT_ASSERT_EQUAL(message.id,value.id);
  }

void Test_Messages::testS2CChooseCharacterNACK()  
  {
  Serializer s;
  
  MessageS2CChooseCharacterNACK message;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CChooseCharacterNACK value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  }

void Test_Messages::testS2CLoginACK()  
  {
  Serializer s;
  
  MessageS2CLoginACK message;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CLoginACK value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  }

void Test_Messages::testS2CLoginNACK()  
  {
  Serializer s;
  
  MessageS2CLoginNACK message;
  message.reason=0;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CLoginNACK value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  CPPUNIT_ASSERT_EQUAL(message.reason,value.reason);
  }

void Test_Messages::testS2CLogoutACK()  
  {
  Serializer s;
  
  MessageS2CLogoutACK message;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CLogoutACK value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  }

void Test_Messages::testS2CLogoutNACK()  
  {
  Serializer s;
  
  MessageS2CLogoutNACK message;
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CLogoutNACK value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  }

void Test_Messages::testS2CPerception()  
  {
  Serializer s;
  
  RPObject miguel;
  miguel.put("name","miguel");
  miguel.put("id",1213);
  miguel.addSlot(RPSlot("r_hand"));
  miguel.addSlot(RPSlot("l_hand"));
    
  MessageS2CPerception message;
  message.typePerception=0;
  message.addedObjects.push_back(miguel);
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CPerception value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  CPPUNIT_ASSERT(message.typePerception==value.typePerception);
  CPPUNIT_ASSERT(message.addedObjects==value.addedObjects);
  CPPUNIT_ASSERT(message.deletedObjects==value.deletedObjects);
  }

void Test_Messages::testS2CServerInfo()  
  {
  Serializer s;
    
  MessageS2CServerInfo message;
  message.contents.push_back("game=good");
  message.contents.push_back("price=free");
  message.contents.push_back("license=GPL");
  
  message.write(s);
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  MessageS2CServerInfo value;
  
  value.read(s);  
  CPPUNIT_ASSERT_EQUAL(message.type,value.type);
  CPPUNIT_ASSERT_EQUAL(message.clientid,value.clientid);
  CPPUNIT_ASSERT(message.contents==value.contents);
  }

  
