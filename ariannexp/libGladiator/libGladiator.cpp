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
#include "../libClient/NetworkClientManager.h"
#include "../libClient/Trace.h"
#include "libGladiator.h"

#include <fstream>
#include <sstream>

using namespace arianne;
using namespace std;

/* Global definitions */
NetworkClientManager *networkClient=0;
list<string> serverInfo;
list<string> characterList;
list<string>::iterator characterListIterator;
int playerID=-1;
bool recievedFirstTotalPerception=false;
  
struct World
  {
  RPObject arena;
  map<int,RPObject> players;
  };
  
World world;  

const int MAX_NUM_ITERATIONS=10;

BBDECL void BBCALL trace(char *event,char *text)
  {
  global_trace.add("Blitz",event,text);
  }

char *returnValue_ConnectToMarauroaServer=0;

BBDECL const char * BBCALL ConnectToMarauroaServer(char *serverName,int port, char *username, char *password)
  {
  global_trace.add("ConnectToMarauroaServer",">");
  if(returnValue_ConnectToMarauroaServer)
    {
    delete[] returnValue_ConnectToMarauroaServer;    
    returnValue_ConnectToMarauroaServer=0;
    }

  try
    {
    global_trace.add("ConnectToMarauroaServer","D","Creating NetworkClientManager");
    networkClient=new NetworkClientManager(serverName,port);
  
    global_trace.add("ConnectToMarauroaServer","D","Sending Login Message");
    MessageC2SLogin messageLogin;
    messageLogin.username=username;
    messageLogin.password=password;  
    networkClient->addMessage(messageLogin);
    
    int messagesToRecieve=3;
    int j=0;
    while(j<MAX_NUM_ITERATIONS && messagesToRecieve!=0)
      {
      arianne::Message *msg=networkClient->getMessage();
      if(msg && msg->type==arianne::TYPE_S2C_LOGIN_NACK)
        {
        global_trace.add("ConnectToMarauroaServer","D","Recieved a Login NACK message");
        MessageS2CLoginNACK *msgLoginNACK=(MessageS2CLoginNACK *)msg;        
        string result="ERROR: "+ msgLoginNACK->toString();
        returnValue_ConnectToMarauroaServer=new char[result.size()+1];
        strcpy(returnValue_ConnectToMarauroaServer,result.c_str());  

	    global_trace.add("ConnectToMarauroaServer","<");
        return returnValue_ConnectToMarauroaServer;              
        }
      else if(msg && msg->type==arianne::TYPE_S2C_LOGIN_ACK)
        {
        global_trace.add("ConnectToMarauroaServer","D","Recieved a Login ACK message");
        --messagesToRecieve;
        }
      else if(msg && msg->type==arianne::TYPE_S2C_SERVERINFO)
        {
        global_trace.add("ConnectToMarauroaServer","D","Recieved a Server Info message");
        MessageS2CServerInfo *msgServerInfo=(MessageS2CServerInfo*)msg;
        serverInfo=msgServerInfo->contents;
        --messagesToRecieve;        
        }
      else if(msg && msg->type==arianne::TYPE_S2C_CHARACTERLIST)
        {
        global_trace.add("ConnectToMarauroaServer","D","Recieved a Character List message");
        MessageS2CCharacterList *msgCharList=(MessageS2CCharacterList*)msg;
        characterList=msgCharList->characters;
        --messagesToRecieve;        
        }
      else
        {      
        global_trace.add("ConnectToMarauroaServer","D","No message recieved");
        j++;
        SDL_Delay(750);
        }
       
      if(msg)
        {  
        delete msg;
        msg=0;
        }
      }
      
    if(j==MAX_NUM_ITERATIONS)
      {
      global_trace.add("ConnectToMarauroaServer","E","Maximum number of iterations reached, connection aborted.");
      global_trace.add("ConnectToMarauroaServer","<");
      return "ERROR: Can't reach server";
      }
    else
      {      
      global_trace.add("ConnectToMarauroaServer","D","Connected correctly");
      global_trace.add("ConnectToMarauroaServer","<");
      return "OK";
      }
    }
  catch(InvalidVersionException e)
    {
    global_trace.add("ConnectToMarauroaServer","X",string("Exception happened: ")+e.what());
    global_trace.add("ConnectToMarauroaServer","<");
    return "ERROR: You are running an out-of-date version";
    }
  catch(exception e)
    {
    global_trace.add("ConnectToMarauroaServer","X",string("Exception happened: ")+e.what());
    global_trace.add("ConnectToMarauroaServer","<");
    return "ERROR: Exception caught";
    }
  catch(...)
    {
    global_trace.add("ConnectToMarauroaServer","X","Unknown Exception happened");
    global_trace.add("ConnectToMarauroaServer","<");
    return "ERROR: Very bad Exception caught";
    }
  }
  
