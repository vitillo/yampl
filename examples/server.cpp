#include <unistd.h>
#include <iostream>

#include "SocketFactory.h"
#include "utils/utils.h"

using namespace std;
using namespace YAMPL;

int main(int argc, char *argv[]){
  char ping[100], *ping_ptr = &ping[0];
  string pong = "Pong from " + to_string(getpid());
  
  Channel channel("service", MANY_TO_ONE);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createServerSocket(channel);

  while(true){
    socket->recv(&ping_ptr, sizeof(ping));
    socket->send(pong.c_str(), pong.size() + 1);
    cout << ping << endl;
    sleep(1);
  }
}
