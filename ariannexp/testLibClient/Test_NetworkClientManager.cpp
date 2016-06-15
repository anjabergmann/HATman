#include "Test_NetworkClientManager.h"

using namespace std;
using namespace arianne;

Test_NetworkClientManager::Test_NetworkClientManager()
  {
  }

void Test_NetworkClientManager::test()
  {
  try
    {
    NetworkClientManager netClient("127.0.0.1",0);
   
    MessageC2SLogin messageLogin;
    messageLogin.username="Test Player";
    messageLogin.password="Test Password";  
    netClient.addMessage(messageLogin);

    int i=0;
    while(i<5)
      {
      ++i;
      SDL_Delay(100);
      Message *msg=netClient.getMessage();
      if(msg)
        {
        --i;
        cout << "We got a message!: " << (int)msg->type << endl;
        delete msg;
        }
      }

    MessageC2SChooseCharacter messageChooseCharacter;
    messageChooseCharacter.character="Son Goku";
    netClient.addMessage(messageChooseCharacter);

    i=0;
    while(i<10)
      {
      ++i;
      SDL_Delay(1000);
      Message *msg=netClient.getMessage();
      if(msg)
        {
        --i;
        cout << "We got a message!: " << (int)msg->type << endl;
        delete msg;
        }
      }

    MessageC2SLogout messageLogout;
    netClient.addMessage(messageLogout);
    }
  catch(...)
    {
    CPPUNIT_FAIL("Exception happened!");
    }
  }