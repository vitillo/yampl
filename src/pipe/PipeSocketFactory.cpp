#include "yampl/Exceptions.h"

#include "yampl/pipe/SocketFactory.h"
#include "yampl/pipe/ServerSocket.h"
#include "yampl/pipe/ClientSocket.h"

namespace yampl{
namespace pipe{

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name){
  return new ClientSocket(channel, semantics, deallocator, name);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  return new ServerSocket(channel, semantics, deallocator);
}

}
}
