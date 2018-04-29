#ifndef YAMPL_SOCKETFACTORY_H
#define YAMPL_SOCKETFACTORY_H

#include "yampl/ISocketFactory.h"
#include "yampl/plugin/IPlugin.hpp"

#include <vector>
#include <scy/sharedlibrary.h>

namespace yampl{

namespace ZMQ{
  class SocketFactory;
}

namespace pipe{
  class SocketFactory;
}

class SocketFactory : public ISocketFactory{
  public:
    SocketFactory();
    virtual ~SocketFactory();

    virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator, const std::string& name = DEFAULT_ID);
    virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    std::vector<scy::SharedLibrary> factories;
    ISocketFactory *m_zmqFactory;
    ISocketFactory *m_pipeFactory;
    ISocketFactory *m_shmFactory;

    SocketFactory & operator=(const SocketFactory &);
};

}

#endif
