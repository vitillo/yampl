#include <cstdlib>

#include "ZMQ/ZMQSocketFactory.h"
#include "pipe/PipeSocketFactory.h"
#include "YAMPLSocketFactory.h"

namespace YAMPL{

SocketFactory::SocketFactory(){
  m_zmqFactory = new ZMQ::SocketFactory();
}

SocketFactory::~SocketFactory(){
  delete m_zmqFactory;
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  return m_zmqFactory->createClientSocket(channel, semantics, deallocator);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  return m_zmqFactory->createServerSocket(channel, semantics, deallocator);
}


}
