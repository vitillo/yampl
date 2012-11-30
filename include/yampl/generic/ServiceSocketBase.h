#ifndef YAMPL_SERVICESOCKET
#define YAMPL_SERVICESOCKET

#include "yampl/ISocket.h"

namespace yampl{

template <typename P, typename C>
class ServiceSocketBase : public ISocket{
  public:
    virtual ~ServiceSocketBase();

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

    P *getProducerSocket();
    C *getConsumerSocket();

  protected:
    ServiceSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    Channel m_reqChannel;
    Channel m_repChannel;
    P *m_producerSocket;
    C *m_consumerSocket;

  private:
    ServiceSocketBase(const ServiceSocketBase &);
    ServiceSocketBase & operator=(const ServiceSocketBase &);
};

template <typename P, typename C>
ServiceSocketBase<P, C>::ServiceSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_reqChannel(channel.name + "_req", channel.context), m_repChannel(channel.name + "_rep", channel.context), m_producerSocket(0), m_consumerSocket(0){}


template <typename P, typename C>
inline ServiceSocketBase<P, C>::~ServiceSocketBase(){
  delete m_producerSocket;
  delete m_consumerSocket;
}

template <typename P, typename C>
inline void ServiceSocketBase<P, C>::send(SendArgs &args){
  m_producerSocket->send(args);
}

template <typename P, typename C>
inline ssize_t ServiceSocketBase<P, C>::recv(RecvArgs &args){
  return m_consumerSocket->recv(args);
}

template <typename P, typename C>
inline P * ServiceSocketBase<P, C>::getProducerSocket(){
  return m_producerSocket;
}

template <typename P, typename C>
inline C * ServiceSocketBase<P, C>::getConsumerSocket(){
  return m_consumerSocket;
}





}


#endif
