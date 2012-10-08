#include <iostream>

#include "ZMQSocketFactory.h"

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  char *message = 0;

  ISocketFactory *factory = new ZMQSocketFactory();
  ISocket *socket = factory->createConsumerSocket(Channel("pipe", ONE_TO_MANY));

  while(true){
    socket->receive((void **)&message);
    cout << message << endl;
  }
}
