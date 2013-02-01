#include "yampl/Exceptions.h"

#include "yampl/shm/SocketFactory.h"
#include "yampl/shm/ClientSocket.h"
#include "yampl/shm/ServerSocket.h"

namespace yampl{
namespace shm{

  ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name){
  return new ClientSocket(channel, semantics, deallocator, name);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  return new ServerSocket(channel, semantics, deallocator);
}

}
}
