#include <zmq.hpp>

#include "yampl/Exceptions.h"

#include "yampl/ZMQ/ServerSocket.h"

namespace yampl{
namespace ZMQ{

ServerSocket::ServerSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *)) : SocketBase(channel, context, semantics, deallocator, ZMQ_ROUTER), m_lastAddress(new zmq::message_t){
  int mandatory = 1;
  m_socket->setsockopt(ZMQ_ROUTER_MANDATORY, &mandatory, sizeof(mandatory));
  m_socket->bind((m_channel.name).c_str());
}

ServerSocket::~ServerSocket(){
  delete m_lastAddress;
}

void ServerSocket::send(SendArgs& args){
  if(m_semantics == MOVE_DATA){
    zmq::message_t message(args.buffer, args.size, m_deallocator, args.hint);

    sendMessage(message, args.peerId);
  }else{
    zmq::message_t message(args.size);

    memcpy((void*)message.data(), args.buffer, args.size);
    sendMessage(message, args.peerId);
  }
}

void ServerSocket::sendMessage(zmq::message_t &message, const std::string *peerId){
  if(peerId){
    zmq::message_t address(peerId->size() + 1);
    memcpy((void*)address.data(), peerId->c_str(), peerId->size() + 1);
    if(m_socket->send(address, ZMQ_SNDMORE | ZMQ_DONTWAIT) == -1)
      throw UnroutableException();
  }else{
    if(m_socket->send(*m_lastAddress, ZMQ_SNDMORE | ZMQ_DONTWAIT) == -1)
      throw UnroutableException();
  }

  m_socket->send(message);
}

ssize_t ServerSocket::recv(RecvArgs& args){
  zmq::pollitem_t item = {*m_socket, 0, ZMQ_POLLIN, 0};

  if(!m_isRecvPending){
    zmq::poll(&item, 1, args.timeout);
  }

  if(item.revents & ZMQ_POLLIN || m_isRecvPending){
    if(!m_isRecvPending){
      m_socket->recv(m_lastAddress);
      m_socket->recv(m_message);
      m_isRecvPending = true;
    }

    if(m_semantics == MOVE_DATA){
      *args.buffer = m_message->data();
    }else{
      if(*args.buffer && args.size < m_message->size()){
	throw InvalidSizeException();
      }else if(*args.buffer){
	memcpy(*args.buffer, m_message->data(), m_message->size());
      }else{
	*args.buffer = malloc(m_message->size());
	memcpy(*args.buffer, m_message->data(), m_message->size());
      }
    }

    if(args.peerIdOut){
      *args.peerIdOut = (char *)m_lastAddress->data();
    }

    m_isRecvPending = false;
    return m_message->size();
  }else{
    return -1;
  }
}

}
}
