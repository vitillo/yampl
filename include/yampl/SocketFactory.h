#ifndef YAMPL_SOCKETFACTORY_H
#define YAMPL_SOCKETFACTORY_H

#include "yampl/ISocketFactory.h"

namespace yampl{

namespace ZMQ{
  class SocketFactory;
}

namespace pipe{
  class SocketFactory;
}

class SocketFactory : public ISocketFactory{
  public:
    SocketFactory();
    virtual ~SocketFactory();

    virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator, const std::string& name = CONST_DEFAULT_ID) override;
    virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator) override;

  private:
    ISocketFactory *m_zmqFactory;
    ISocketFactory *m_pipeFactory;
    ISocketFactory *m_shmFactory;

    SocketFactory & operator=(const SocketFactory &);
};

}

#endif
