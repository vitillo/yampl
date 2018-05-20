#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sys/wait.h>

#include "yampl.h"

using namespace yampl;
using namespace std;


void client(ISocketFactory *factory, const Channel &channel){
  ISocket *socket = factory->createClientSocket(channel, "client");
  char buffer[100];

  socket->send("Hello World!");
  socket->recv(buffer);
  assert(strcmp(buffer, "pong") == 0);

  delete socket;
}

void server(ISocketFactory *factory, const Channel &channel){
  ISocket *socket = factory->createServerSocket(channel);
  char buffer[100];
  std::string dest;
  socket->recv(buffer, dest);

  assert(dest == "client");

  try{
    socket->sendTo("foobar", "pong");
  }catch(UnroutableException){}

  socket->sendTo("client", "pong");

  delete socket;
}

int main(int argc, char *argv[])
{
  int status = -1;

  if(fork() == 0){
    ISocketFactory *zmqFactory = new SocketFactory();
    client(zmqFactory, Channel("zmq", LOCAL));
    ISocketFactory *pipeFactory = new SocketFactory();
    client(pipeFactory, Channel("pipe", LOCAL_PIPE));
    ISocketFactory *shmFactory = new SocketFactory();
    client(shmFactory, Channel("shm", LOCAL_SHM));
  }else{
    ISocketFactory *zmqFactory = new SocketFactory();
    server(zmqFactory, Channel("zmq", LOCAL));
    ISocketFactory *pipeFactory = new SocketFactory();
    server(pipeFactory, Channel("pipe", LOCAL_PIPE));
    ISocketFactory *shmFactory = new SocketFactory();
    server(shmFactory, Channel("shm", LOCAL_SHM));

    wait(&status);
    cout << "Success" << endl;
	status = 0;
  }

  return status;
}
