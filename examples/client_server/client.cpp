#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]){
  char pong[100];
  const string ping = "Ping from " + to_string(getpid());
  
  Channel channel("service");
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel);

  while(true){
    socket->send(ping);
    socket->recv(pong);
    cout << pong << endl;
  }
}
