#include <iostream>

#include "ZMQ/SocketFactory.h"
#include "pipe/SocketFactory.h"

#include "utils.h"

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  char *ping = 0;
  string pong = "Pong from " + pid();
  
  ISocketFactory *factory = new ZMQ::SocketFactory();
  ISocket *socket = factory->createServerSocket(Channel("service", ONE_TO_ONE), true, deallocator);

  while(true){
    socket->receive(&ping);
    socket->send(pong.c_str(), pong.size());
    cout << ping << endl;
    sleep(1);
  }
}
