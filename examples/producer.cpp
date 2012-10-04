#include "Socket.h"
#include "ZMQSocketFactory.h"
#include "PipeSocketFactory.h"
#include "utils.h"

using namespace IPC;
using namespace std;

int main(int argc, char *argv[]){
  string message = "Hello from " +  pid();
  
  ISocket *socket = ZMQSocketFactory().createProducerSocket(Channel("pipe", ONE_TO_MANY), true, deallocator);

  while(true){
    socket->send(message.c_str(), message.size());
    cout << "Message sent" <<  endl;
    sleep(1);
  }

  /*ISocket *socket = PipeSocketFactory().createProducerSocket(Channel("pipe", ONE_TO_ONE), true, deallocator);

  while(true){
    socket->send(message.c_str(), message.size());
    cout << "Message sent" <<  endl;
    sleep(1);
  }*/
}
