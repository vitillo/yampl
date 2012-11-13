#ifndef YAMPL_SOCKET_H
#define YAMPL_SOCKET_H

#include "yampl/ISocket.h"
#include "yampl/ISocketFactory.h"

namespace yampl{

class ClientSocket : public ISocket{
  public:
    typedef ISocket::discriminator_t discriminator_t;

    ClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ~ClientSocket();

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0);

  private:
    static ISocketFactory *s_zmqFactory;
    ISocket *m_socket;
};

class ServerSocket : public ISocket{
  public:
    typedef ISocket::discriminator_t discriminator_t;

    ServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ~ServerSocket();

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0);

  private:
    static ISocketFactory *s_zmqFactory;
    ISocket *m_socket;
};

}

#endif
