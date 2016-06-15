#include "NetworkClientManager.h"
#include <sstream>

arianne::NetworkClientManager::NetworkClientManager(std::string const& host, Uint16 port):
  pendingPackets(),
  msgFactory()
  {
  global_trace.add("NetworkClientManager::NetworkClientManager",">");

  if(SDLNet_Init()==-1) 
    {
    global_trace.add("NetworkClientManager::NetworkClientManager","X",SDLNet_GetError());
    throw IOException(SDLNet_GetError());
    }
    
  clientid=-1;
  SDLNet_ResolveHost(&address,host.c_str(),port);
  
  socket=SDLNet_UDP_Open(0);
  if(!socket)
    {
    global_trace.add("NetworkClientManager::NetworkClientManager","X",SDLNet_GetError());
    throw IOException(std::string("Can't open UDP socket: ")+SDLNet_GetError());
    }

  global_trace.add("NetworkClientManager::NetworkClientManager","<");
  }

arianne::NetworkClientManager::~NetworkClientManager()
  {
  global_trace.add("NetworkClientManager::~NetworkClientManager",">");
  SDLNet_Quit();
  SDLNet_UDP_Close(socket);
  global_trace.add("NetworkClientManager::~NetworkClientManager","<");
  }

arianne::Message *arianne::NetworkClientManager::getMessage()
  {
  global_trace.add("NetworkClientManager::getMessage",">");
  
  global_trace.add("NetworkClientManager::getMessage","D","Checking for completed messages");
  std::map<Uint8,NetworkClientManager::PacketContainer>::iterator it;
  for(it=pendingPackets.begin();it!=pendingPackets.end();++it)
    {
    if(it->second.remaining==0)
      {
      global_trace.add("NetworkClientManager::getMessage","D","Found a completed message");
      Message *msg=0;
      try
        {
        global_trace.add("NetworkClientManager::getMessage","D","Before deserializing message");
        msg=msgFactory.getMessage(it->second.content,it->second.address);
        global_trace.add("NetworkClientManager::getMessage","D","After deserializing message");
        }
      catch(InvalidVersionException e)
        {
        throw e;
        }
      catch(std::exception e)
        {
        global_trace.add("NetworkClientManager::getMessage","X",e.what());
        }
      catch(...)
        {
        global_trace.add("NetworkClientManager::getMessage","X","Exception while deserializing the completed messages");
        }       
      
      if(msg && msg->type==TYPE_S2C_LOGIN_ACK)
        {
        global_trace.add("NetworkClientManager::getMessage","D","Recieved Login message: Setting clientid");
        clientid=msg->clientid;
        }
      
      pendingPackets.erase(it);  

      global_trace.add("NetworkClientManager::getMessage","<");
      return msg;
      }
    else
      {
      if(SDL_GetTicks()-it->second.timestamp>CLIENT_MESSAGE_DROPPED_TIMEOUT)
        {
        global_trace.add("NetworkClientManager::getMessage","D","Message has timedout. Removing");
        pendingPackets.erase(it);
        }
      }
    }
  
  int i=0;
  
  while(i<CLIENT_NETWORK_NUM_READ)
    {
    try
      {
      ++i;
      UDPpacket *packet=SDLNet_AllocPacket(NETWORK_PACKET_SIZE);
      if(SDLNet_UDP_Recv(socket,packet))
        {
        global_trace.add("NetworkClientManager::getMessage","D","Read an UDP Packet");
        Uint8 *data=packet->data;
        
        Uint8 total=data[0];
        Uint8 position=data[1];
        Uint8 signature=data[2];
        
        std::ostringstream out;
        out << "TOTAL(" << (int)total << "); POS(" << (int)position << "); SIG(" << (int)signature <<")";
        global_trace.add("NetworkClientManager::getMessage","D",out.str());
        
        it=pendingPackets.find(signature);
        if(it==pendingPackets.end())
          {
          global_trace.add("NetworkClientManager::getMessage","D","Packet is a NEW message recieved");
          PacketContainer message;
          
          message.signature=signature;
          message.remaining=total-1;
          message.address=packet->address;
          message.content.resize((NETWORK_PACKET_SIZE-3)*total);
          message.timestamp= SDL_GetTicks();
          
          memcpy(&message.content[(NETWORK_PACKET_SIZE-3)*position],
                 &packet->data[3],
                 packet->len-3);
          
          pendingPackets.insert(std::pair<Uint8,NetworkClientManager::PacketContainer>(signature,message));
          }
        else
          {
          global_trace.add("NetworkClientManager::getMessage","D","Packet is an EXISTING message recieved");
          --it->second.remaining;
          
          if(it->second.remaining<0)
            {
            global_trace.add("NetworkClientManager::getMessage","X","Severe error");
            throw IOException("Confused messages");
            }

          memcpy(&(it->second.content[(NETWORK_PACKET_SIZE-3)*position]),
                 &packet->data[3],
                 packet->len-3);
          }        
        }
      else
        {
        SDL_Delay(10);
        }
      SDLNet_FreePacket(packet);
      }
    catch(std::exception e)
      {
      global_trace.add("NetworkClientManager::getMessage","X",e.what());
      pendingPackets.clear();
      } 
    catch(...)
      {
      global_trace.add("NetworkClientManager::getMessage","X","Catched an unnamed exception");
      pendingPackets.clear();
      }
    }
  
  global_trace.add("NetworkClientManager::getMessage","<");
  return 0;
  }

void arianne::NetworkClientManager::addMessage(Message& message)
  {
  try
    {
    message.clientid=clientid;
    message.source=address;
    
    Serializer s;
    message.write(s);
    
    std::vector<Uint8> data;
    s.toVector(data);

    UDPpacket *packet=SDLNet_AllocPacket((Uint32)data.size());
    memcpy(packet->data,&data[0],data.size());
    packet->len=(Uint32)data.size();
    packet->address=address;
    
    SDLNet_UDP_Send(socket,-1,packet);
    
    SDLNet_FreePacket(packet);
    }
  catch(std::exception e)
    {
    global_trace.add("NetworkClientManager::addMessage","X",e.what());
    }
  }
