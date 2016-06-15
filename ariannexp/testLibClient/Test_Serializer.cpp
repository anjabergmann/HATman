/* $Id: Test_Serializer.cpp,v 1.4 2004/01/26 08:27:57 arianne_rpg Exp $ */
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
#include "Test_Serializer.h"

using namespace std;
using namespace arianne;

Test_Serializer::Test_Serializer()
  {
  }

void Test_Serializer::testUint8()
  {
  Serializer s;
  s << (Uint8)255;
  s << (Uint8)145;
  s << (Uint8)128;
  s << (Uint8)55;
  s << (Uint8)0;
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  Uint8 value;
  
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint8)255);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint8)145);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint8)128);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint8)55);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint8)0);
  }
  
void Test_Serializer::testUint16()
  {
  Serializer s;
  s << (Uint16)65535;
  s << (Uint16)45463;
  s << (Uint16)32768;
  s << (Uint16)14123;
  s << (Uint16)0;
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  Uint16 value;
  
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint16)65535);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint16)45463);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint16)32768);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint16)14123);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint16)0);
  }
  
void Test_Serializer::testUint32()
  {
  Serializer s;
  s << (Uint32)4294967295;
  s << (Uint32)3243252341;
  s << (Uint32)2147483648;
  s << (Uint32)1412426978;
  s << (Uint32)0;
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  Uint32 value;
  
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint32)4294967295);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint32)3243252341);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint32)2147483648);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint32)1412426978);
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,(Uint32)0);
  }
  
void Test_Serializer::testString()
  {
  Serializer s;
  s << "";
  s << "Hi world";
  s << "Writing test cases is a fun thing, you should try it";
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  string value;
  
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,string(""));
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,string("Hi world"));
  result >> value;  
  CPPUNIT_ASSERT_EQUAL(value,string("Writing test cases is a fun thing, you should try it"));  
  }

void Test_Serializer::testListAndVector()
  {
  Serializer s;
  
  list<string> contents;
  contents.push_back("Hi");
  contents.push_back("world");
  contents.push_back("this");
  contents.push_back("is");
  contents.push_back("a");
  contents.push_back("test");
  
  s << contents;
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  list<string> value;
  
  result >> value;  
  CPPUNIT_ASSERT(value==contents);

  vector<string> contentsVector;
  contentsVector.push_back("Hi");
  contentsVector.push_back("world");
  contentsVector.push_back("this");
  contentsVector.push_back("is");
  contentsVector.push_back("a");
  contentsVector.push_back("test");
  
  s << contentsVector;
  
  s.toVector(data);
  
  Serializer resultVector(data);
  vector<string> valueVector;
  
  resultVector >> valueVector;  
  CPPUNIT_ASSERT(valueVector==contentsVector);
  }
  
void Test_Serializer::testToVector()
  {
  Serializer s;
  s << (Uint32)12167442;
  s << "Hi world";
  s << (Uint16)12456;
  
  vector<Uint8> data;
  s.toVector(data);
  
  Serializer result(data);
  Uint32 value_1;
  string value_2;
  Uint16 value_3;
  
  result >> value_1;  
  CPPUNIT_ASSERT_EQUAL(value_1,(Uint32)12167442);
  result >> value_2;  
  CPPUNIT_ASSERT_EQUAL(value_2,string("Hi world"));
  result >> value_3;  
  CPPUNIT_ASSERT_EQUAL(value_3,(Uint16)12456);  
  }
