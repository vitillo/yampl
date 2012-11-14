#include "yampl/ZMQ/ZMQSocketFactory.h"
#include "yampl/Socket.h"

namespace yampl{

ISocketFactory *ClientSocket::s_zmqFactory = 0;

ClientSocket::ClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(!s_zmqFactory){
    s_zmqFactory = new ZMQ::SocketFactory();
  }

  m_socket = s_zmqFactory->createClientSocket(channel, semantics, deallocator);
}

ClientSocket::~ClientSocket(){
}

void ClientSocket::send(void *buffer, size_t size, discriminator_t *discriminator, void *hint){
  return m_socket->send(buffer, size, discriminator, hint);
}

ssize_t ClientSocket::recv(void **buffer, size_t size, discriminator_t *discriminator){
  return m_socket->recv(buffer, size, discriminator);
}

ISocketFactory *ServerSocket::s_zmqFactory = 0;

ServerSocket::ServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)){
  if(!s_zmqFactory){
    s_zmqFactory = new ZMQ::SocketFactory();
  }

  m_socket = s_zmqFactory->createServerSocket(channel, semantics, deallocator);
}

ServerSocket::~ServerSocket(){
}

void ServerSocket::send(void *buffer, size_t size, discriminator_t *discriminator, void *hint){
  return m_socket->send(buffer, size, discriminator, hint);
}

ssize_t ServerSocket::recv(void **buffer, size_t size, discriminator_t *discriminator){
  return m_socket->recv(buffer, size, discriminator);
}

}
