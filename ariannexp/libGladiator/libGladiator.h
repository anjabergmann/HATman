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
#ifndef __Lib_Gladiator_h
#define __Lib_Gladiator_h

#ifdef WIN32
  #define BBDECL extern "C" _declspec(dllexport)
  #define BBCALL _stdcall
#else
  #define BBDECL 
  #define BBCALL
#endif

BBDECL void BBCALL trace(char *event,char *text);

BBDECL const char * BBCALL ConnectToMarauroaServer(char *serverName, int port, char *username, char *password);
BBDECL int BBCALL AvailableCharacters()  ;

BBDECL void BBCALL ResetCharacter();
BBDECL int BBCALL HasNextCharacter();
BBDECL char * BBCALL NextCharacter();

BBDECL char * BBCALL ChooseCharacter(char *character);
BBDECL int BBCALL HasNewPerception();
BBDECL char * BBCALL DisconnectFromMarauroaServer();


/** Proposed interface: Proposed 0.1
BBDECL int BBCALL CreateRPObjectIterator();
BBDECL void BBCALL DeleteRPObjectIterator(int iterator);

BBDECL void BBCALL ResetRPObjectIterator(int iterator);
BBDECL int BBCALL HasNextRPObjectIterator(int iterator);
BBDECL void BBCALL NextRPObjectIterator(int iterator);

BBDECL char * BBCALL RPObjectGet(int iterator,char *attribute);
BBDECL int BBCALL RPObjectHas(int iterator,char *attribute);
BBDECL void BBCALL RPObjectPut(int iterator,char *attribute, char *value);

BBDECL void BBCALL RPObjectResetSlotIterator(int iterator);
BBDECL int BBCALL RPObjectHasNextSlotIterator(int iterator);
BBDECL void BBCALL RPObjectNextSlotIterator(int iterator);
*/
/** Old interface */


BBDECL char * BBCALL ArenaGet(char *attribute);
BBDECL void BBCALL ArenaPut(char *attribute, char *value);

BBDECL void BBCALL ArenaResetGladiator();
BBDECL int BBCALL ArenaHasNextGladiator();
BBDECL void BBCALL ArenaNextGladiator();
BBDECL int BBCALL ArenaFindGladiator(int id);

BBDECL char * BBCALL GladiatorGet(char *attribute);
BBDECL int BBCALL GladiatorHas(char *attribute);
BBDECL void BBCALL GladiatorPut(char *attribute, char *value);

BBDECL int BBCALL Player();
BBDECL int BBCALL NumPlayers();
BBDECL void BBCALL ResetPlayer();
BBDECL int BBCALL HasNextPlayer();
BBDECL void BBCALL NextPlayer();
BBDECL int BBCALL FindPlayer(int id);

BBDECL char * BBCALL PlayerGet(char *attribute);
BBDECL int BBCALL PlayerHas(char *attribute);
BBDECL void BBCALL PlayerPut(char *attribute, char *value);

BBDECL int BBCALL PlayerHasSlot(char *name);
BBDECL int BBCALL PlayerSetSlot(char *name);

BBDECL void BBCALL SlotResetRPObject();
BBDECL int BBCALL SlotHasNextRPObject();
BBDECL void BBCALL SlotNextRPObject();
BBDECL int BBCALL SlotFindRPObject(unsigned int id);

BBDECL char * BBCALL RPObjectGet(char *attribute);
BBDECL int BBCALL RPObjectHas(char *attribute);
BBDECL void BBCALL RPObjectPut(char *attribute, char *value);

BBDECL int BBCALL actionRequestFight(int wait_reply);
BBDECL int BBCALL actionFightMode(int wait_reply, char *mode);
BBDECL int BBCALL actionVote(int wait_reply, char *vote);
BBDECL int BBCALL actionChat(int wait_reply, char *text);

void printWorld();

#endif
