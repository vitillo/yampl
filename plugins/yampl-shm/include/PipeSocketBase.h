#ifndef YAMPL_SHM_PIPESOCKETBASE_H
#define YAMPL_SHM_PIPESOCKETBASE_H

#include "yampl/ISocket.h"
#include "yampl/Channel.h"
#include "yampl/ISocketFactory.h"
#include "yampl/utils/SharedMemory.h"
#include "yampl/utils/RingBuffer.h"

namespace yampl{
namespace shm{

  class PipeSocketBase : public ISocket{
  public:
    virtual ~PipeSocketBase();

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

  protected:
    size_t m_size;
    std::string m_name;
    std::tr1::shared_ptr<SharedMemory> m_memory;
    std::tr1::shared_ptr<RingBuffer> m_queue;

    PipeSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    void openSocket(bool isProducer);

  private:
    PipeSocketBase(const PipeSocketBase &);
    PipeSocketBase & operator=(PipeSocketBase &);

    Semantics m_semantics;
    void (*m_deallocator)(void *, void *);
    bool m_isRecvPending;
    size_t m_receiveSize;
    void *m_receiveBuffer;
};

}
}

#endif
