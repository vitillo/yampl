#include "ZMQ/SocketFactory.h"

#include <iostream>

#include "utils.h"

using namespace IPC;
using namespace std;

int main(int argc, char *argv[]){
  char *pong = 0;
  string ping = "Ping from " + pid();
  
  ISocketFactory *factory = new ZMQ::SocketFactory();
  ISocket *socket = factory->createClientSocket(Channel("service", ONE_TO_ONE), true, deallocator);

  while(true){
    socket->send(ping.c_str(), ping.size());
    socket->recv(&pong);
    cout << pong << endl;
  }
}
