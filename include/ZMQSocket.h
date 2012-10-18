#ifndef ZMQSOCKET_H
#define ZMQSOCKET_H

#include "../Socket.h"

namespace zmq{
  class context_t;
  class socket_t;
}

namespace IPC{
namespace ZMQ{

class SocketBase : public ISocket{
  public:
    virtual ~SocketBase();

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

  protected:
    SocketBase(Channel channel, zmq::context_t *context, int type, bool ownership, void (*deallocator)(void *, void *) = defaultDeallocator);

    Channel m_channel;
    bool m_ownership;
    zmq::socket_t *m_socket;
    zmq::message_t *m_message;
    void (*m_deallocator)(void *, void *);
};

class ProducerSocket : public SocketBase{
  public:
    ProducerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));
    
    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      SocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }
};

class ConsumerSocket: public SocketBase{
  public:
    ConsumerSocket(Channel channel, zmq::context_t *context, bool ownership);

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      throw InvalidOperationException();
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return SocketBase::receive(buffer, size);
    }
};

class ClientSocket : public SocketBase{
  public:
    ClientSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      SocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return SocketBase::receive(buffer, size);
    }
};

class ServerSocket : public SocketBase{
  public:
    ServerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      SocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return SocketBase::receive(buffer, size);
    }
};

}
}

#endif
