#include <unistd.h>
#include <iostream>

#include "yampl.h"

using namespace std;
using namespace yampl;

int main(int argc, char *argv[]){
  char ping[100], *ping_ptr = &ping[0];
  string pong = "Pong from " + to_string(getpid());
  
  Channel channel("service");
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createServerSocket(channel);

  while(true){
    socket->recv(&ping_ptr, sizeof(ping));
    socket->send(pong.c_str(), pong.size() + 1);
    cout << ping << endl;
    sleep(1);
  }
}
