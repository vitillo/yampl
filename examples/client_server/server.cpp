#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]){
  char ping[100];
  Channel channel("service", LOCAL_SHM);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createServerSocket(channel);

  while(true){
    socket->recv(ping);
    socket->send("pong");
    cout << ping << endl;
    sleep(1);
  }
}
