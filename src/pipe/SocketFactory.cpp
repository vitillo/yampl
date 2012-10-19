#include "Exceptions.h"

#include "pipe/SocketFactory.h"
#include "pipe/Socket.h"

namespace IPC{
namespace pipe{

ISocket *SocketFactory::createProducerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new ProducerSocket(channel, ownership, m_zerocopy, deallocator);
}

ISocket *SocketFactory::createConsumerSocket(Channel channel, bool ownership){
  return new ConsumerSocket(channel, ownership);
}

ISocket *SocketFactory::createClientSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  switch(channel.topology){
    case ONE_TO_ONE:
      return new ClientSocket(channel, ownership, m_zerocopy, deallocator);

    case MANY_TO_ONE:
      return new MOClientSocket(channel, ownership, m_zerocopy, deallocator);

    default:
      throw UnsupportedException();
  }
}

ISocket *SocketFactory::createServerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  switch(channel.topology){
    case ONE_TO_ONE:
      return new ServerSocket(channel, ownership, m_zerocopy, deallocator);
    
    case MANY_TO_ONE:
      return new MOServerSocket(channel, ownership, m_zerocopy, deallocator);

    default:
      throw UnsupportedException();
  }
}

}
}
