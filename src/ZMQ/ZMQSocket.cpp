#include <unistd.h>

#include <zmq.hpp>

#include "Channel.h"
#include "ZMQ/ZMQSocket.h"

using namespace std;

namespace IPC{
namespace ZMQ{

SocketBase::~SocketBase(){
  delete m_message;
  delete m_socket;
}

SocketBase::SocketBase(Channel channel, zmq::context_t *context, int type, Semantics semantics, void (*deallocator)(void *, void *)) : m_channel(channel), m_semantics(semantics), m_socket(0), m_message(new zmq::message_t()), m_deallocator(deallocator), m_received(false), m_connected(false), m_type(type){
  if(m_channel.context == LOCAL_PROCESS)
    m_channel.name = "ipc:///tmp/zmq_" + m_channel.name;
  else if(m_channel.context == THREAD)
    m_channel.name = "inproc://" + m_channel.name;
  else
    throw UnsupportedException();

  m_socket = new zmq::socket_t(*context, m_type);

  /* TODO: configure buffering
  if(type == ZMQ_PUSH) 
    m_socket->setsockopt(ZMQ_HWM, &m_channel.asynchronicity, sizeof(m_channel.asynchronicity));
  */
}

void SocketBase::connect(){
  Topology topo = m_channel.topology;

  if((topo == ONE_TO_ONE || topo == ONE_TO_MANY) && (m_type == ZMQ_PUSH || m_type == ZMQ_REQ))
    m_socket->bind((m_channel.name).c_str());
  else if(topo == MANY_TO_ONE && m_type == ZMQ_REP)
    m_socket->bind((m_channel.name).c_str());
  else{
    if(m_channel.context == THREAD){
      while(true){
	try{
	  m_socket->connect((m_channel.name).c_str());
	}catch(zmq::error_t err){
	  if(err.num() != ECONNREFUSED)
	    throw err;

	  usleep(1000);
	  continue;
	}

	break;
      }
    }else
      m_socket->connect((m_channel.name).c_str());
  }

  m_connected = true;
}

void SocketBase::send(void *buffer, size_t size, void *hint){
  if(!m_connected)
    connect();

  if(m_semantics == MOVE_DATA){
    zmq::message_t message((void *)buffer, size, m_deallocator, hint);
    m_socket->send(message);
  }else{
    zmq::message_t message(size);
    memcpy((void*)message.data(), buffer, size);
    m_socket->send(message);
  }
}

size_t SocketBase::recv(void **buffer, size_t size){
  if(!m_connected)
    connect();

  if(!m_received){
    m_socket->recv(m_message);
    m_received = true;
  }

  if(m_semantics == MOVE_DATA){
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

  m_received = false;
  return m_message->size();
}

ProducerSocket::ProducerSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *)) : SocketBase(channel, context, ZMQ_PUSH, semantics, deallocator){}

ConsumerSocket::ConsumerSocket(Channel channel, zmq::context_t *context, Semantics semantics) : SocketBase(channel, context, ZMQ_PULL, semantics, 0){}

ClientSocket::ClientSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *)) : SocketBase(channel, context, ZMQ_REQ, semantics, deallocator){}

ServerSocket::ServerSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *)) : SocketBase(channel, context, ZMQ_REP, semantics, deallocator){}

}
}