BBDECL int BBCALL AvailableCharacters()  
  {
  global_trace.add("AvailableCharacters",">");
  global_trace.add("AvailableCharacters","<");
  return (int)characterList.size();
  }

BBDECL void BBCALL ResetCharacter()
  {
  global_trace.add("ResetCharacter",">");
  characterListIterator=characterList.begin();
  global_trace.add("ResetCharacter","<");
  }
  
BBDECL int BBCALL HasNextCharacter()
  {
  global_trace.add("HasNextCharacter",">");
  if(characterListIterator==characterList.end())
    {
    global_trace.add("HasNextCharacter","D","There are ANY characters left");
    global_trace.add("HasNextCharacter","<");
    return 0;
    }
  else
    {
    global_trace.add("HasNextCharacter","D","There are MORE characters");
    global_trace.add("HasNextCharacter","<");
    return 1;
    }
  }
  
char *returnValue_NextCharacter=0;

BBDECL char * BBCALL NextCharacter()
  {
  global_trace.add("NextCharacter",">");
  if(characterListIterator==characterList.end())
    {
    global_trace.add("NextCharacter","E","You are reading beyond the limit");
    global_trace.add("NextCharacter","<");
    return "";
    }
  else
    {
    if(returnValue_NextCharacter)
      {
      delete[] returnValue_NextCharacter; 
      returnValue_NextCharacter=0;
      }
    
    string& character=*characterListIterator; 
    returnValue_NextCharacter=new char[character.size()+1];
    strcpy(returnValue_NextCharacter,character.c_str());  
  
    ++characterListIterator;
    global_trace.add("NextCharacter","<");
    return returnValue_NextCharacter;
    }    
  }

BBDECL char * BBCALL ChooseCharacter(char *character)
  {
  global_trace.add("ChooseCharacter",">");
  try
    {
    global_trace.add("ChooseCharacter","D","Sending Choose Character Message");
    MessageC2SChooseCharacter msgChooseCharacter;
    msgChooseCharacter.character=character;
    networkClient->addMessage(msgChooseCharacter);

    int j=0;
    while(j<MAX_NUM_ITERATIONS)
      {
      arianne::Message *msg=networkClient->getMessage();
      if(msg && msg->type==arianne::TYPE_S2C_CHOOSECHARACTER_NACK)
        {
        global_trace.add("ChooseCharacter","D","Recieved Choose Character NACK Message");
        delete msg;
        msg=0;
        
        global_trace.add("ChooseCharacter","<");
        return "ERROR: No such character";              
        }
      else if(msg && msg->type==arianne::TYPE_S2C_CHOOSECHARACTER_ACK)
        {
        global_trace.add("ChooseCharacter","D","Recieved Choose Character ACK Message");
        MessageS2CChooseCharacterACK *msgCCharACK=(MessageS2CChooseCharacterACK*)msg;
        playerID=msgCCharACK->id;
        
        delete msg;
        msg=0;
        
        global_trace.add("ChooseCharacter","<");
        return "OK";              
        }
      else
        {      
        if(msg)
          {
          delete msg;
          msg=0;
          }
          
        j++;
        SDL_Delay(750);
        }
      }
      
    if(j==MAX_NUM_ITERATIONS)
      {
      global_trace.add("ChooseCharacter","D","No messages or invalid ones recieved from server");
      global_trace.add("ChooseCharacter","<");
      return "No message or invalid one recieved from server";
      }
    }
  catch(exception e)
    {
    global_trace.add("ChooseCharacter","X",string("Exception happened: ")+e.what());
    global_trace.add("ChooseCharacter","<");
    return "ERROR: Exception caught";
    }
  catch(...)
    {
    global_trace.add("ChooseCharacter","X","Unknown Exception happened");
    global_trace.add("ChooseCharacter","<");
    return "ERROR: Very bad Exception caught";
    }
  
  return "ERROR";
  }

