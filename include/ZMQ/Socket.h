#ifndef ZMQSOCKET_H
#define ZMQSOCKET_H

#include "../Channel.h"
#include "../Socket.h"
#include "../Exceptions.h"

namespace zmq{
  class context_t;
  class socket_t;
}

namespace IPC{
namespace ZMQ{

class SocketBase : public ISocket{
  public:
    SocketBase(const SocketBase &) = delete;
    virtual ~SocketBase();

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

    SocketBase & operator=(const SocketBase &) = delete;

  protected:
    SocketBase(Channel channel, zmq::context_t *context, int type, bool ownership, void (*deallocator)(void *, void *));

    Channel m_channel;
    bool m_ownership;
    zmq::socket_t *m_socket;
    zmq::message_t *m_message;
    void (*m_deallocator)(void *, void *);
};

class ProducerSocket : public SocketBase{
  public:
    ProducerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));
    ProducerSocket(const ProducerSocket &) = delete;
    
    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      SocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }

    ProducerSocket & operator=(const ProducerSocket &) = delete;
};

class ConsumerSocket: public SocketBase{
  public:
    ConsumerSocket(Channel channel, zmq::context_t *context, bool ownership);
    ConsumerSocket(const ConsumerSocket &) = delete;

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      throw InvalidOperationException();
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return SocketBase::receive(buffer, size);
    }

    ConsumerSocket & operator=(const ConsumerSocket &) = delete;
};

class ClientSocket : public SocketBase{
  public:
    ClientSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));
    ClientSocket(const ClientSocket &) = delete;

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      SocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return SocketBase::receive(buffer, size);
    }

    ClientSocket & operator=(const ClientSocket) = delete;
};

class ServerSocket : public SocketBase{
  public:
    ServerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));
    ServerSocket(const ServerSocket &) = delete;

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      SocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return SocketBase::receive(buffer, size);
    }

    ServerSocket & operator=(const ServerSocket &) = delete;
};

}
}

#endif
