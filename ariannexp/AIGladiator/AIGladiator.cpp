// AIGladiator.cpp: define el punto de entrada de la aplicación de consola.
#include <iostream>
#include <string>
#include <SDL.h>

#include <ctime>
#include "../libGladiator/libGladiator.h"

using namespace std;

string chat[]=
  {
  "I'm the first Gladiator bot, did you know?",
  "Hi buddy, how is it going?",
  "Hehehe, nice weather out there!",
  "I love Windows",
  "AHHH!!! There is a penguin out there!",
  "A day for firm decisions!!!!!  Or is it?",
  "Come on! I am only a stupid bot",
  "It becomes a bit boring all that text",
  "Please reports bugs on the game!",
  "If you have problems, please go to http://arianne.sf.net and report",
  "This is a a game in heavy development, your reports are greatly appreciated",
  "You are our best developer! Report bugs!",
  ":-P",
  "You should run always the latest version available",
  "Don't worry, be happy!"
  };
  
int chat_size=15;
string playerName;

bool voiceStatus=true;

int main(int argc, char** argv)
  {
  string choosenmode;

  while(true)
    {
	cout << "Trying to connect to server..." << endl;
	string result=ConnectToMarauroaServer("192.168.0.1",3214,argv[1],argv[2]);
	if(result!="OK")
	  {
	  cout << "FAILED";
	  return 1;
	  }
	else
	  {
	  ResetCharacter();    
	  cout << "Choosing character..." << endl;
	  result=ChooseCharacter(argv[3]);
	    
	  if(result!="OK")
		{
		cout << "FAILED";
		return 1;
		}
	    
	  cout << "Starting perception loop..." << endl;
	  Uint32 startTime=SDL_GetTicks();
	  bool enter=false;
	    
	  srand((int)time(0));
	  Uint32 total=180000+((rand()%120)*1000);
	    
	  int turn=0;
	  while(SDL_GetTicks()<(startTime+total))
		{      
		int iresult=HasNewPerception();
		if(iresult==1)
		  {
  		  cout << SDL_GetTicks() << " - " << iresult << endl;
		  cout << "Number of Players: " << NumPlayers() << endl;
	       
		  ResetPlayer();      
		  while(HasNextPlayer()==1)
			{        
			if(PlayerHas("?text"))
			  {
			  string text=PlayerGet("?text");
			  if(text.find(playerName)!=string::npos)
			    {			    
			    voiceStatus=!voiceStatus;
			    }
			  }
			  
			cout << "Player name: " << string(PlayerGet("name")) << endl;
			NextPlayer();
			}        
	        
			if(Player()==1)
			{  
			if(PlayerHas("fighting")==0 && PlayerHas("requested")==0)
				{
				cout << "I request fight"<< endl;
				actionRequestFight(1);
				}
		    else  if((PlayerHas("fighting")==1) && (string(ArenaGet("status"))=="fighting"))
			  {
			  cout << "I change fight mode"<< endl;
			  string mode[]={"rock","paper","scissor"};
     		  actionFightMode(0,const_cast<char*>(mode[SDL_GetTicks()%3].c_str()));
			  }
				        
			if(voiceStatus && (rand()%20==0))
			  {
			  cout << "I talk"<< endl;
			  actionChat(0,const_cast<char*>(chat[rand()%chat_size].c_str()));
			  }
			}
			
  		  ++turn;
		  }
	      
		SDL_Delay(100);
		}    
	    
	  cout << "Logout of server"<< endl;
	  DisconnectFromMarauroaServer();
	  SDL_Delay((rand()%40)*1000);
      } 
    }
  
  return 0;
  }

