#include "yampl/Exceptions.h"

#include "yampl/shm/SHMSocketFactory.h"
#include "yampl/shm/SHMSocket.h"

namespace yampl{
namespace shm{

  ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context != LOCAL){
    throw UnsupportedException();
  }

  return new ClientSocket(channel, semantics, deallocator);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context != LOCAL){
    throw UnsupportedException();
  }

  return new ServerSocket(channel, semantics, deallocator);
}

}
}