bool totalperception_first_time=true;

BBDECL int BBCALL IsSyncedWithServer()
  {
  if(totalperception_first_time) 
    {
    return 0;
    }
  else
    {
    return 1;
    }
  }


void handleNewPerception(MessageS2CPerception *msgPerception)
  {
  if(msgPerception->typePerception==TOTAL_PERCEPTION)
    {
    totalperception_first_time=false;
    global_trace.add("HasNewPerception","D","TOTAL Perception");
    recievedFirstTotalPerception=true;
    world.players.clear();
    
    list<RPObject>::iterator it=msgPerception->addedObjects.begin();
    while(it!=msgPerception->addedObjects.end())
      {
      string type=it->get("type");
      if(type=="character")
        {
        int id=it->getInt("id");
        world.players.insert(pair<int,RPObject>(id,*it));
        }
      else if(type=="arena")
        {
        world.arena=*it;
        }
        
      ++it;
      }
    
    global_trace.add("HasNewPerception","D","TOTAL Perception: Reseting iterators");
    ArenaResetGladiator();
    ResetPlayer();
    }
  else if(!totalperception_first_time)
    {
    global_trace.add("HasNewPerception","D","DELTA Perception");
    list<RPObject>::iterator it=msgPerception->deletedObjects.begin();
    global_trace.add("HasNewPerception","D","Deleted Objects");
    while(it!=msgPerception->deletedObjects.end())
      {
      global_trace.add("HasNewPerception","D","  "+it->toString());
      Uint32 id=it->getInt("id");
      world.players.erase(id);
      ++it;
      }

    it=msgPerception->modifiedDeletedObjects.begin();
    global_trace.add("HasNewPerception","D","Modified Deleted Objects");
    while(it!=msgPerception->modifiedDeletedObjects.end())
      {
      global_trace.add("HasNewPerception","D","  "+it->toString());
      Uint32 id=it->getInt("id");
      if(world.arena.getInt("id")==id)
        {
        world.arena.applyDifferences(0,&(*it));
        }
      else
        {
        world.players[id].applyDifferences(0,&(*it));
        }
      ++it;
      }

    it=msgPerception->modifiedAddedObjects.begin();
    global_trace.add("HasNewPerception","D","Modified Added Objects");
    while(it!=msgPerception->modifiedAddedObjects.end())
      {
      global_trace.add("HasNewPerception","D","  "+it->toString());
      Uint32 id=it->getInt("id");
      if(world.arena.getInt("id")==id)
        {
        world.arena.applyDifferences(&(*it),0);
        }
      else
        {
        world.players[id].applyDifferences(&(*it),0);
        }
      ++it;
      }    
    
    it=msgPerception->addedObjects.begin();
    global_trace.add("HasNewPerception","D","Added Objects");
    while(it!=msgPerception->addedObjects.end())
      {
      string type=it->get("type");
      if(type=="character")
        {
        int id=it->getInt("id");
        world.players[id]=*it;
        }
      else if(type=="arena")
        {
        world.arena=*it;
        }
        
      ++it;
      }

    global_trace.add("HasNewPerception","D","DELTA Perception: Reseting iterators");
    ArenaResetGladiator();
    ResetPlayer();
    }

  /* We update the player to its full version */
  if(msgPerception->myRPObject.size()>0)
    {
    int id=msgPerception->myRPObject.getInt("id");
    world.players[id]=msgPerception->myRPObject;
    }
  
  printWorld();
  }
  
void printWorld()
  {
  global_trace.add("World","D",world.arena.toString());
  
  std::map<int,arianne::RPObject>::iterator it=world.players.begin();
  for(;it!=world.players.end();++it)
    {
    global_trace.add("World","D",it->second.toString());
    }  
  }

