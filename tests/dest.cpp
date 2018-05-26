#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sys/wait.h>

#include "yampl.h"

using namespace yampl;
using namespace std;

void client(ISocketFactory *factory, const Channel &channel)
{
  ISocket *socket = factory->createClientSocket(channel, "client");
  char buffer[100];

  socket->send("Hello World!");
  socket->recv(buffer);
  assert(strcmp(buffer, "pong") == 0);

  delete socket;
}

void server(ISocketFactory* factory, const Channel &channel)
{
  ISocket *socket = factory->createServerSocket(channel);
  char buffer[100];
  std::string dest;
  socket->recv(buffer, dest);

  std::cout << "dest = " << dest << std::endl;
  assert(dest == "client");

  try {
    socket->sendTo("foobar", "pong");
  }
  catch(UnroutableException&) {
    // OK, it's expected to reach this
  }

  socket->sendTo("client", "pong");

  delete socket;
}

int main(int argc, char *argv[])
{
  int status = -1;
  ISocketFactory* factory;

  if(fork() == 0)
  {
    factory = new SocketFactory();

    client(factory, Channel("zmq", LOCAL));
    client(factory, Channel("pipe", LOCAL_PIPE));
    client(factory, Channel("shm", LOCAL_SHM));

    delete factory;
  }
  else {
    factory = new SocketFactory();
    server(factory, Channel("zmq", LOCAL));
    server(factory, Channel("pipe", LOCAL_PIPE));
    server(factory, Channel("shm", LOCAL_SHM));

    wait(&status);
    cout << "Success" << endl;
	status = 0;

    delete factory;
  }

  return status;
}
