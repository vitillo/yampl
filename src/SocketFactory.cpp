#include <cstdlib>

#include "yampl/ZMQ/SocketFactory.h"
#include "yampl/pipe/SocketFactory.h"
#include "yampl/SocketFactory.h"

namespace yampl{

std::string DEFAULT_ID = "";

SocketFactory::SocketFactory(){
  m_zmqFactory = new ZMQ::SocketFactory();
  m_pipeFactory = new pipe::SocketFactory();
}

SocketFactory::~SocketFactory(){
  delete m_zmqFactory;
  delete m_pipeFactory;
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name){
  return m_zmqFactory->createClientSocket(channel, semantics, deallocator, name);

  /*if(channel.context == LOCAL){
    return m_pipeFactory->createClientSocket(channel, semantics, deallocator);
  }else{
    return m_zmqFactory->createClientSocket(channel, semantics, deallocator);
  }*/
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  return m_zmqFactory->createServerSocket(channel, semantics, deallocator);

  /*if(channel.context == LOCAL){
    return m_pipeFactory->createServerSocket(channel, semantics, deallocator);
  }else{
    return m_zmqFactory->createServerSocket(channel, semantics, deallocator);
  }*/
}


}