BBDECL int BBCALL HasNewPerception()
  {
  global_trace.add("HasNewPerception",">");
  
  try
    {
    arianne::Message *msg=networkClient->getMessage();
    if(msg && msg->type==arianne::TYPE_S2C_PERCEPTION)
      {
      global_trace.add("HasNewPerception","D","Recieved Perception Message");
      MessageS2CPerception *msgPerception=(MessageS2CPerception*)msg;
      
      handleNewPerception(msgPerception);
      
      global_trace.add("HasNewPerception","D","Sending Perception ACK Message");
      MessageC2SPerceptionACK msgPACK;
      networkClient->addMessage(msgPACK);      
      
      delete msg;
      msg=0;

      global_trace.add("HasNewPerception","<");
      return 1;              
      }
    else
      {
      if(msg)
        {
        delete msg;
        msg=0;
        }
        
      global_trace.add("HasNewPerception","<");
      return 0;
      }
    }
  catch(exception e)
    {
    global_trace.add("HasNewPerception","X",string("Exception happened: ")+e.what());
    global_trace.add("HasNewPerception","<");
    return 0;
    }
  catch(char *e)
    {
    global_trace.add("HasNewPerception","X",string("Exception happened: ")+e);
    global_trace.add("HasNewPerception","<");
    return 0;
    }
  catch(...)
    {
    global_trace.add("HasNewPerception","X","Unknown Exception happened");
    global_trace.add("HasNewPerception","<");
    return 0;
    }
  }

BBDECL char * BBCALL DisconnectFromMarauroaServer()
  {
  global_trace.add("DisconnectFromMarauroaServer",">");
  try
    {
    global_trace.add("DisconnectFromMarauroaServer","D","Sending Logout Message");
    MessageC2SLogout msgLogout;
    networkClient->addMessage(msgLogout);

    int j=0;
    while(j<MAX_NUM_ITERATIONS)
      {
      arianne::Message *msg=networkClient->getMessage();
      if(msg && msg->type==arianne::TYPE_S2C_LOGOUT_NACK)
        {
        global_trace.add("DisconnectFromMarauroaServer","D","Received Logout NACK");
        delete msg;
        msg=0;
        return "ERROR: You can't logout now";              
        }
      else if(msg && msg->type==arianne::TYPE_S2C_LOGOUT_ACK)
        {
        global_trace.add("DisconnectFromMarauroaServer","D","Received Logout ACK");
        delete msg;
        msg=0;
        return "OK";              
        }
      else
        {
        if(msg)
          {
          delete msg;
          msg=0;
          }
          
        j++;
        SDL_Delay(750);
        }
      }

    if(j==MAX_NUM_ITERATIONS)
      {
      global_trace.add("ChooseCharacter","D","No messages or invalid ones recieved from server");
      global_trace.add("ChooseCharacter","<");
      return "No message or invalid one recieved from server";
      }
    }
  catch(exception e)
    {
    global_trace.add("ChooseCharacter","X",string("Exception happened: ")+e.what());
    global_trace.add("ChooseCharacter","<");
    return "ERROR: Exception caught";
    }
  catch(...)
    {
    global_trace.add("ChooseCharacter","X","Unknown Exception happened");
    global_trace.add("ChooseCharacter","<");
    return "ERROR: Very bad Exception caught";
    }

  return "ERROR";
  }

char *returnValue_ArenaGet=0;

BBDECL char * BBCALL ArenaGet(char *attribute)
  {
  global_trace.add("ArenaGet",">");
  
  if(world.arena.has(attribute))
    {
    global_trace.add("ArenaGet","D",string("Attribute ")+attribute+" exits");
    if(returnValue_ArenaGet)
      {
      delete[] returnValue_ArenaGet; 
      returnValue_ArenaGet=0;
      }
    
    string value=world.arena.get(attribute); 
    returnValue_ArenaGet=new char[value.size()+1];
    strcpy(returnValue_ArenaGet,value.c_str());  
    
    global_trace.add("ArenaGet","<");
    return returnValue_ArenaGet;
    }
  else
    {
    global_trace.add("ArenaGet","D",string("Attribute ")+attribute+" does NOT exit");
    global_trace.add("ArenaGet","<");
    return "";
    }
  }
  
