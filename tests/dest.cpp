#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yampl.h"

using namespace yampl;
using namespace std;

Channel channel("service");

void client(){
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel, "client");

  socket->send("Hello World!");

  char buffer[100];
  socket->recv(buffer);

  assert(strcmp(buffer, "pong") == 0);

  delete socket;
  delete factory;
}

void server(){
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createServerSocket(channel);

  char buffer[100];
  std::string dest;
  socket->recv(buffer, dest);

  assert(dest == "client");
  socket->sendTo(dest, "pong");

  delete socket;
  delete factory;
}

int main(){
  if(fork() == 0){
    client();
  }else{
    server();
    wait();
    cout << "Success" << endl;
  }
}
