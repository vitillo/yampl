#include <unistd.h>
#include <iostream>

#include "SocketFactory.h"
#include "utils/Thread.h"

inline void deallocator(void *, void*){}

using namespace std;
using namespace IPC;

void server(ISocketFactory *factory){
  Channel channel("service", MANY_TO_ONE, THREAD);
  ISocket *socket = factory->createServerSocket(channel, MOVE_DATA, deallocator);

  while(true){
    cout << "Ping from client " << socket->recv<int>() << endl;
    socket->send(0);
    sleep(1);
  }
}

void client(ISocketFactory *factory, int id){
  Channel channel("service", MANY_TO_ONE, THREAD);
  ISocket *socket = factory->createClientSocket(channel, MOVE_DATA, deallocator);

  while(true){
    socket->send(id);
    socket->recv<int>();
  }
}

int main(int argc, char *argv[]){
  const int nThreads = 10;
  ISocketFactory *factory = new SocketFactory();

  Thread s(tr1::bind(&server, factory));
  
  for(int i = 0; i < nThreads; i++){
    Thread c(tr1::bind(&client, factory, i));
    c.detach();
  }

  s.join();
}