BBDECL void BBCALL ArenaPut(char *attribute, char *value)
  {
  global_trace.add("ArenaPut",">");
  world.arena.put(attribute,value);
  global_trace.add("ArenaPut","<");
  }

std::map<Uint32,arianne::RPObject>::iterator arena_gladiators_it;  

BBDECL void BBCALL ArenaResetGladiator()
  {
  global_trace.add("ArenaResetGladiator",">");
  if(recievedFirstTotalPerception==false) 
    {
    global_trace.add("ArenaResetGladiator","E","There is no Total perception recieved, we can't reset Gladiator");
    global_trace.add("ArenaResetGladiator","<");
    return;
    }
    
  arena_gladiators_it=world.arena.getSlot("gladiators")->begin();
  global_trace.add("ArenaResetGladiator","<");
  }

BBDECL int BBCALL ArenaHasNextGladiator()
  {
  global_trace.add("ArenaHasNextGladiator",">");

  if(recievedFirstTotalPerception==false) 
    {
    global_trace.add("ArenaHasNextGladiator","E","There is no Total perception recieved, we can't reset Gladiator");
    global_trace.add("ArenaHasNextGladiator","<");
    return 0;
    }
    
  if(arena_gladiators_it!=world.arena.getSlot("gladiators")->end())
    {
    global_trace.add("ArenaHasNextGladiator","D","There are MORE characters");
    global_trace.add("ArenaHasNextGladiator","<");
    return 1;
    }
  else
    {
    global_trace.add("ArenaHasNextGladiator","D","There are ANY characters left");
    global_trace.add("ArenaHasNextGladiator","<");
    return 0;
    }  
  }

BBDECL void BBCALL ArenaNextGladiator()
  {
  global_trace.add("ArenaNextGladiator",">");
  if(arena_gladiators_it!=world.arena.getSlot("gladiators")->end())
    {
    ++arena_gladiators_it;
    }
  global_trace.add("ArenaNextGladiator","<");
  }
  
BBDECL int BBCALL ArenaFindGladiator(int id)
  {
  global_trace.add("ArenaFindGladiator",">");
  if(recievedFirstTotalPerception==false) 
    {
    global_trace.add("ArenaFindGladiator","E","There is no Total perception recieved, we can't reset Gladiator");
    global_trace.add("ArenaFindGladiator","<");
    return 0;
    }
    
  arena_gladiators_it=world.arena.getSlot("gladiators")->find(id);
  if(arena_gladiators_it!=world.arena.getSlot("gladiators")->end())
    {
    global_trace.add("ArenaFindGladiator","<");
    return 1;
    }
  else
    {
    global_trace.add("ArenaFindGladiator","<");
    return 0;
    }  
  }

char *returnValue_GladiatorsGet=0;
BBDECL char * BBCALL GladiatorGet(char *attribute)
  {
  global_trace.add("GladiatorGet",">");
  
  if(arena_gladiators_it->second.has(attribute))
    {
    if(returnValue_GladiatorsGet)
      {
      delete[] returnValue_GladiatorsGet; 
      returnValue_GladiatorsGet=0;
      }
    
    global_trace.add("GladiatorGet","D",string("Attribute ")+attribute+" exits");
    string value=arena_gladiators_it->second.get(attribute);; 
    returnValue_GladiatorsGet=new char[value.size()+1];
    strcpy(returnValue_GladiatorsGet,value.c_str());  

    global_trace.add("GladiatorGet","<");
    return returnValue_GladiatorsGet;
    }
  else
    {
    global_trace.add("GladiatorGet","D",string("Attribute ")+attribute+" does NOT exit");
    global_trace.add("GladiatorGet","<");
    return "";
    }
  }

BBDECL int BBCALL GladiatorHas(char *attribute)
  {
  global_trace.add("GladiatorHas",">");
  if(arena_gladiators_it->second.has(attribute))
    {
    global_trace.add("GladiatorHas","D","There are MORE gladiators");
    global_trace.add("GladiatorHas","<");
    return 1;
    }
  else
    {
    global_trace.add("GladiatorHas","D","There are ANY gladiators left");
    global_trace.add("GladiatorHas","<");
    return 0;
    }
  }

  
