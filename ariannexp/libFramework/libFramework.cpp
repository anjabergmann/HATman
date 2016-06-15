// libFramework.cpp: define el punto de entrada de la aplicación DLL.
//
#include "libFramework.h"
#include "../libClient/NetworkClientManager.h"

#define MAX_NUM_ITERATIONS 15
using namespace arianne;
using namespace std;
NetworkClientManager *netClient=0;
std::list<std::string> serverInfoList;
std::list<std::string> characterList;

bool gotMapContent=false;
std::list<arianne::RPObject> mapContent;

Uint32 playerID=0;

bool gotNewPerception=false;
Perception *newPerception=0;

bool isError_flag=false;
Uint32 isError_code;
std::string isError_reason;

CALLBACKFUNC callback;

BBDECL void BBCALL setIdleCFunction(CALLBACKFUNC function)
  {
  callback=function;
  }


void *pythonCallbackFunction;

void setError(std::string reason)
  {
  isError_flag=true;
  isError_code=0;
  isError_reason=reason;
  }

BBDECL void BBCALL connectToArianne(char *serverName, int port) 
  {
  try
    {
    netClient=new NetworkClientManager(serverName, port);
    }
  catch(...)
    {
    }
  }
  
BBDECL bool BBCALL login(char *username, char *password) 
  {
  if(netClient==0)
    {
    return false;
    }
    
  MessageC2SLogin messageLogin;
  messageLogin.username=username;
  messageLogin.password=password;  
  netClient->addMessage(messageLogin);
    
  int messagesToRecieve=3;
  bool continueWaiting=true;
  bool gotNACK=false;

  while(continueWaiting && messagesToRecieve!=0 && !gotNACK)
    {
    arianne::Message *msg=0;
	try
	  {
	  msg=netClient->getMessage();
	  }
	catch(InvalidVersionException e)
	  {
	  setError("This is an outdated version of libFramework. Please update it at http://arianne.sourceforge.net");
	  return false;
	  }
    catch(std::exception e)
	  {
	  setError("Exception");
	  return false;
	  }
    
    if(msg && msg->type==arianne::TYPE_S2C_LOGIN_NACK)
      {
	  gotNACK=true;
      setError(((MessageS2CLoginNACK *)msg)->toString());
      }
    else if(msg && msg->type==arianne::TYPE_S2C_LOGIN_ACK)
      {
      --messagesToRecieve;
      }
    else if(msg && msg->type==arianne::TYPE_S2C_SERVERINFO)
      {
      MessageS2CServerInfo *msgServerInfo=(MessageS2CServerInfo*)msg;
      serverInfoList=msgServerInfo->contents;
      --messagesToRecieve;        
      }
    else if(msg && msg->type==arianne::TYPE_S2C_CHARACTERLIST)
      {
      MessageS2CCharacterList *msgCharList=(MessageS2CCharacterList*)msg;
      characterList=msgCharList->characters;
      --messagesToRecieve;        
      }
    else
      {
      continueWaiting=callback(pythonCallbackFunction);      
      }
       
    if(msg)
      {  
      delete msg;
      msg=0;
      }
    }
      
  if(!continueWaiting)
    {
	setError("Unable to connect to server. Check if server is up at http://gladiators.game-server.cc");
	return false;
    }
  else if(gotNACK)
    {
	return false;
	}
  else
    {
	return true;
	}
  }

BBDECL std::list<std::string> BBCALL availableCharacters()
  {
  return characterList;
  }
  
BBDECL std::list<std::string> BBCALL serverInfo() 
  {
  return serverInfoList;
  }

BBDECL std::string BBCALL getGameType()
  {
  list<string>::const_iterator it=serverInfoList.begin();
  
  while(it!=serverInfoList.end())
    {
    if(it->find("server_typeGame")!=string::npos)
      {
      int pos=it->find("=");
      return it->substr(pos+1);
      }
    }
  
  setError("ServerInfo not found");
  return "";
  }
  

BBDECL bool BBCALL chooseCharacter(std::string character)
  {
  MessageC2SChooseCharacter msgChooseCharacter;
  msgChooseCharacter.character=character;
  netClient->addMessage(msgChooseCharacter);

  int messagesToRecieve=2;
  int j=0;
  bool gotNACK=false;
  bool continueWaiting=true;

  while(continueWaiting && messagesToRecieve!=0 && !gotNACK)
    {
    arianne::Message *msg=netClient->getMessage();

    if(msg && msg->type==arianne::TYPE_S2C_CHOOSECHARACTER_NACK)
      {		
      gotNACK=true;
	  setError("Server rejects that character. Please check typos");
      }
    else if(msg && msg->type==arianne::TYPE_S2C_CHOOSECHARACTER_ACK)
      {
      MessageS2CChooseCharacterACK *msgCCharACK=(MessageS2CChooseCharacterACK*)msg;
      playerID=msgCCharACK->id;     
      --messagesToRecieve;        
      }
    else if(msg && msg->type==arianne::TYPE_S2C_MAP)
      {
      MessageS2CRPMap *msgMap=(MessageS2CRPMap*)msg;
      mapContent=msgMap->content;
	  gotMapContent=true;     
      --messagesToRecieve;        
      }
    else
      {
      continueWaiting=callback(pythonCallbackFunction);      
      }

    if(msg)
      {
      delete msg;
      msg=0;
      }
    }
      
  if(!continueWaiting)
    {
  	setError("Unable to connect to server. Check if server is up at http://gladiators.game-server.cc");
	return false;
    }
  else if(gotNACK)
    {
	return false;
	}
  else
    {
	return true;
	}
  }

