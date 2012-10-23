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
    char *ping = 0;
    string pong = "Pong from server thread";

    Channel channel("service", MANY_TO_ONE, THREAD);
    ISocket *socket = factory->createServerSocket(channel, MOVE_DATA, deallocator);

    while(true){
      socket->recv(&ping);
      socket->send(pong.c_str(), pong.size() + 1);
      cout << ping << endl;
      sleep(1);
    }
  });

  
  for(int i = 0; i < nThreads; i++){
    thread t([factory, i] {
      char *pong = 0;
      string ping = "Ping from client " + to_string(i);

      Channel channel("service", MANY_TO_ONE, THREAD);
      ISocket *socket = factory->createClientSocket(channel, MOVE_DATA, deallocator);

      while(true){
	socket->send(ping.c_str(), ping.size() + 1);
	socket->recv(&pong);
      }
    });

    t.detach();
  }

  server.join();
}