BBDECL void BBCALL GladiatorPut(char *attribute, char *value)
  {
  global_trace.add("GladiatorPut",">");
  arena_gladiators_it->second.put(attribute,value);
  global_trace.add("GladiatorPut","<");
  }

std::map<int,arianne::RPObject>::iterator world_players_it; 
  
BBDECL int BBCALL Player()
  {
  global_trace.add("Player",">");
  global_trace.add("Player","<");
  return FindPlayer(playerID);
  }

BBDECL int BBCALL NumPlayers()
  {
  global_trace.add("NumPlayers",">");
  global_trace.add("NumPlayers","<");
  return (int)world.players.size();
  }

BBDECL void BBCALL ResetPlayer()
  {
  global_trace.add("ResetPlayer",">");
  world_players_it=world.players.begin();
  global_trace.add("ResetPlayer","<");
  }

BBDECL int BBCALL HasNextPlayer()
  {
  global_trace.add("HasNextPlayer",">");
  if(world_players_it!=world.players.end())
    {
    global_trace.add("HasNextPlayer","D","There are MORE players");
    global_trace.add("HasNextPlayer","<");
    return 1;
    }
  else
    {
    global_trace.add("HasNextPlayer","D","There are ANY players left");
    global_trace.add("HasNextPlayer","<");
    return 0;
    }  
  }

BBDECL void BBCALL NextPlayer()
  {
  global_trace.add("NextPlayer",">");
  if(world_players_it!=world.players.end())
    {
    ++world_players_it;
    }
  global_trace.add("NextPlayer","<");  
  }
  
BBDECL int BBCALL FindPlayer(int id)
  {
  global_trace.add("FindPlayer",">");
  world_players_it=world.players.find(id);
  if(world_players_it!=world.players.end())
    {
    global_trace.add("FindPlayer","D","Player found");
    global_trace.add("FindPlayer","<");
    return 1;
    }
  else
    {
    global_trace.add("FindPlayer","D","Player NOT found");
    global_trace.add("FindPlayer","<");
    return 0;
    }  
  }
  
char *returnvalue_PlayerGet=0;

BBDECL char * BBCALL PlayerGet(char *attribute)
  {
  global_trace.add("PlayerGet",">");
  
  if(world_players_it->second.has(attribute))
    {
    if(returnvalue_PlayerGet)
      {
      delete[] returnvalue_PlayerGet; 
      returnvalue_PlayerGet=0;
      }
    
    string value=world_players_it->second.get(attribute);
    returnvalue_PlayerGet=new char[value.size()+1];
    strcpy(returnvalue_PlayerGet,value.c_str());  

    global_trace.add("PlayerGet","D",string("Attribute ")+attribute+" exits");
    global_trace.add("PlayerGet","<");
    return returnvalue_PlayerGet;
    }
  else
    {
    global_trace.add("PlayerGet","D",string("Attribute ")+attribute+" does NOT exit");
    global_trace.add("PlayerGet","<");
    return "";
    }
  }

BBDECL int BBCALL PlayerHas(char *attribute)
  {
  global_trace.add("PlayerHas",">");
  global_trace.add("PlayerHas","<");
  return (world_players_it->second.has(attribute)==true)?1:0;
  }
  
BBDECL void BBCALL PlayerPut(char *attribute, char *value)
  {
  global_trace.add("PlayerPut",">");
  world_players_it->second.put(attribute,value);
  global_trace.add("PlayerPut","<");
  }


BBDECL int BBCALL PlayerHasSlot(char *name)
  {
  if(world_players_it->second.hasSlot(name))
    {
    return 1;
    }
  else
    {
    return 0;
    }   
  }

RPSlot world_player_slot;
  
BBDECL int BBCALL PlayerSetSlot(char *name)
  {
  if(world_players_it->second.hasSlot(name))
    {
    world_player_slot=RPSlot(*world_players_it->second.getSlot(name));
    return 1;
    }
  else
    {
    return 0;
    }   
  }

