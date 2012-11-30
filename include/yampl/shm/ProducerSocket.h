#ifndef YAMPL_SHM_PRODUCERSOCKET_H
#define YAMPL_SHM_PRODUCERSOCKET_H

#include "yampl/shm/PipeSocketBase.h"

namespace yampl{
namespace shm{

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

  private:
    ProducerSocket(const ProducerSocket &);
    ProducerSocket & operator=(const ProducerSocket &);
};

inline ProducerSocket::ProducerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, semantics, deallocator){
  openSocket(true);
}

inline void ProducerSocket::send(SendArgs &args){
  PipeSocketBase::send(args);
}

inline ssize_t ProducerSocket::recv(RecvArgs &args){
  throw InvalidOperationException();
}

}
}

#endif
