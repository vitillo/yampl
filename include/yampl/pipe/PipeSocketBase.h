#ifndef YAMPL_PIPE_PIPESOCKETBASE_H
#define YAMPL_PIPE_PIPESOCKETBASE_H

#include <deque>
#include <tr1/memory>

#include "yampl/ISocket.h"
#include "yampl/Channel.h"
#include "yampl/ISocketFactory.h"
#include "yampl/utils/SpinLock.h"
#include "yampl/utils/Thread.h"
#include "yampl/utils/RawPipe.h"

namespace yampl{
namespace pipe{

enum Mode{
  PIPE_PULL,
  PIPE_PUSH,
  PIPE_CLIENT,
  PIPE_SERVER
};

class PipeSocketBase : public ISocket{
  friend class ServerSocket;

  public:
    virtual ~PipeSocketBase();

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

  protected:
    PipeSocketBase(const Channel &channel, Mode type, Semantics semantics, void (*deallocator)(void *, void *));

  private:
    PipeSocketBase(const PipeSocketBase &);
    PipeSocketBase & operator=(PipeSocketBase &);

    void ctlThreadFun(void (*deallocator)(void *, void *));
 
    Mode m_mode;
    Semantics m_semantics;
    bool m_isRecvPending;
    size_t m_receiveSize;
    void *m_receiveBuffer;
    SpinLock m_lock;
    std::tr1::shared_ptr<Thread> m_ctlThread;
    std::tr1::shared_ptr<RawPipe> m_transferPipe;
    std::tr1::shared_ptr<RawPipe> m_ctlPipe;
    std::deque<std::pair<void *, void *> > m_pendingBuffers;
};

}
}

#endif
