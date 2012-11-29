#ifndef YAMPL_SIMPLECLIENTSOCKET_H
#define YAMPL_SIMPLECLIENTSOCKET_H

#include "yampl/generic/ServiceSocket.h"

namespace yampl{

template <typename P, typename C>
class SimpleClientSocket : public ServiceSocket<P, C>{
  public:
    SimpleClientSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name);

  protected:
    using ServiceSocket<P, C>::m_producerSocket;
    using ServiceSocket<P, C>::m_consumerSocket;
    using ServiceSocket<P, C>::m_reqChannel;
    using ServiceSocket<P, C>::m_repChannel;
};

template <typename P, typename C>
SimpleClientSocket<P, C>::SimpleClientSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : ServiceSocket<P, C>(channel, semantics, deallocator){
  m_producerSocket = new P(m_reqChannel, semantics, deallocator);
  m_consumerSocket = new C(m_repChannel, semantics); 
}




}

#endif
