#ifndef SOCKETFACTORY_H
#define SOCKETFACTORY_H

#include "ISocketFactory.h"

namespace IPC{

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

    virtual ISocket *createProducerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createConsumerSocket(Channel channel, bool ownership = true);
    virtual ISocket *createClientSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createServerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    ZMQ::SocketFactory *m_zmqFactory = 0;
    pipe::SocketFactory *m_pipeFactory = 0;

    SocketFactory & operator=(const SocketFactory &);
};

}

#endif
