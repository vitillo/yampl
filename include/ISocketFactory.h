#ifndef ISOCKETFACTORY_H
#define ISOCKETFACTORY_H

#include "Channel.h"
#include "ISocket.h"

namespace IPC{

void defaultDeallocator(void *, void *);

class ISocketFactory{
  public:
    virtual ~ISocketFactory(){}

    virtual ISocket *createProducerSocket(Channel channel, bool ownership = true , void (*deallocator)(void *, void *) = defaultDeallocator) = 0;
    virtual ISocket *createConsumerSocket(Channel channel, bool ownership = true) = 0;
    virtual ISocket *createClientSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator) = 0;
    virtual ISocket *createServerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator) = 0;


  private:
    ISocketFactory & operator=(const ISocketFactory &);
};

}

#endif
