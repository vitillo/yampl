#include <iostream>

#include "SocketFactory.h"

#include "utils.h"

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  char *ping = 0;
  string pong = "Pong from " + pid();
  
  Channel channel("service", MANY_TO_ONE);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createServerSocket(channel, true, deallocator);

  while(true){
    socket->recv(&ping);
    socket->send(pong.c_str(), pong.size());
    cout << ping << endl;
    sleep(1);
  }
}
