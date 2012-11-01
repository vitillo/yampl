#ifndef IPC_ISOCKETFACTORY_H
#define IPC_ISOCKETFACTORY_H

#include <stdlib.h>

#include "Channel.h"
#include "ISocket.h"

namespace IPC{

inline void defaultDeallocator(void *buffer, void *){
  free(buffer);
}

inline void voidDeallocator(void *, void *){}

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
