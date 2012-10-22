#include "SocketFactory.h"

#include <iostream>

#include "utils.h"

using namespace IPC;
using namespace std;

int main(int argc, char *argv[]){
  string message = "Hello from " +  pid();
  
  Channel channel("pipe", ONE_TO_MANY);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createProducerSocket(channel, true, deallocator);

  while(true){
    socket->send(message.c_str(), message.size());
    cout << "Message sent" <<  endl;
    sleep(1);
  }
}
