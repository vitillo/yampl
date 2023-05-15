#ifndef YAMPL_ZEROMQ_SOCKETFACTORY_H
#define YAMPL_ZEROMQ_SOCKETFACTORY_H

#include "yampl/ISocketFactory.h"

namespace zmq{
  class context_t;
}

namespace yampl{
namespace zeromq{

class SocketFactory : public ISocketFactory{
  public:
    SocketFactory();
    virtual ~SocketFactory();

    virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator, const std::string& name = CONST_DEFAULT_ID);
    virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    zmq::context_t *m_context;

    SocketFactory & operator=(const SocketFactory &);
};

}
}

#endif
