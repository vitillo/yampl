#include "ZMQSocketFactory.h"
#include "ZMQSocket.h"

using namespace IPC;

zmq::context_t *ZMQSocketFactory::m_context = 0;


ISocket *ZMQSocketFactory::createProducerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new ZMQProducerSocket(channel, m_context, ownership, deallocator);
}

ISocket *ZMQSocketFactory::createConsumerSocket(Channel channel, bool ownership){
  return new ZMQConsumerSocket(channel, m_context, ownership);
}

ISocket *ZMQSocketFactory::createClientSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new ZMQClientSocket(channel, m_context, ownership, deallocator);
}

ISocket *ZMQSocketFactory::createServerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new ZMQServerSocket(channel, m_context, ownership, deallocator);
}

