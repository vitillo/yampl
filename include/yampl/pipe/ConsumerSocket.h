#ifndef YAMPL_PIPE_CONSUMERSOCKET_H
#define YAMPL_PIPE_CONSUMERSOCKET_H

#include "yampl/pipe/PipeSocketBase.h"

namespace yampl{
namespace pipe{

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, Semantics semantics);

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);

};

inline ConsumerSocket::ConsumerSocket(const Channel &channel, Semantics semantics) : PipeSocketBase(channel, PIPE_PULL, semantics, 0){
}

inline void ConsumerSocket::send(SendArgs &args){
  throw InvalidOperationException();
}

inline ssize_t ConsumerSocket::recv(RecvArgs &args){
  return PipeSocketBase::recv(args);
}

}
}

#endif
