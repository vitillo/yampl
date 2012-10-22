#include "SocketFactory.h"

#include <iostream>

#include "utils.h"

using namespace IPC;
using namespace std;

int main(int argc, char *argv[]){
  char *pong = 0;
  string ping = "Ping from " + pid();
  
  Channel channel("service", MANY_TO_ONE);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel, true, deallocator);

  while(true){
    socket->send(ping.c_str(), ping.size());
    socket->recv(&pong);
    cout << pong << endl;
  }
}
