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

    virtual ISocket *createProducerSocket(Channel channel, bool ownership = false, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createConsumerSocket(Channel channel, bool ownership = false);
    virtual ISocket *createClientSocket(Channel channel, bool ownership = false, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createServerSocket(Channel channel, bool ownership = false, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    zmq::context_t *m_context;

    SocketFactory & operator=(const SocketFactory &);
};

}
}

#endif
