#include "yampl/Exceptions.h"

#include "yampl/shm/SHMSocketFactory.h"
#include "yampl/shm/SHMSocket.h"

namespace yampl{
namespace shm{

  ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name){
  if(channel.context != LOCAL){
    throw UnsupportedException();
  }

  return new MOClientSocket(channel, semantics, deallocator, name);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context != LOCAL){
    throw UnsupportedException();
  }

  return new MOServerSocket(channel, semantics, deallocator);
}

}
}
