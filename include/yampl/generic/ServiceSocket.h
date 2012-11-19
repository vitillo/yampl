#ifndef YAMPL_SERVICESOCKET
#define YAMPL_SERVICESOCKET

#include "yampl/ISocket.h"

namespace yampl{

class ServiceSocketBase : public ISocket{
  public:
    virtual ~ServiceSocketBase(){
      delete m_producerSocket;
      delete m_consumerSocket;
    }

    virtual void send(SendArgs &args){
      m_producerSocket->send(args);
    }

    virtual ssize_t recv(RecvArgs &args){
      return m_consumerSocket->recv(args);
    }

    ISocket *getProducerSocket(){
      return m_producerSocket;
    }

    ISocket *getConsumerSocket(){
      return m_consumerSocket;
    }

  protected:
    Channel m_reqChannel;
    Channel m_repChannel;
    ISocket *m_producerSocket;
    ISocket *m_consumerSocket;

    ServiceSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_reqChannel(channel.name + "_req", channel.context), m_repChannel(channel.name + "_rep", channel.context), m_producerSocket(0), m_consumerSocket(0){}

  private:
    ServiceSocketBase(const ServiceSocketBase &);
    ServiceSocketBase & operator=(const ServiceSocketBase &);
};

template <typename P, typename C>
class ClientSocket : public ServiceSocketBase{
  public:
    ClientSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, semantics, deallocator){
      m_producerSocket = new P(m_reqChannel, semantics, deallocator);
      m_consumerSocket = new C(m_repChannel, semantics); 
    }
};

template <typename P, typename C>
class ServerSocket : public ServiceSocketBase{
  public:
    ServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, semantics, deallocator){
      m_consumerSocket = new C(m_reqChannel, semantics); 
      m_producerSocket = new P(m_repChannel, semantics, deallocator);
    }
};

}


#endif
