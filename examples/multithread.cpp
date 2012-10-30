#include <unistd.h>
#include <iostream>
#include <thread>

#include "SocketFactory.h"

inline void deallocator(void *, void*){}

using namespace std;
using namespace IPC;

int main(int argc, char *argv[]){
  const int nThreads = 10;
  ISocketFactory *factory = new SocketFactory();

  thread server([factory] {
    Channel channel("service", MANY_TO_ONE, THREAD);
    ISocket *socket = factory->createServerSocket(channel, MOVE_DATA, deallocator);

    while(true){
      cout << "Ping from client " << socket->recv<int>() << endl;
      socket->send(0);
      sleep(1);
    }
  });
  
  for(int i = 0; i < nThreads; i++){
    thread t([factory, i] {
      Channel channel("service", MANY_TO_ONE, THREAD);
      ISocket *socket = factory->createClientSocket(channel, MOVE_DATA, deallocator);

      while(true){
        socket->send(i);
	socket->recv<int>();
      }
    });

    t.detach();
  }

  server.join();
}
