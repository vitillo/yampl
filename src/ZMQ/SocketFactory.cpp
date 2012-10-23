#include <zmq.hpp>

#include "ZMQ/SocketFactory.h"
#include "ZMQ/Socket.h"

namespace IPC{
namespace ZMQ{

SocketFactory::SocketFactory() : m_context(new zmq::context_t(1)){
}

SocketFactory::~SocketFactory(){
  delete m_context;
}

ISocket *SocketFactory::createProducerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.topology == MANY_TO_MANY || channel.context == DISTRIBUTED_PROCESS)
    throw UnsupportedException();

  return new ProducerSocket(channel, m_context, semantics, deallocator);
}

ISocket *SocketFactory::createConsumerSocket(Channel channel, Semantics semantics){
  if(channel.topology == MANY_TO_MANY || channel.context == DISTRIBUTED_PROCESS)
    throw UnsupportedException();

  return new ConsumerSocket(channel, m_context, semantics);
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.topology == MANY_TO_MANY || channel.context == DISTRIBUTED_PROCESS)
    throw UnsupportedException();

  return new ClientSocket(channel, m_context, semantics, deallocator);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.topology == MANY_TO_MANY || channel.context == DISTRIBUTED_PROCESS)
    throw UnsupportedException();

  return new ServerSocket(channel, m_context, semantics, deallocator);
}

}
}
