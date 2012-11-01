#include <unistd.h>
#include <iostream>

#include "SocketFactory.h"
#include "utils/utils.h"

using namespace IPC;
using namespace std;

void deallocator(void *, void*){}

int main(int argc, char *argv[]){
  string message = "Hello from " +  to_string(getpid());
  
  Channel channel("pipe", ONE_TO_MANY);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createProducerSocket(channel, MOVE_DATA, deallocator);

  while(true){
    socket->send(message.c_str(), message.size());
    cout << "Message sent" <<  endl;
    sleep(1);
  }
}