BBDECL Uint32 BBCALL ourRPObjectID()
  {
  return playerID;
  }


BBDECL bool BBCALL hasRPMap()
  {
  return gotMapContent;
  }

BBDECL std::list<arianne::RPObject> BBCALL getRPMap()
  {
  return mapContent;
  }

BBDECL void BBCALL handlePerception(MessageS2CPerception *msgPerception)
  {
  Perception tmp;
  tmp.type=msgPerception->typePerception;
  tmp.timestamp=msgPerception->timestamp;
  tmp.added=msgPerception->addedObjects;
  tmp.modifiedAdded=msgPerception->modifiedAddedObjects;
  tmp.modifiedDeleted=msgPerception->modifiedDeletedObjects;
  tmp.deleted=msgPerception->deletedObjects;
  tmp.myRPObjectChanged=((msgPerception->myRPObject.size()==0)?false:true);
  tmp.myRPObject=msgPerception->myRPObject;

  if(newPerception)
    {
    delete newPerception;
    }
    
  newPerception=new Perception(tmp);
      
  MessageC2SPerceptionACK msgPACK;
  netClient->addMessage(msgPACK);      
  }  

BBDECL bool BBCALL hasPerception()
  {
  arianne::Message *msg=netClient->getMessage();
  
  if(msg && msg->type==arianne::TYPE_S2C_PERCEPTION)
    {
    handlePerception((MessageS2CPerception*)msg);

    delete msg;
    msg=0;
    
    return true;
  	}
 
  return false;
  }

BBDECL Perception BBCALL getPerception()  
  {
  return Perception(*newPerception);
  }

BBDECL void BBCALL send(arianne::RPAction action)
  {
  MessageC2SAction messageAction;
  messageAction.action=action;
  netClient->addMessage(messageAction);
  }
  
BBDECL Uint32 BBCALL sendBlocking(arianne::RPAction action)
  {
  MessageC2SAction messageAction;
  messageAction.action=action;
  netClient->addMessage(messageAction);

  int j=0;
  bool gotAction=false;
  Uint32 actionid;
  bool continueWaiting=true;

  while(continueWaiting && gotAction)
    {
    arianne::Message *msg=netClient->getMessage();
    if(msg && msg->type==arianne::TYPE_S2C_ACTION_ACK)
	  {
	  gotAction=true;
	  actionid=((MessageS2CActionACK*)msg)->actionid;
	  }
    else if(msg && msg->type==arianne::TYPE_S2C_PERCEPTION)
      {
      handlePerception((MessageS2CPerception*)msg);
      }
	else
	  {
      continueWaiting=callback(pythonCallbackFunction);      
	  }
   
    if(msg)
	  {
	  delete msg;
	  msg=0;
	  }
    }

  if(j==MAX_NUM_ITERATIONS)
    {
  	setError("Unable to connect to server. Check if server is up at http://gladiators.game-server.cc");
    return 0;
    }
  else
    {
    return actionid;
    }  
  }

BBDECL void BBCALL outOfSync()
  {
  MessageC2SOutOfSync msgOutOfSync;
  netClient->addMessage(msgOutOfSync);
  }

BBDECL bool BBCALL logout()
  {
  MessageC2SLogout msgLogout;
  netClient->addMessage(msgLogout);

  int j=0;
  bool gotACK=false;
  bool gotNACK=false;
  bool continueWaiting=true;

  while(continueWaiting && !gotACK && !gotNACK)
    {
    arianne::Message *msg=netClient->getMessage();
    if(msg && msg->type==arianne::TYPE_S2C_LOGOUT_NACK)
      {
	  gotNACK=true;
      }
    else if(msg && msg->type==arianne::TYPE_S2C_LOGOUT_ACK)
      {
	  gotACK=true;
      }
	else
	  {
      continueWaiting=callback(pythonCallbackFunction);      
	  }
   
    if(msg)
	  {
	  delete msg;
	  msg=0;
	  }
    }

  if(!continueWaiting)
    {
  	setError("Unable to connect to server. Check if server is up at http://gladiators.game-server.cc");
	return false;
    }
  else if(gotNACK)
    {
	return false;
	}
  else
    {
	return true;
	}
  }


BBDECL bool BBCALL iserror()
  {
  return isError_flag;
  }

BBDECL Uint32 BBCALL errorCode()
  {
  return isError_code;
  }

BBDECL std::string BBCALL errorReason()
  {
  return isError_reason;
  }
