#include <iostream>
#include "SocketFactory.h"

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  char *message = 0;

  Channel channel("pipe", ONE_TO_MANY);
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createConsumerSocket(channel, MOVE_DATA);

  while(true){
    socket->recv(&message);
    cout << message << endl;
  }
}
