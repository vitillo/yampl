#include <iostream>

#include "ZMQ/SocketFactory.h"

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  char *message = 0;

  ISocketFactory *factory = new ZMQ::SocketFactory();
  ISocket *socket = factory->createConsumerSocket(Channel("pipe", ONE_TO_MANY));

  while(true){
    socket->receive(&message);
    cout << message << endl;
  }
}
