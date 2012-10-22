#include <iostream>
#include <thread>

#include "ZMQ/SocketFactory.h"

#include "utils.h"

using namespace std;
using namespace IPC;

void clientCode(ISocketFactory *factory, int id){
  char *pong = 0;
  string ping = "Ping from client " + to_string(id);

  Channel channel("service", MANY_TO_ONE, THREAD);
  ISocket *socket = factory->createClientSocket(channel, true, deallocator);

  socket->send(ping.c_str(), ping.size());
  socket->recv(&pong);
}

int main(int argc, char *argv[]){
  const int nThreads = 10;
  ISocketFactory *factory = new ZMQ::SocketFactory();

  thread server([factory] {
    char *ping = 0;
    string pong = "Pong from server thread";

    Channel channel("service", MANY_TO_ONE, THREAD);
    ISocket *socket = factory->createServerSocket(channel, true, deallocator);

    for(int i = 0; i < nThreads; i++){
      socket->recv(&ping);
      socket->send(pong.c_str(), pong.size());
      cout << ping << endl;
    }
  });

  
  for(int i = 0; i < nThreads; i++){
    thread client(clientCode, factory, i);
    client.detach();
  }

  server.join();
}
