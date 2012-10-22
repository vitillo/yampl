#include <cstdlib>

#include "ZMQ/SocketFactory.h"
#include "pipe/SocketFactory.h"

#include "SocketFactory.h"

namespace IPC{

void defaultDeallocator(void *buffer, void *hint){
  free(buffer);
}

SocketFactory::SocketFactory(){
  m_zmqFactory = new ZMQ::SocketFactory();
  m_pipeFactory = new pipe::SocketFactory(true);
}

SocketFactory::~SocketFactory(){
  delete m_zmqFactory;
  delete m_pipeFactory;
}

ISocket *SocketFactory::createProducerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  if(channel.topology == ONE_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createProducerSocket(channel, ownership, deallocator);
  else
    return m_zmqFactory->createProducerSocket(channel, ownership, deallocator);
}

ISocket *SocketFactory::createConsumerSocket(Channel channel, bool ownership){
  if(channel.topology == ONE_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createConsumerSocket(channel, ownership);
  else
    return m_zmqFactory->createConsumerSocket(channel, ownership);
}

ISocket *SocketFactory::createClientSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  if(channel.topology == ONE_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createClientSocket(channel, ownership, deallocator);
  /*else if(channel.topology == MANY_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createClientSocket(channel, ownership, deallocator);*/
  else
    return m_zmqFactory->createClientSocket(channel, ownership, deallocator);
}

ISocket *SocketFactory::createServerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)){
  if(channel.topology == ONE_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createServerSocket(channel, ownership, deallocator);
  /*else if(channel.topology == MANY_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createServerSocket(channel, ownership, deallocator);*/
  else
    return m_zmqFactory->createServerSocket(channel, ownership, deallocator);
}


}
