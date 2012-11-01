#include <cstdlib>

#include "ZMQ/ZMQSocketFactory.h"
#include "pipe/PipeSocketFactory.h"

#include "SocketFactory.h"

namespace IPC{

SocketFactory::SocketFactory(){
  m_zmqFactory = new ZMQ::SocketFactory();
  m_pipeFactory = new pipe::SocketFactory(true);
}

SocketFactory::~SocketFactory(){
  delete m_zmqFactory;
  delete m_pipeFactory;
}

ISocket *SocketFactory::createProducerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.topology == ONE_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createProducerSocket(channel, semantics, deallocator);
  else
    return m_zmqFactory->createProducerSocket(channel, semantics, deallocator);
}

ISocket *SocketFactory::createConsumerSocket(Channel channel, Semantics semantics){
  if(channel.topology == ONE_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createConsumerSocket(channel, semantics);
  else
    return m_zmqFactory->createConsumerSocket(channel, semantics);
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.topology == ONE_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createClientSocket(channel, semantics, deallocator);
  /*else if(channel.topology == MANY_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createClientSocket(channel, semantics, deallocator);*/
  else
    return m_zmqFactory->createClientSocket(channel, semantics, deallocator);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.topology == ONE_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createServerSocket(channel, semantics, deallocator);
  /*else if(channel.topology == MANY_TO_ONE && channel.context == LOCAL_PROCESS)
    return m_pipeFactory->createServerSocket(channel, semantics, deallocator);*/
  else
    return m_zmqFactory->createServerSocket(channel, semantics, deallocator);
}


}
