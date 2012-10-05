#ifndef IPC_ZMQSOCKETFACTORY_H
#define IPC_ZMQSOCKETFACTORY_H

#include "SocketFactory.h"

namespace zmq{
  class context_t;
}

namespace IPC{

class ZMQSocketFactory : public ISocketFactory{
  public:
    ZMQSocketFactory();

    virtual ISocket *createProducerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createConsumerSocket(Channel channel, bool ownership = true);
    virtual ISocket *createClientSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createServerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    static zmq::context_t *m_context;
};

}

#endif