Uint32 world_player_slot_it;   

BBDECL void BBCALL SlotResetRPObject()
  {
  world_player_slot_it=0;
  }
  
BBDECL int BBCALL SlotHasNextRPObject()
  {
  if(world_player_slot_it!=world_player_slot.size())
    {
    return 1;
    }
  else
    {
    return 0;
    }
  }
  
BBDECL void BBCALL SlotNextRPObject()
  {
  if(world_player_slot_it!=world_player_slot.size())
    {
    ++world_player_slot_it;
    }  
  }
  
BBDECL int BBCALL SlotFindRPObject(Uint32 id)
  {
  for(Uint32 i=0;i<world_player_slot.size();++i)
    {
    if(world_player_slot.get(i).getInt("id")==id)
      {
      world_player_slot_it=i;
      return 1;
      }
    }
  
  return 0;
  }
  

BBDECL char * BBCALL RPObjectGet(char *attribute)
  {
  static char *returnValue;
  
  if(world_player_slot.get(world_player_slot_it).has(attribute))
    {
    if(returnValue)
      {
      delete[] returnValue; 
      returnValue=0;
      }
    
    string value=world_player_slot.get(world_player_slot_it).get(attribute);
    returnValue=new char[value.size()+1];
    strcpy(returnValue,value.c_str());  

    global_trace.add("RPObjectGet","D",string("Attribute ")+attribute+" exits");
    global_trace.add("RPObjectGet","<");
    return returnValue;
    }
  else
    {
    global_trace.add("RPObjectGet","D",string("Attribute ")+attribute+" does NOT exit");
    global_trace.add("RPObjectGet","<");
    return "";
    }  
  }
  
BBDECL int BBCALL RPObjectHas(char *attribute)
  {
  global_trace.add("PlayerHas",">");
  global_trace.add("PlayerHas","<");
  return (world_player_slot.get(world_player_slot_it).has(attribute)==true)?1:0;
  }
  
BBDECL void BBCALL RPObjectPut(char *attribute, char *value)
  {
  world_player_slot.get(world_player_slot_it).put(attribute,value);
  }



int handleActionMessage(string module)
  {
  int j=0;
  while(j<MAX_NUM_ITERATIONS)
    {
    arianne::Message *msg=networkClient->getMessage();
    if(msg && msg->type==arianne::TYPE_S2C_ACTION_ACK)
	  {
      global_trace.add(module,"D","Recieved Action ACK Message");
	  delete msg;
	  msg=0;
	
      global_trace.add(module,"<");
	  return 1;
	  }
    else if(msg && msg->type==arianne::TYPE_S2C_PERCEPTION)
      {
      global_trace.add(module,"D","Recieved Perception Message");
      MessageS2CPerception *msgPerception=(MessageS2CPerception*)msg;          
      handleNewPerception(msgPerception);
    
      global_trace.add(module,"D","Sending Perception ACK Message");
      MessageC2SPerceptionACK msgPACK;
      networkClient->addMessage(msgPACK);          
    
      delete msg;
      msg=0;
      }
    else
  	  {
	  if(msg)
	    {
	    delete msg;
	    msg=0;
	    }
	
  	  j++;
	  SDL_Delay(750);
	  }
    }

  if(j==MAX_NUM_ITERATIONS)
    {
    global_trace.add(module,"E","Maximum number of iterations reached, connection aborted.");
    global_trace.add(module,"<");
    return 0;
    }
  else
    {
    global_trace.add(module,"!","This SHOULD NEVER happen");
    return -1;
    }
  }

