#include <zmq.hpp>

#include "yampl/zeromq/SocketBase.h"

namespace yampl{
namespace zeromq{

SocketBase::SocketBase(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *), int type) : m_channel(channel), m_semantics(semantics), m_deallocator(deallocator), m_socket(0), m_message(new zmq::message_t()), m_isRecvPending(false){
  if(m_channel.context == LOCAL){
    m_channel.name = "ipc:///tmp/zmq_" + m_channel.name;
  }else if(m_channel.context == THREAD){
    m_channel.name = "inproc://" + m_channel.name;
  }else{
    m_channel.name = "tcp://" + m_channel.name;
  }

  m_socket = new zmq::socket_t(*context, type);
}

SocketBase::~SocketBase(){
  delete m_message;
  delete m_socket;
}

}
}
