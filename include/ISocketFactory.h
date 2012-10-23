#ifndef ISOCKETFACTORY_H
#define ISOCKETFACTORY_H

#include "Channel.h"
#include "ISocket.h"

namespace IPC{

void defaultDeallocator(void *, void *);

enum Semantics{
  COPY_DATA = 0,
  MOVE_DATA
};

class ISocketFactory{
  public:
    virtual ~ISocketFactory(){}

    virtual ISocket *createProducerSocket(Channel channel, Semantics semantics = COPY_DATA , void (*deallocator)(void *, void *) = defaultDeallocator) = 0;
    virtual ISocket *createConsumerSocket(Channel channel, Semantics semantics = COPY_DATA) = 0;
    virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator) = 0;
    virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator) = 0;


  private:
    ISocketFactory & operator=(const ISocketFactory &);
};

}

#endif
