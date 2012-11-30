#include <zmq.hpp>

#include "yampl/ZMQ/SocketFactory.h"
#include "yampl/ZMQ/ClientSocket.h"
#include "yampl/ZMQ/ServerSocket.h"

namespace yampl{
namespace ZMQ{

SocketFactory::SocketFactory() : m_context(new zmq::context_t(1)){
}

SocketFactory::~SocketFactory(){
  delete m_context;
}

ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name){
  return new ClientSocket(channel, m_context, semantics, deallocator, name);
}

ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  return new ServerSocket(channel, m_context, semantics, deallocator);
}

}
}
