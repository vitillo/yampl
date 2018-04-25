#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sys/wait.h>

#include "yampl.h"
#include "yampl/zeromq/SocketFactory.h"
#include "yampl/pipe/SocketFactory.h"
#include "yampl/shm/SocketFactory.h"

using namespace std;
using namespace yampl;

const char message[] = "Hello World!";

void client(ISocketFactory *factory, const Channel &channel){
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
  delete factory;
}

void server(ISocketFactory *factory, const Channel &channel){
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
  delete factory;
}

int main(int argc, char *argv[])
{
  int status;

  if(fork() == 0){
    ISocketFactory *zmqFactory = new zeromq::SocketFactory();
    client(zmqFactory, Channel("zmq", LOCAL));
    ISocketFactory *pipeFactory = new pipe::SocketFactory();
    client(pipeFactory, Channel("pipe", LOCAL_PIPE));
    ISocketFactory *shmFactory = new shm::SocketFactory();
    client(shmFactory, Channel("shm", LOCAL_SHM));
  }else{
    ISocketFactory *zmqFactory = new zeromq::SocketFactory();
    server(zmqFactory, Channel("zmq", LOCAL));
    ISocketFactory *pipeFactory = new pipe::SocketFactory();
    server(pipeFactory, Channel("pipe", LOCAL_PIPE));
    ISocketFactory *shmFactory = new shm::SocketFactory();
    server(shmFactory, Channel("shm", LOCAL_SHM));

    wait(&status);
    cout << "Success" << endl;
  }
}
