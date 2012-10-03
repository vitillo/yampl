#include "ZMQSocketFactory.h"
#include "ZMQSocket.h"

using namespace IPC;

ISocket *ZMQSocketFactory::createProducerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new ZMQProducerSocket(channel, ownership, deallocator);
}

ISocket *ZMQSocketFactory::createConsumerSocket(Channel channel, bool ownership){
  return new ZMQConsumerSocket(channel, ownership);
}

ISocket *ZMQSocketFactory::createClientSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new ZMQClientSocket(channel, ownership, deallocator);
}

ISocket *ZMQSocketFactory::createServerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  return new ZMQServerSocket(channel, ownership, deallocator);
}

