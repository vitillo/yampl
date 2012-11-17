#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]){
  char *message = 0;

  Channel channel("127.0.0.1:3333", DISTRIBUTED);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createServerSocket(channel, MOVE_DATA);

  while(true){
    socket->recv(message);
    cout << message << endl;
  }
}
