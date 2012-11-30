#ifndef YAMPL_SHM_CONSUMERSOCKET_H
#define YAMPL_SHM_CONSUMERSOCKET_H

#include "yampl/shm/PipeSocketBase.h"

namespace yampl{
namespace shm{

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, Semantics semantics) : PipeSocketBase(channel, semantics, 0){
    }

    virtual bool hasPendingMessages(){
      openSocket(false);
      return !m_queue->empty();
    }

    virtual void send(SendArgs &args){
      throw InvalidOperationException();
    }

    virtual ssize_t recv(RecvArgs &args){
      openSocket(false);
      return PipeSocketBase::recv(args);
    }

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);
};

}
}

#endif
