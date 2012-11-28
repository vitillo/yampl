#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yampl.h"
#include "yampl/pipe/PipeSocketFactory.h"
#include "yampl/shm/SHMSocketFactory.h"
#include "yampl/ZMQ/ZMQSocketFactory.h"

using namespace yampl;
using namespace std;

Channel channel("service");

void client(ISocket *socket){
  socket->send("Hello World!");

  char buffer[100];
  socket->recv(buffer);

  assert(strcmp(buffer, "pong") == 0);

  delete socket;
}

void server(ISocket *socket){
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

int main(){
  if(fork() == 0){
    ISocketFactory *factory = new shm::SocketFactory();
    ISocket *socket = factory->createClientSocket(channel, "client");
    client(socket);
    factory = new pipe::SocketFactory();
    socket = factory->createClientSocket(channel, "client");
    client(socket);
    factory = new ZMQ::SocketFactory();
    socket = factory->createClientSocket(channel, "client");
    client(socket);
  }else{
    ISocketFactory *factory = new shm::SocketFactory();
    ISocket *socket = factory->createServerSocket(channel);
    server(socket);
    factory = new pipe::SocketFactory();
    socket = factory->createServerSocket(channel);
    server(socket);
    factory = new ZMQ::SocketFactory();
    socket = factory->createServerSocket(channel);
    server(socket);
    wait();
    cout << "Success" << endl;
  }
}
