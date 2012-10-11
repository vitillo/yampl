#ifndef ZMQSOCKET_H
#define ZMQSOCKET_H

#include "Socket.h"

namespace zmq{
  class context_t;
  class socket_t;
}

namespace IPC{

class ZMQSocketBase : public ISocket{
  public:
    virtual ~ZMQSocketBase();

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

  protected:
    ZMQSocketBase(Channel channel, zmq::context_t *context, int type, bool ownership, void (*deallocator)(void *, void *) = defaultDeallocator);

    Channel m_channel;
    bool m_ownership;
    zmq::socket_t *m_socket;
    zmq::message_t *m_message;
    void (*m_deallocator)(void *, void *);
};

class ZMQProducerSocket : public ZMQSocketBase{
  public:
    ZMQProducerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));
    
    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      ZMQSocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }
};

class ZMQConsumerSocket: public ZMQSocketBase{
  public:
    ZMQConsumerSocket(Channel channel, zmq::context_t *context, bool ownership);

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      throw InvalidOperationException();
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return ZMQSocketBase::receive(buffer, size);
    }
};

class ZMQClientSocket : public ZMQSocketBase{
  public:
    ZMQClientSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      ZMQSocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return ZMQSocketBase::receive(buffer, size);
    }
};

class ZMQServerSocket : public ZMQSocketBase{
  public:
    ZMQServerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      ZMQSocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return ZMQSocketBase::receive(buffer, size);
    }
};

}

#endif
