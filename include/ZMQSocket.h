#ifndef IPC_ZMQSOCKET_H
#define IPC_ZMQSOCKET_H

#include "Socket.h"

namespace zmq{
  class context_t;
  class socket_t;
}

namespace IPC{

class ZMQBaseSocket : public ISocket{
  public:
    virtual ~ZMQBaseSocket();

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

  protected:
    ZMQBaseSocket(Channel channel, zmq::context_t *context, int type, bool ownership, void (*deallocator)(void *, void *) = defaultDeallocator);

    Channel m_channel;
    bool m_ownership;
    zmq::socket_t *m_socket;
    void (*m_deallocator)(void *, void *);
};

class ZMQProducerSocket : public ZMQBaseSocket{
  public:
    ZMQProducerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));
    
    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      ZMQBaseSocket::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }
};

class ZMQConsumerSocket: public ZMQBaseSocket{
  public:
    ZMQConsumerSocket(Channel channel, zmq::context_t *context, bool ownership);

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      throw InvalidOperationException();
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return ZMQBaseSocket::receive(buffer, size);
    }
};

class ZMQClientSocket : public ZMQBaseSocket{
  public:
    ZMQClientSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      ZMQBaseSocket::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return ZMQBaseSocket::receive(buffer, size);
    }
};

class ZMQServerSocket : public ZMQBaseSocket{
  public:
    ZMQServerSocket(Channel channel, zmq::context_t *context, bool ownership, void (*deallocator)(void *, void *));

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      ZMQBaseSocket::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return ZMQBaseSocket::receive(buffer, size);
    }
};

}

#endif
