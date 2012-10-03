#ifndef IPC_SOCKETFACTORY_H
#define IPC_SOCKETFACTORY_H

#include "Socket.h"

namespace IPC{

class ISocketFactory{
  public:
    virtual ISocket *createProducerSocket(Channel channel, int type, bool ownership , void (*deallocator)(void *, void *)) = 0;
    virtual ISocket *createConsumerSocket(Channel channel, int type, bool ownership) = 0;
    virtual ISocket *createClientSocket(Channel channel, int type, bool ownership, void (*deallocator)(void *, void *)) = 0;
    virtual ISocket *createServerSocket(Channel channel, int type, bool ownership, void (*deallocator)(void *, void *)) = 0;
};

}

#endif
