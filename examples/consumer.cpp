#include <iostream>

#include "ZMQSocketFactory.h"

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  char *message = 0;
  Environment env;

  ISocket *socket = ZMQSocketFactory().createConsumerSocket(Channel(env, "pipe", ONE_TO_MANY));

  while(true){
    socket->receive((void **)&message);
    cout << message << endl;
  }
}
