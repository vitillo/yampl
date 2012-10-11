#ifndef SOCKETFACTORY_H
#define SOCKETFACTORY_H

#include "Socket.h"

namespace IPC{

class ISocketFactory{
  public:
    virtual ~ISocketFactory(){}

    virtual ISocket *createProducerSocket(Channel channel, bool ownership = true , void (*deallocator)(void *, void *) = defaultDeallocator) = 0;
    virtual ISocket *createConsumerSocket(Channel channel, bool ownership = true) = 0;
    virtual ISocket *createClientSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator) = 0;
    virtual ISocket *createServerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator) = 0;
};

}

#endif
