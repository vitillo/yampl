#include <zmq.hpp>

#include "ZMQ/Socket.h"

namespace IPC{
namespace ZMQ{

SocketBase::~SocketBase(){
  delete m_message;
  delete m_socket;
}

SocketBase::SocketBase(Channel channel, zmq::context_t *context, int type, bool ownership, void (*deallocator)(void *, void *)) : m_channel(channel), m_ownership(ownership), m_message(new zmq::message_t()), m_deallocator(deallocator){

  Topology topo = m_channel.topology;
  m_socket = new zmq::socket_t(*context, type);
 
  /*
   * TODO: configure buffering
  if(type == ZMQ_PUSH) 
    m_socket->setsockopt(ZMQ_HWM, &m_channel.asynchronicity, sizeof(m_channel.asynchronicity));
  */

  if((topo == ONE_TO_ONE || topo == ONE_TO_MANY) && (type == ZMQ_PUSH || type == ZMQ_REQ))
    m_socket->bind(("ipc:///tmp/zmq_" + m_channel.name).c_str());
  else if(topo == MANY_TO_ONE && type == ZMQ_REP)
    m_socket->bind(("ipc:///tmp/zmq_" + m_channel.name).c_str());
  else
    m_socket->connect(("ipc:///tmp/zmq_" + m_channel.name).c_str());
}

void SocketBase::send(void *buffer, size_t size, void *hint){
  if(m_ownership){
    zmq::message_t message((void *)buffer, size, m_deallocator, hint);
    m_socket->send(message);
  }else{
    zmq::message_t message(size);
    memcpy((void*)message.data(), buffer, size);
    m_socket->send(message);
  }
}

size_t SocketBase::recv(void **buffer, size_t size){
  static bool received = false;

  if(!received){
    m_socket->recv(m_message);
    received = true;
  }

  if(m_ownership){
    *buffer = m_message->data();
  }else{
    if(*buffer && size < m_message->size())
      throw InvalidSizeException();
    else if(*buffer)
      memcpy(*buffer, m_message->data(), m_message->size());
    else{
      *buffer = malloc(m_message->size());
      memcpy(*buffer, m_message->data(), m_message->size());
    }
  }

  received = false;
  return m_message->size();
}

ProducerSocket::ProducerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *)) : SocketBase(channel, context, ZMQ_PUSH, ownership, deallocator){}

ConsumerSocket::ConsumerSocket(Channel channel, zmq::context_t *context, bool ownership) : SocketBase(channel, context, ZMQ_PULL, ownership, 0){}

ClientSocket::ClientSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *)) : SocketBase(channel, context, ZMQ_REQ, ownership, deallocator){}

ServerSocket::ServerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *)) : SocketBase(channel, context, ZMQ_REP, ownership, deallocator){}

}
}
