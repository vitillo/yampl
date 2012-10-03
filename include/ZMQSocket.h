#ifndef IPC_ZMQSOCKET_H
#define IPC_ZMQSOCKET_H

#include "Socket.h"

namespace IPC{

class ZMQBaseSocket : public ISocket{
  public:
    virtual ~ZMQBaseSocket(){
      delete m_socket;
    }

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual int receive(void **buffer, size_t size = 0);

  protected:
    ZMQBaseSocket(Channel channel, int type, bool ownership, void (*deallocator)(void *, void *) = defaultDeallocator);

    Channel m_channel;
    bool m_ownership;
    zmq::socket_t *m_socket;
    void (*m_deallocator)(void *, void *);
};

class ZMQProducerSocket : public ZMQBaseSocket{
  public:
    ZMQProducerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)) : ZMQBaseSocket(channel, ZMQ_PUSH, ownership, deallocator){
    }
    
    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      ZMQBaseSocket::send(buffer, size, hint);
    }

    virtual int receive(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }
};

class ZMQConsumerSocket: public ZMQBaseSocket{
  public:
    ZMQConsumerSocket(Channel channel, bool ownership) : ZMQBaseSocket(channel, ZMQ_PULL, ownership){
    }

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      throw InvalidOperationException();
    }

    virtual int receive(void **buffer, size_t size = 0){
      return ZMQBaseSocket::receive(buffer, size);
    }
};

class ZMQClientSocket : public ZMQBaseSocket{
  public:
    ZMQClientSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)) : ZMQBaseSocket(channel, ZMQ_REQ, ownership, deallocator){}

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      ZMQBaseSocket::send(buffer, size, hint);
    }

    virtual int receive(void **buffer, size_t size = 0){
      return ZMQBaseSocket::receive(buffer, size);
    }
};

class ZMQServerSocket : public ZMQBaseSocket{
  public:
    ZMQServerSocket(Channel channel, bool ownership, void (*deallocator)(void *, void *)) : ZMQBaseSocket(channel, ZMQ_REP, ownership, deallocator){}

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      ZMQBaseSocket::send(buffer, size, hint);
    }

    virtual int receive(void **buffer, size_t size = 0){
      return ZMQBaseSocket::receive(buffer, size);
    }
};

}

#endif
