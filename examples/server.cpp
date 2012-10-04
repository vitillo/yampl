#include <iostream>

#include "ZMQSocketFactory.h"
#include "utils.h"

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  char *ping = 0;
  string pong = "Pong from " + pid();
  
  ISocket *socket = ZMQSocketFactory().createServerSocket(Channel("service", MANY_TO_ONE), true, deallocator);

  while(true){
    socket->receive((void **)&ping);
    socket->send(pong.c_str(), pong.size());
    cout << ping << endl;
    sleep(1);
  }
}
