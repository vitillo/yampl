#ifndef YAMPL_ISOCKETFACTORY_H
#define YAMPL_ISOCKETFACTORY_H

#include <stdlib.h>

#include "yampl/Channel.h"
#include "yampl/ISocket.h"

namespace yampl{

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

    virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator) = 0;
    virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator) = 0;

  private:
    ISocketFactory & operator=(const ISocketFactory &);
};

}

#endif