BBDECL int BBCALL actionRequestFight(int wait_reply)
  {
  global_trace.add("actionRequestFight",">");
  
  try
    {
	if(Player())
      {
	  RPObject gladiator=world_players_it->second.getSlot("!gladiators")->get();		
	    
	  RPAction action;
	  action.put("type","request_fight");
	  action.put("gladiator_id",gladiator.get("id"));
	    
      global_trace.add("actionRequestFight","D","Sending Request Fight Action Message");
	  MessageC2SAction messageAction;
	  messageAction.action=action;
	  networkClient->addMessage(messageAction);
	  
	  if(wait_reply==0)
	    {
		global_trace.add("actionRequestFight","<");
	    return 0;
	    }
	    
	  return handleActionMessage("actionRequestFight");
      }    
    else
      {
      global_trace.add("actionRequestFight","E","Player not found");
      global_trace.add("actionRequestFight","<");
      return 0;
      }  
	}
  catch(exception e)
    {
    global_trace.add("actionRequestFight","X",string("Exception happened: ")+e.what());
    global_trace.add("actionRequestFight","<");
    return 0;
    }
  catch(...)
    {
    global_trace.add("actionRequestFight","X","Unknown Exception happened");
    global_trace.add("actionRequestFight","<");
    return 0;
    }
  }
  
BBDECL int BBCALL actionFightMode(int wait_reply,char *mode)
  {
  global_trace.add("actionFightMode",">");

  try
    {
	if(Player())
	  {
	  RPObject gladiator=world_players_it->second.getSlot("!gladiators")->get();
	    
	  RPAction action;
	  action.put("type","fight_mode");
	  action.put("fight_mode",mode);
	  action.put("gladiator_id",gladiator.get("id"));
	    
      global_trace.add("actionFightMode","D","Sending Fight Mode Action Message");
	  MessageC2SAction messageAction;
	  messageAction.action=action;
	  networkClient->addMessage(messageAction);

	  if(wait_reply==0)
	    {
		global_trace.add("actionFightMode","<");
	    return 0;
	    }
	    
	  return handleActionMessage("actionFightMode");
      }    
    else
      {
      global_trace.add("actionFightMode","E","Player not found");
      global_trace.add("actionFightMode","<");
      return 0;
      }  
    }
  catch(exception e)
    {
    global_trace.add("actionFightMode","X",string("Exception happened: ")+e.what());
    global_trace.add("actionFightMode","<");
    return 0;
    }
  catch(...)
    {
    global_trace.add("actionFightMode","X","Unknown Exception happened");
    global_trace.add("actionFightMode","<");
    return 0;
    }
  }

BBDECL int BBCALL actionVote(int wait_reply,char *vote)
  {
  global_trace.add("actionVote",">");

  try
    {
	if(Player())
	  {
	  RPAction action;
	  action.put("type","vote");
	  action.put("vote",vote);
	    
	  MessageC2SAction messageAction;
	  messageAction.action=action;
	  networkClient->addMessage(messageAction);

	  if(wait_reply==0)
	    {
		global_trace.add("actionVote","<");
	    return 0;
	    }
	    
	  return handleActionMessage("actionVote");
      }    
    else
      {
      global_trace.add("actionVote","E","Player not found");
      global_trace.add("actionVote","<");
      return 0;
      }  
    }
  catch(exception e)
    {
    global_trace.add("actionVote","X",string("Exception happened: ")+e.what());
    global_trace.add("actionVote","<");
    return 0;
    }
  catch(...)
    {
    global_trace.add("actionVote","X","Unknown Exception happened");
    global_trace.add("actionVote","<");
    return 0;
    }
  }

BBDECL int BBCALL actionChat(int wait_reply,char *text)
  {
  global_trace.add("actionChat",">");

  try
    {
	if(Player())
	  {
	  RPAction action;
	  action.put("type","chat");
	  action.put("content",text);
	    
	  MessageC2SAction messageAction;
	  messageAction.action=action;
	  networkClient->addMessage(messageAction);

	  if(wait_reply==0)
	    {
		global_trace.add("actionChat","<");
	    return 0;
	    }
	    
	  return handleActionMessage("actionChat");
      }    
    else
      {
      global_trace.add("actionChat","E","Player not found");
      global_trace.add("actionChat","<");
      return 0;
      }  
    }
  catch(exception e)
    {
    global_trace.add("actionChat","X",string("Exception happened: ")+e.what());
    global_trace.add("actionChat","<");
    return 0;
    }
  catch(...)
    {
    global_trace.add("actionChat","X","Unknown Exception happened");
    global_trace.add("actionChat","<");
    return 0;
    }
  }

