#include <cstdlib>

#include "yampl/ZMQ/ZMQSocketFactory.h"
#include "yampl/pipe/PipeSocketFactory.h"
#include "yampl/SocketFactory.h"

namespace yampl{

SocketFactory::SocketFactory(){
  m_zmqFactory = new ZMQ::SocketFactory();
  m_pipeFactory = new pipe::SocketFactory();
}

SocketFactory::~SocketFactory(){
  delete m_zmqFactory;
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context == LOCAL){
    return m_pipeFactory->createClientSocket(channel, semantics, deallocator);
  }else{
    return m_zmqFactory->createClientSocket(channel, semantics, deallocator);
  }
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context == LOCAL){
    return m_pipeFactory->createServerSocket(channel, semantics, deallocator);
  }else{
    return m_zmqFactory->createServerSocket(channel, semantics, deallocator);
  }
}


}
