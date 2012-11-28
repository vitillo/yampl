#include <unistd.h>

#include <zmq.hpp>

#include "yampl/Channel.h"
#include "yampl/ZMQ/ZMQSocket.h"

using namespace std;

namespace yampl{
namespace ZMQ{
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

ClientSocket::ClientSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : SocketBase(channel, context, semantics, deallocator, ZMQ_DEALER), m_isConnected(false){
  if(&name == &DEFAULT_ID){
    size_t ids = 255;
    char id[ids];
    m_socket->getsockopt(ZMQ_IDENTITY, id, &ids);
  }else{
    m_socket->setsockopt(ZMQ_IDENTITY, name.c_str(), name.size() + 1);
  }
}

ClientSocket::~ClientSocket(){
}

void ClientSocket::send(SendArgs& args){
  if(!m_isConnected){
    connect();
  }

  if(m_semantics == MOVE_DATA){
    zmq::message_t message(args.buffer, args.size, m_deallocator, args.hint);

    m_socket->send(message);
  }else{
    zmq::message_t message(args.size);

    memcpy((void*)message.data(), args.buffer, args.size);
    m_socket->send(message);
   }
}

void ClientSocket::connect(){
  if(m_channel.context == THREAD){
    while(true){
      try{
	m_socket->connect((m_channel.name).c_str());
      }catch(zmq::error_t err){
	if(err.num() != ECONNREFUSED){
	  throw ErrnoException();
	}

	usleep(1000);
	continue;
      }

      break;
    }
  }else{
    m_socket->connect((m_channel.name).c_str());
  }

  m_isConnected = true;
}

ssize_t ClientSocket::recv(RecvArgs& args){
  if(!m_isConnected){
    connect();
  }

  zmq::pollitem_t item = {*m_socket, 0, ZMQ_POLLIN, 0};
  
  if(!m_isRecvPending){
    zmq::poll(&item, 1, args.timeout);
  }

  if(item.revents & ZMQ_POLLIN || m_isRecvPending){
    if(!m_isRecvPending){
      m_socket->recv(m_message); 
      m_isRecvPending = true;
    }

    if(m_semantics == MOVE_DATA){
      *args.buffer = m_message->data();
    }else{
      if(args.allocate){
	*args.buffer = malloc(m_message->size());
	memcpy(*args.buffer, m_message->data(), m_message->size());
      }else if(args.size >= m_message->size()){
	memcpy(*args.buffer, m_message->data(), m_message->size());
      }else{
	throw InvalidSizeException();
      }
    }

    m_isRecvPending = false;
    return m_message->size();
  }else{
    return -1;
  }
}

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
    if(m_socket->send(address, ZMQ_SNDMORE) == -1)
      throw UnroutableException();
  }else{
    if(m_socket->send(*m_lastAddress, ZMQ_SNDMORE) == -1)
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
