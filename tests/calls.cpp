#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sys/wait.h>

#include "yampl.h"

using namespace yampl;
using namespace std;

Channel channel("service", LOCAL);

void client(){
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createClientSocket(channel);
  char buffer[] = "Second";
  
  socket->send("First", 6);

  socket->send(buffer);

  socket->send(2.3f);

  socket->send(string("Third"));

  delete socket;
  delete factory;
}

void server(){
  ISocketFactory *factory = new SocketFactory();
  ISocket *socket = factory->createServerSocket(channel);
  
  char buffer[100];
  char *buffer2 = new char[100];
  float ret;

  socket->recv(buffer);
  assert(strcmp(buffer, "First") == 0);

  socket->recv(buffer2, 100);
  assert(strcmp(buffer2, "Second") == 0);

  ret = socket->recv<float>();
  assert(ret == 2.3f);

  socket->recv(buffer);
  assert(strcmp(buffer, "Third") == 0);

  delete socket;
  delete factory;
}

int main()
{
  int status = -1;

  if(fork() == 0){
    client();
  }else{
    server();
    wait(&status);
    cout << "[calls] Success" << endl;
 	status = 0; 
  }

  return status;
}
