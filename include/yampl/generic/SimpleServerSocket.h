#ifndef YAMPL_SIMPLESERVERSOCKET_H
#define YAMPL_SIMPLESERVERSOCKET_H

#include "yampl/generic/ServiceSocketBase.h"

namespace yampl{

template <typename P, typename C>
class SimpleServerSocket : public ServiceSocketBase<P, C>{
  public:
    SimpleServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

  protected:
    using ServiceSocketBase<P, C>::m_producerSocket;
    using ServiceSocketBase<P, C>::m_consumerSocket;
    using ServiceSocketBase<P, C>::m_reqChannel;
    using ServiceSocketBase<P, C>::m_repChannel;
};

template <typename P, typename C>
SimpleServerSocket<P, C>::SimpleServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : ServiceSocketBase<P, C>(channel, semantics, deallocator){
  m_consumerSocket = new C(m_reqChannel, semantics); 
  m_producerSocket = new P(m_repChannel, semantics, deallocator);
}

}

#endif
