#include <cstdlib>

#include "yampl/zeromq/SocketFactory.h"
#include "yampl/pipe/SocketFactory.h"
#include "yampl/shm/SocketFactory.h"
#include "yampl/SocketFactory.h"

namespace yampl{

std::string DEFAULT_ID = "";
const std::string CONST_DEFAULT_ID = "";

SocketFactory::SocketFactory(){
  m_zmqFactory = new zeromq::SocketFactory();
  m_pipeFactory = new pipe::SocketFactory();
  m_shmFactory = new shm::SocketFactory();
}

SocketFactory::~SocketFactory(){
  delete m_zmqFactory;
  delete m_pipeFactory;
  delete m_shmFactory;
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name){
  if(channel.context == LOCAL_PIPE){
    return m_pipeFactory->createClientSocket(channel, semantics, deallocator);
  }else if(channel.context == LOCAL_SHM){
    return m_shmFactory->createClientSocket(channel, semantics, deallocator);
  }else{
    return m_zmqFactory->createClientSocket(channel, semantics, deallocator,name);
  }
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(channel.context == LOCAL_PIPE){
    return m_pipeFactory->createServerSocket(channel, semantics, deallocator);
  }else if(channel.context == LOCAL_SHM){
    return m_shmFactory->createServerSocket(channel, semantics, deallocator);
  }else{
    return m_zmqFactory->createServerSocket(channel, semantics, deallocator);
  }
}


}
