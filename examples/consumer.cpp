#include <iostream>

#include "ZMQSocketFactory.h"
#include "PipeSocketFactory.h"

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  char *message = 0;

  ISocket *socket = ZMQSocketFactory().createConsumerSocket(Channel("pipe", ONE_TO_MANY));

  //ISocket *socket = PipeSocketFactory().createConsumerSocket(Channel("pipe", ONE_TO_ONE));

  while(true){
    socket->receive((void **)&message);
    cout << message << endl;
  }
}
