#ifndef YAMPL_PIPE_PRODUCERSOCKET_H
#define YAMPL_PIPE_PRODUCERSOCKET_H

#include "yampl/pipe/PipeSocketBase.h"

namespace yampl{
namespace pipe{

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

  private:
    ProducerSocket(const ProducerSocket &);
    ProducerSocket & operator=(const ProducerSocket &);
};

inline ProducerSocket::ProducerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, PIPE_PUSH, semantics, deallocator){
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
