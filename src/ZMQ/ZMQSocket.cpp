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

  size_t ids = 255;
  char id[ids];
  m_socket->getsockopt(ZMQ_IDENTITY, id, &ids);
  m_identity = id;
}

SocketBase::~SocketBase(){
  delete m_message;
  delete m_socket;
}

ClientSocket::ClientSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *)) : SocketBase(channel, context, semantics, deallocator, ZMQ_DEALER), m_isConnected(false){
}

ClientSocket::~ClientSocket(){
}

void ClientSocket::send(void *buffer, size_t size, const std::string &peerID, void *hint){
  try_send(buffer, size, peerID, hint, -1);
}

bool ClientSocket::try_send(void *buffer, size_t size, const std::string &peerID, void *hint, long timeout){
  if(!m_isConnected){
    connect();
  }

  zmq::pollitem_t item = {*m_socket, 0, ZMQ_POLLOUT, 0};
  zmq::poll(&item, 1, timeout);
  
  if(item.revents & ZMQ_POLLOUT){
    if(m_semantics == MOVE_DATA){
      zmq::message_t message((void *)buffer, size, m_deallocator, hint);

      m_socket->send(message);
    }else{
      zmq::message_t message(size);

      memcpy((void*)message.data(), buffer, size);
      m_socket->send(message);
     }
    return true;
  }else{
    return false;
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
    
ssize_t ClientSocket::recv(void *&buffer, size_t size, const std::string *&peerID){
  return try_recv(buffer, size, peerID, -1);
}


ssize_t ClientSocket::try_recv(void *&buffer, size_t size, const std::string *&peerID, long timeout){
  if(!m_isConnected){
    connect();
  }

  zmq::pollitem_t item = {*m_socket, 0, ZMQ_POLLIN, 0};
  zmq::poll(&item, 1, timeout);

  if(item.revents & ZMQ_POLLIN){
    if(!m_isRecvPending){
      m_socket->recv(m_message); 
      m_isRecvPending = true;
    }

    if(m_semantics == MOVE_DATA){
      buffer = m_message->data();
    }else{
      if(buffer && size < m_message->size()){
	throw InvalidSizeException();
      }else if(buffer){
	memcpy(buffer, m_message->data(), m_message->size());
      }else{
	buffer = malloc(m_message->size());
	memcpy(buffer, m_message->data(), m_message->size());
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
  m_socket->setsockopt(ZMQ_ROUTER_MANDATORY, &mandatory, sizeof (mandatory));
  m_socket->bind((m_channel.name).c_str());
}

ServerSocket::~ServerSocket(){
  delete m_lastAddress;
}

void ServerSocket::send(void *buffer, size_t size, const std::string &peerID, void *hint){
  try_send(buffer, size, peerID, hint, -1);
}


bool ServerSocket::try_send(void *buffer, size_t size, const std::string &peerID, void *hint, long timeout){
  zmq::pollitem_t item = {*m_socket, 0, ZMQ_POLLOUT, 0};
  zmq::poll(&item, 1, timeout);

  if(item.revents & ZMQ_POLLOUT){
    if(m_semantics == MOVE_DATA){
      zmq::message_t message((void *)buffer, size, m_deallocator, hint);

      sendMessage(message, peerID);
    }else{
      zmq::message_t message(size);

      memcpy((void*)message.data(), buffer, size);
      sendMessage(message, peerID);
    }

    return true;
  }else{
    return false;
  }
}

void ServerSocket::sendMessage(zmq::message_t &message, const std::string &peerID){
  if(&peerID != &NO_ID){
    zmq::message_t address(peerID.size());
    memcpy((void*)address.data(), peerID.c_str(), peerID.size());
    if(m_socket->send(address, ZMQ_SNDMORE) == -1)
      throw UnroutableException();
  }else{
    if(m_socket->send(*m_lastAddress, ZMQ_SNDMORE) == -1)
      throw UnroutableException();
  }

  m_socket->send(message);
}

ssize_t ServerSocket::recv(void *&buffer, size_t size, const std::string *&peerID){
  return try_recv(buffer, size, peerID, -1);
}

ssize_t ServerSocket::try_recv(void *&buffer, size_t size, const std::string *&peerID, long timeout){
  zmq::pollitem_t item = {*m_socket, 0, ZMQ_POLLIN, 0};
  zmq::poll(&item, 1, timeout);

  if(item.revents & ZMQ_POLLIN){
    if(!m_isRecvPending){
      m_socket->recv(m_lastAddress);
      m_socket->recv(m_message);
      m_isRecvPending = true;
    }

    if(m_semantics == MOVE_DATA){
      buffer = m_message->data();
    }else{
      if(buffer && size < m_message->size()){
	throw InvalidSizeException();
      }else if(buffer){
	memcpy(buffer, m_message->data(), m_message->size());
      }else{
	buffer = malloc(m_message->size());
	memcpy(buffer, m_message->data(), m_message->size());
      }
    }

    if(&peerID != &NO_ID_PTR){
      if(peerID){
	throw UnsupportedException();
      }else{
	//TODO: return always the same instance!
	peerID = new string((char *)m_lastAddress->data());
      }
    }

    m_isRecvPending = false;
    return m_message->size();
  }else{
    return -1;
  }
}

}
}
