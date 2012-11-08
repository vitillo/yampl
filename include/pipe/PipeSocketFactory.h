#ifndef YAMPL_PIPE_SOCKETFACTORY_H
#define YAMPL_PIPE_SOCKETFACTORY_H

#include "ISocketFactory.h"

namespace YAMPL{
namespace pipe{

class SocketFactory : public ISocketFactory{
  public:
    SocketFactory(bool zerocopy = true) : m_zerocopy(zerocopy){}

    virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    bool m_zerocopy;

    SocketFactory & operator=(const SocketFactory &);
};

}
}

#endif
