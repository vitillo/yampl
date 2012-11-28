#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yampl.h"
#include "yampl/ZMQ/ZMQSocketFactory.h"
#include "yampl/pipe/PipeSocketFactory.h"
#include "yampl/shm/SHMSocketFactory.h"

using namespace std;
using namespace yampl;

Channel channel("service");
const char message[] = "Hello World!";

void client(ISocketFactory *factory){
  char *buffer = new char[100];
  ISocket *socket = factory->createClientSocket(channel);

  socket->send(message);

  try{
    socket->recv(buffer, 1);
  }catch(InvalidSizeException){
    socket->recv(buffer, 100);
  }

  assert(strcmp(buffer, message) == 0);

  delete socket;
}

void server(ISocketFactory *factory){
  char *buffer = new char[100];
  ISocket *socket = factory->createServerSocket(channel);

  try{
    socket->recv(buffer, 1);
  }catch(InvalidSizeException){
    socket->recv(buffer, 100);
  }

  assert(strcmp(buffer, message) == 0);
  socket->send(message);

  delete socket;
}

int main(int argc, char *argv[]){
  if(fork() == 0){
    ISocketFactory *zmqFactory = new ZMQ::SocketFactory();
    client(zmqFactory);
    ISocketFactory *pipeFactory = new pipe::SocketFactory();
    client(pipeFactory);
    ISocketFactory *shmFactory = new shm::SocketFactory();
    client(shmFactory);
  }else{
    ISocketFactory *zmqFactory = new ZMQ::SocketFactory();
    server(zmqFactory);
    ISocketFactory *pipeFactory = new pipe::SocketFactory();
    server(pipeFactory);
    ISocketFactory *shmFactory = new shm::SocketFactory();
    server(shmFactory);

    wait();
    cout << "Success" << endl;
  }
}
