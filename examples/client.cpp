#include "ZMQSocketFactory.h"
#include "utils.h"

using namespace IPC;
using namespace std;

int main(int argc, char *argv[]){
  char *pong = 0;
  Environment env;
  string ping = "Ping from " + pid();
  
  ISocket *socket = ZMQSocketFactory().createClientSocket(Channel(env, "service", MANY_TO_ONE), true, deallocator);

  while(true){
    socket->send(ping.c_str(), ping.size());
    socket->receive((void**)&pong);
    cout << pong << endl;
  }
}
