#include <unistd.h>
#include <iostream>
#include "SocketFactory.h"

using namespace IPC;
using namespace std;

int main(int argc, char *argv[]){
  const string ping = "Ping from " + to_string(getpid());
  char pong[100], *pong_ptr = &pong[0];
  
  Channel channel("service", MANY_TO_ONE);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel);

  while(true){
    socket->send(ping.c_str(), ping.size() + 1);
    socket->recv(&pong_ptr, sizeof(pong));
    cout << pong << endl;
  }
}
