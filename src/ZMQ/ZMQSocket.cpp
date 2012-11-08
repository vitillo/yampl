#include <unistd.h>

#include <zmq.hpp>

#include "Channel.h"
#include "ZMQ/ZMQSocket.h"

using namespace std;

namespace YAMPL{
namespace ZMQ{
SocketBase::SocketBase(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *), int type) : m_channel(channel), m_semantics(semantics), m_socket(0), m_message(new zmq::message_t()), m_deallocator(deallocator){
  if(m_channel.context == LOCAL_PROCESS){
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

ClientSocket::ClientSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *)) : SocketBase(channel, context, semantics, deallocator, ZMQ_DEALER), m_isConnected(false){
}

ClientSocket::~ClientSocket(){
}

void ClientSocket::send(void *buffer, size_t size, const discriminator_t *discriminator, void *hint){
  if(!m_isConnected){
    connect();
  }

  if(m_semantics == MOVE_DATA){
    zmq::message_t message((void *)buffer, size, m_deallocator, hint);

    m_socket->send(message);
  }else{
    zmq::message_t message(size);

    memcpy((void*)message.data(), buffer, size);
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
	  throw err;
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
    
size_t ClientSocket::recv(void **buffer, size_t size, discriminator_t *discriminator){
  if(!m_isConnected){
    connect();
  }

  m_socket->recv(m_message); 

  if(m_semantics == MOVE_DATA){
    *buffer = m_message->data();
  }else{
    if(*buffer && size < m_message->size()){
      throw InvalidSizeException();
    }else if(*buffer){
      memcpy(*buffer, m_message->data(), m_message->size());
    }else{
      *buffer = malloc(m_message->size());
      memcpy(*buffer, m_message->data(), m_message->size());
    }
  }

  return m_message->size();
}

ServerSocket::ServerSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *)) : SocketBase(channel, context, semantics, deallocator, ZMQ_ROUTER), m_lastAddress(new zmq::message_t){
  m_socket->bind((m_channel.name).c_str());
}

ServerSocket::~ServerSocket(){
  delete m_lastAddress;
}

void ServerSocket::send(void *buffer, size_t size, const discriminator_t *discriminator, void *hint){
  if(m_semantics == MOVE_DATA){
    zmq::message_t message((void *)buffer, size, m_deallocator, hint);

    sendMessage(message, discriminator);
  }else{
    zmq::message_t message(size);

    memcpy((void*)message.data(), buffer, size);
    sendMessage(message, discriminator);
  }
}

void ServerSocket::sendMessage(zmq::message_t &message, const discriminator_t *discriminator){
  if(discriminator){
    zmq::message_t address(discriminator->size());
    memcpy((void*)address.data(), discriminator->c_str(), discriminator->size());
    m_socket->send(address, ZMQ_SNDMORE);
  }else{
    m_socket->send(*m_lastAddress, ZMQ_SNDMORE);
  }

  m_socket->send(message);
}

size_t ServerSocket::recv(void **buffer, size_t size, discriminator_t *discriminator){
  m_socket->recv(m_lastAddress);
  m_socket->recv(m_message);

  if(m_semantics == MOVE_DATA){
    *buffer = m_message->data();
  }else{
    if(*buffer && size < m_message->size()){
      throw InvalidSizeException();
    }else if(*buffer){
      memcpy(*buffer, m_message->data(), m_message->size());
    }else{
      *buffer = malloc(m_message->size());
      memcpy(*buffer, m_message->data(), m_message->size());
    }
  }

  if(discriminator){
    *discriminator = (char *)m_lastAddress->data();
  }

  return m_message->size();
}

}
}
