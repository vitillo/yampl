#include "PipeSocketFactory.h"
#include "PipeSocket.h"
#include "Exceptions.h"

namespace IPC{

ISocket *PipeSocketFactory::createProducerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new PipeProducerSocket(channel, ownership, m_zerocopy, deallocator);
}

ISocket *PipeSocketFactory::createConsumerSocket(Channel channel, bool ownership){
  return new PipeConsumerSocket(channel, ownership);
}

ISocket *PipeSocketFactory::createClientSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new PipeClientSocket(channel, ownership, m_zerocopy, deallocator);
}

ISocket *PipeSocketFactory::createServerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new PipeServerSocket(channel, ownership, m_zerocopy, deallocator);
}

}
