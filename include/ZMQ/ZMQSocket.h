#ifndef IPC_ZMQ_SOCKET_H
#define IPC_ZMQ_SOCKET_H

#include "Channel.h"
#include "ISocket.h"
#include "ISocketFactory.h"
#include "Exceptions.h"

namespace zmq{
  class context_t;
  class socket_t;
}

namespace IPC{
namespace ZMQ{

class SocketBase : public ISocket{
  public:
    virtual ~SocketBase();

    virtual void send(void *buffer, size_t size, void *hint = NULL);
    virtual size_t recv(void **buffer, size_t size = 0);

  protected:
    SocketBase(Channel channel, zmq::context_t *context, int type, Semantics semantics, void (*deallocator)(void *, void *));

    Channel m_channel;
    Semantics m_semantics;
    zmq::socket_t *m_socket;
    zmq::message_t *m_message;
    void (*m_deallocator)(void *, void *);

  private:
    SocketBase(const SocketBase &);
    SocketBase & operator=(const SocketBase &);
    void connect();

    bool m_received;
    bool m_connected;
    int m_type;
};

class ProducerSocket : public SocketBase{
  public:
    ProducerSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *));
    
    virtual void send(void *buffer, size_t size, void *hint = NULL){
      SocketBase::send(buffer, size, hint);
    }

    virtual size_t recv(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }

  private:
    ProducerSocket(const ProducerSocket &);
    ProducerSocket & operator=(const ProducerSocket &);
};

class ConsumerSocket: public SocketBase{
  public:
    ConsumerSocket(Channel channel, zmq::context_t *context, Semantics semantics);

    virtual void send(void *buffer, size_t size, void *hint = NULL){
      throw InvalidOperationException();
    }

    virtual size_t recv(void **buffer, size_t size = 0){
      return SocketBase::recv(buffer, size);
    }

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);
};

class ClientSocket : public SocketBase{
  public:
    ClientSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *));

    virtual void send(void *buffer, size_t size, void *hint = NULL){
      SocketBase::send(buffer, size, hint);
    }

    virtual size_t recv(void **buffer, size_t size = 0){
      return SocketBase::recv(buffer, size);
    }

  private:
    ClientSocket(const ClientSocket &);
    ClientSocket & operator=(const ClientSocket);
};

class ServerSocket : public SocketBase{
  public:
    ServerSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *));

    virtual void send(void *buffer, size_t size, void *hint = NULL){
      SocketBase::send(buffer, size, hint);
    }

    virtual size_t recv(void **buffer, size_t size = 0){
      return SocketBase::recv(buffer, size);
    }

  private:
    ServerSocket(const ServerSocket &);
    ServerSocket & operator=(const ServerSocket &);
};

}
}

#endif
