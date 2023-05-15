#include <zmq.hpp>

#include "yampl/Exceptions.h"

#include "yampl/zeromq/ClientSocket.h"

namespace yampl{
namespace zeromq{

ClientSocket::ClientSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : SocketBase(channel, context, semantics, deallocator, ZMQ_DEALER), m_isConnected(false){
  if(&name == &CONST_DEFAULT_ID){
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

}
}
