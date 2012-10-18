#include "ZMQ/SocketFactory.h"

#include "utils.h"

using namespace IPC;
using namespace std;

int main(int argc, char *argv[]){
  string message = "Hello from " +  pid();
  
  ISocketFactory *factory = new ZMQ::SocketFactory();
  ISocket *socket = factory->createProducerSocket(Channel("pipe", ONE_TO_MANY), true, deallocator);

  while(true){
    socket->send(message.c_str(), message.size());
    cout << "Message sent" <<  endl;
    sleep(1);
  }
}
