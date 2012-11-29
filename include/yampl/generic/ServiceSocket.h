#ifndef YAMPL_SERVICESOCKET
#define YAMPL_SERVICESOCKET

#include "yampl/ISocket.h"

namespace yampl{

template <typename P, typename C>
class ServiceSocket : public ISocket{
  public:
    virtual ~ServiceSocket();

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

    P *getProducerSocket();
    C *getConsumerSocket();

  protected:
    ServiceSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    Channel m_reqChannel;
    Channel m_repChannel;
    P *m_producerSocket;
    C *m_consumerSocket;

  private:
    ServiceSocket(const ServiceSocket &);
    ServiceSocket & operator=(const ServiceSocket &);
};

template <typename P, typename C>
ServiceSocket<P, C>::ServiceSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_reqChannel(channel.name + "_req", channel.context), m_repChannel(channel.name + "_rep", channel.context), m_producerSocket(0), m_consumerSocket(0){}


template <typename P, typename C>
inline ServiceSocket<P, C>::~ServiceSocket(){
  delete m_producerSocket;
  delete m_consumerSocket;
}

template <typename P, typename C>
inline void ServiceSocket<P, C>::send(SendArgs &args){
  m_producerSocket->send(args);
}

template <typename P, typename C>
inline ssize_t ServiceSocket<P, C>::recv(RecvArgs &args){
  return m_consumerSocket->recv(args);
}

template <typename P, typename C>
inline P * ServiceSocket<P, C>::getProducerSocket(){
  return m_producerSocket;
}

template <typename P, typename C>
inline C * ServiceSocket<P, C>::getConsumerSocket(){
  return m_consumerSocket;
}





}


#endif
