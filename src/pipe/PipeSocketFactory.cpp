#include "Exceptions.h"

#include "pipe/PipeSocketFactory.h"
#include "pipe/PipeSocket.h"

namespace YAMPL{
namespace pipe{

  ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context != LOCAL_PROCESS){
    throw UnsupportedException();
  }

  return new MOClientSocket(channel, semantics, m_zerocopy, deallocator);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context != LOCAL_PROCESS){
    throw UnsupportedException();
  }

  return new MOServerSocket(channel, semantics, m_zerocopy, deallocator);
}

}
}
