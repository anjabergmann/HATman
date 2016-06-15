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
#ifndef __Lib_Framework_h
#define __Lib_Framework_h

#ifdef WIN32
  #define BBDECL _declspec(dllexport)
  #define BBCALL _stdcall
#else
  #define BBDECL 
  #define BBCALL
#endif

#include "../libClient/RPObject.h"
#include "../libClient/RPAction.h"

using namespace arianne;

#include <string>
#include <vector>
#include <list>

#include "SDL.h"


typedef bool (*CALLBACKFUNC)(void *);

extern CALLBACKFUNC callback;
extern void *pythonCallbackFunction;

struct World
  {
  std::map<Uint32,arianne::RPObject> objects;  
  };  
  
struct Perception 
  {
  static const Uint8 SYNC=1;
  static const Uint8 DELTA=0;

  Uint8 type;
  Uint32 timestamp;
  std::list<arianne::RPObject> added;
  std::list<arianne::RPObject> modifiedAdded;
  std::list<arianne::RPObject> modifiedDeleted;
  std::list<arianne::RPObject> deleted;

  bool myRPObjectChanged;
  arianne::RPObject myRPObject;
  };

BBDECL void BBCALL setIdleCFunction(CALLBACKFUNC function);

BBDECL void BBCALL connectToArianne(char *serverName, int port);
BBDECL bool BBCALL login(char *username, char *password);
BBDECL std::list<std::string> BBCALL availableCharacters();
BBDECL std::list<std::string> BBCALL serverInfo();
BBDECL std::string BBCALL getGameType();
BBDECL bool BBCALL chooseCharacter(std::string character);
BBDECL Uint32 BBCALL ourRPObjectID();

BBDECL bool BBCALL hasRPMap();
BBDECL std::list<arianne::RPObject> BBCALL getRPMap();

BBDECL bool BBCALL hasPerception();
BBDECL Perception BBCALL getPerception();

BBDECL void BBCALL send(arianne::RPAction action);
BBDECL Uint32 BBCALL sendBlocking(arianne::RPAction action);

BBDECL void BBCALL outOfSync();
BBDECL bool BBCALL logout();

BBDECL bool BBCALL iserror();
BBDECL Uint32 BBCALL errorCode();
BBDECL std::string BBCALL errorReason();

#endif
