#include "Exceptions.h"

#include "pipe/PipeSocketFactory.h"
#include "pipe/PipeSocket.h"

namespace YAMPL{
namespace pipe{

ISocket *SocketFactory::createProducerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.topology != ONE_TO_ONE || channel.context != LOCAL_PROCESS)
    throw UnsupportedException();

  return new ProducerSocket(channel, semantics, m_zerocopy, deallocator);
}

ISocket *SocketFactory::createConsumerSocket(Channel channel, Semantics semantics){
  if(channel.topology != ONE_TO_ONE || channel.context != LOCAL_PROCESS)
    throw UnsupportedException();

  return new ConsumerSocket(channel, semantics);
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context != LOCAL_PROCESS)
    throw UnsupportedException();

  switch(channel.topology){
    case ONE_TO_ONE:
      return new ClientSocket(channel, semantics, m_zerocopy, deallocator);

    case MANY_TO_ONE:
      return new MOClientSocket(channel, semantics, m_zerocopy, deallocator);

    default:
      throw UnsupportedException();
  }
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context != LOCAL_PROCESS)
    throw UnsupportedException();

  switch(channel.topology){
    case ONE_TO_ONE:
      return new ServerSocket(channel, semantics, m_zerocopy, deallocator);
    
    case MANY_TO_ONE:
      return new MOServerSocket(channel, semantics, m_zerocopy, deallocator);

    default:
      throw UnsupportedException();
  }
}

}
}
