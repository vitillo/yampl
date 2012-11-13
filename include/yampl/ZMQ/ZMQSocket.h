#ifndef YAMPL_ZMQ_SOCKET_H
#define YAMPL_ZMQ_SOCKET_H

#include "yampl/Channel.h"
#include "yampl/ISocket.h"
#include "yampl/ISocketFactory.h"
#include "yampl/Exceptions.h"

namespace zmq{
  class context_t;
  class socket_t;
  class message_t;
}

namespace yampl{
namespace ZMQ{

class SocketBase : public ISocket{
  public:
    typedef ISocket::discriminator_t discriminator_t;

    virtual ~SocketBase();
    
    virtual void send(void *buffer, size_t size, const discriminator_t *discriminator = 0, void *hint = 0) = 0;
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0) = 0;

  protected:
    SocketBase(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *), int type);

    Channel m_channel;
    Semantics m_semantics;
    zmq::socket_t *m_socket;
    zmq::message_t *m_message;
    void (*m_deallocator)(void *, void *);

  private:
    SocketBase(const SocketBase &);
    SocketBase & operator=(const SocketBase &);
};

class ClientSocket : public SocketBase{
  public:
    typedef SocketBase::discriminator_t discriminator_t;

    ClientSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *));
    virtual ~ClientSocket();

    virtual void send(void *buffer, size_t size, const discriminator_t *discriminator = 0, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0);

  private:
    bool m_isConnected;

    ClientSocket(const ClientSocket &);
    ClientSocket & operator=(const ClientSocket &);
    void connect();
 };

class ServerSocket : public SocketBase{
  public:
    typedef SocketBase::discriminator_t discriminator_t;

    ServerSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *));
    virtual ~ServerSocket();
 
    virtual void send(void *buffer, size_t size, const discriminator_t *discriminator = 0, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0);

  private:
    ServerSocket(const ClientSocket &);
    ServerSocket & operator=(const ClientSocket &);
    void sendMessage(zmq::message_t &message, const discriminator_t *discriminator);

    zmq::message_t *m_lastAddress;
 };

}
}

#endif
