#include <zmq.hpp>

#include "ZMQSocket.h"

using namespace IPC;

ZMQBaseSocket::~ZMQBaseSocket(){
  delete m_socket;
}

ZMQBaseSocket::ZMQBaseSocket(Channel channel, zmq::context_t *context, int type, bool ownership, void (*deallocator)(void *, void *)) : m_channel(channel), m_ownership(ownership), m_deallocator(deallocator){
  if(m_channel.topology == MANY_TO_MANY)
    throw UnsupportedException();

  Topology topo = m_channel.topology;
  m_socket = new zmq::socket_t(*context, type);
 
  if(type == ZMQ_PUSH) 
    m_socket->setsockopt(ZMQ_HWM, &m_channel.asynchronicity, sizeof(m_channel.asynchronicity));

  if((topo == ONE_TO_ONE || topo == ONE_TO_MANY) && (type == ZMQ_PUSH || type == ZMQ_REQ))
    m_socket->bind(("ipc:///tmp/zmq_" + m_channel.name).c_str());
  else if(topo == MANY_TO_ONE && type == ZMQ_REP)
    m_socket->bind(("ipc:///tmp/zmq_" + m_channel.name).c_str());
  else
    m_socket->connect(("ipc:///tmp/zmq_" + m_channel.name).c_str());
}

void ZMQBaseSocket::send(const void *buffer, size_t size, void *hint){
  if(m_ownership){
    zmq::message_t message((void *)buffer, size, m_deallocator, hint);
    m_socket->send(message);
  }else{
    zmq::message_t message(size);
    memcpy((void*)message.data(), buffer, size);
    m_socket->send(message);
  }
}

size_t ZMQBaseSocket::receive(void **buffer, size_t size){
  static zmq::message_t message;
  static bool received = false;

  if(!received){
    m_socket->recv(&message);
    received = true;
  }

  if(m_ownership){
    *buffer = message.data();
  }else{
    if(*buffer && size < message.size())
      throw InvalidSizeException();
    else if(*buffer)
      memcpy(*buffer, message.data(), message.size());
    else{
      *buffer = malloc(message.size());
      memcpy(*buffer, message.data(), message.size());
    }
  }

  received = false;
  return message.size();
}

ZMQProducerSocket::ZMQProducerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *)) : ZMQBaseSocket(channel, context, ZMQ_PUSH, ownership, deallocator){}

ZMQConsumerSocket::ZMQConsumerSocket(Channel channel, zmq::context_t *context, bool ownership) : ZMQBaseSocket(channel, context, ZMQ_PULL, ownership){}

ZMQClientSocket::ZMQClientSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *)) : ZMQBaseSocket(channel, context, ZMQ_REQ, ownership, deallocator){}

ZMQServerSocket::ZMQServerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *)) : ZMQBaseSocket(channel, context, ZMQ_REP, ownership, deallocator){}


