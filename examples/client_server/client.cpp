#include <unistd.h>
#include <iostream>

#include "utils/utils.h"
#include "YAMPLSocketFactory.h"

using namespace std;
using namespace YAMPL;

int main(int argc, char *argv[]){
  const string ping = "Ping from " + to_string(getpid());
  char pong[100], *pong_ptr = &pong[0];
  
  Channel channel("service");
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel);

  while(true){
    socket->send(ping.c_str(), ping.size() + 1);
    socket->recv(&pong_ptr, sizeof(pong));
    cout << pong << endl;
  }
}
