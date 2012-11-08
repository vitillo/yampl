#include <unistd.h>
#include <iostream>

#include "utils/utils.h"
#include "YAMPLSocketFactory.h"

using namespace YAMPL;
using namespace std;

void deallocator(void *, void*){}

int main(int argc, char *argv[]){
  string message = "Hello from " +  to_string(getpid());
  
  Channel channel("127.0.0.1:3333", DISTRIBUTED_PROCESS);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel, MOVE_DATA, deallocator);

  while(true){
    socket->send(message.c_str(), message.size());
    cout << "Message sent" <<  endl;
    sleep(1);
  }
}
