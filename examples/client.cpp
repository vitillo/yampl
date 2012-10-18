#include "ZMQ/SocketFactory.h"

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
    socket->receive((void**)&pong);
    cout << pong << endl;
  }
}
