#include <zmq.hpp>

#include "ZMQSocketFactory.h"
#include "ZMQSocket.h"

using namespace IPC;

ZMQSocketFactory::ZMQSocketFactory() : m_context(new zmq::context_t(1)){
}

ZMQSocketFactory::~ZMQSocketFactory(){
  delete m_context;
}

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

