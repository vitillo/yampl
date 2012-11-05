#ifndef YAMPL_SOCKETFACTORY_H
#define YAMPL_SOCKETFACTORY_H

#include "ISocketFactory.h"

namespace YAMPL{

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

    virtual ISocket *createProducerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createConsumerSocket(Channel channel, Semantics semantics = COPY_DATA);
    virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    ZMQ::SocketFactory *m_zmqFactory ;
    pipe::SocketFactory *m_pipeFactory;

    SocketFactory & operator=(const SocketFactory &);
};

}

#endif
