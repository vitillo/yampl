#include "yampl/Exceptions.h"

#include "yampl/shm/SocketFactory.h"
#include "yampl/shm/ClientSocket.h"
#include "yampl/shm/ServerSocket.h"

namespace yampl{
namespace shm{

  ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name){
  if(channel.context != LOCAL){
    throw UnsupportedException();
  }

  return new ClientSocket(channel, semantics, deallocator, name);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context != LOCAL){
    throw UnsupportedException();
  }

  return new ServerSocket(channel, semantics, deallocator);
}

}
}
