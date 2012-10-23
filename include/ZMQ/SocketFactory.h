#ifndef ZMQSOCKETFACTORY_H
#define ZMQSOCKETFACTORY_H

#include "ISocketFactory.h"

namespace zmq{
  class context_t;
}

namespace IPC{
namespace ZMQ{

class SocketFactory : public ISocketFactory{
  public:
    SocketFactory();
    virtual ~SocketFactory();

    virtual ISocket *createProducerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createConsumerSocket(Channel channel, Semantics semantics = COPY_DATA);
    virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    zmq::context_t *m_context;

    SocketFactory & operator=(const SocketFactory &);
};

}
}

#endif
