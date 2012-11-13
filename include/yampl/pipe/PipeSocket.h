#ifndef YAMPL_PIPE_SOCKET_H
#define YAMPL_PIPE_SOCKET_H

#include <unistd.h>

#include <string>
#include <deque>
#include <vector>
#include <tr1/memory>
#include <tr1/functional>

#include "yampl/ISocket.h"
#include "yampl/ISocketFactory.h"
#include "yampl/Exceptions.h"
#include "yampl/utils/SpinLock.h"
#include "yampl/utils/Poller.h"
#include "yampl/utils/Thread.h"

namespace yampl{
namespace pipe{

enum Mode{
  PIPE_PULL,
  PIPE_PUSH,
  PIPE_CLIENT,
  PIPE_SERVER
};

class MOServerSocket;

class RawPipe{
  public:
    int read;
    int write;

    RawPipe(const std::string &name);
    ~RawPipe();


  private:
    RawPipe(const RawPipe &);
    RawPipe & operator=(const RawPipe &);

    const std::string m_name;
    bool m_doUnlink;
};

class PipeSocketBase : public ISocket{
  friend class MOServerSocket;

  public:
    virtual ~PipeSocketBase();

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0);

  protected:
    PipeSocketBase(const Channel &channel, Mode type, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *));

  private:
    PipeSocketBase(const PipeSocketBase &);
    PipeSocketBase & operator=(PipeSocketBase &);

    void ctlThreadFun(void (*deallocator)(void *, void *));
 
    Mode m_mode;
    Semantics m_semantics;
    bool m_fast;
    size_t m_receiveSize;
    void *m_receiveBuffer;
    bool m_destroy;
    SpinLock m_lock;
    std::tr1::shared_ptr<Thread> m_ctlThread;
    std::tr1::shared_ptr<RawPipe> m_transferPipe;
    std::tr1::shared_ptr<RawPipe> m_ctlPipe;
    std::deque<std::pair<void *, void *> > m_pendingBuffers;
};

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, PIPE_PUSH, semantics, fastTransfer, deallocator){}

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0){
      PipeSocketBase::send(buffer, size, discriminator, hint);
    }

    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0){
      throw InvalidOperationException();
    }

  private:
    ProducerSocket(const ProducerSocket &);
    ProducerSocket & operator=(const ProducerSocket &);
};

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, Semantics semantics) : PipeSocketBase(channel, PIPE_PULL, semantics, true, 0){}

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0){
      throw InvalidOperationException();
    }

    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0){
      return PipeSocketBase::recv(buffer, size, discriminator);
    }

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);

};

class ServiceSocketBase : public ISocket{
  friend class MOServerSocket;

  public:
    virtual ~ServiceSocketBase();

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0);

  protected:
    ServiceSocketBase(const Channel &channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *), Mode mode);

  private:
    ServiceSocketBase(const ServiceSocketBase &);
    ServiceSocketBase & operator=(const ServiceSocketBase &);

    ISocket *m_reqSocket;
    ISocket *m_repSocket;
    bool m_receiveCompleted;
    Mode m_mode;
};

class ClientSocket : public ServiceSocketBase{
  public:
    ClientSocket(const Channel &channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, semantics, fastTransfer, deallocator, PIPE_CLIENT){}

  private:
    ClientSocket(const ClientSocket &);
    ClientSocket & operator=(const ClientSocket &);
};

class ServerSocket : public ServiceSocketBase{
  public:
    ServerSocket(const Channel &channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, semantics, fastTransfer, deallocator, PIPE_SERVER){}

  private:
    ServerSocket(const ServerSocket &);
    ServerSocket & operator=(const ServerSocket &);
};

class MOClientSocket: public ISocket{
  public:
    MOClientSocket(const Channel& channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *));
    virtual ~MOClientSocket();

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0){
      m_private->send(buffer, size, discriminator, hint);
    }

    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0){
      return m_private->recv(buffer, size, discriminator);
    }

  private:
    MOClientSocket(const MOClientSocket &);
    MOClientSocket & operator=(const MOClientSocket &);

    pid_t m_pid;
    ISocket *m_server;
    ISocket *m_private;
};

class MOServerSocket: public ISocket{
  public:
    MOServerSocket(const Channel& channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *));
    virtual ~MOServerSocket();

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0);

  private:
    MOServerSocket(const MOServerSocket &);
    MOServerSocket & operator=(const MOServerSocket &);

    void listenerThreadFun(const Channel &channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *));

    Poller m_peerPoll;
    ServerSocket *m_currentPeer;
    bool m_destroy;
    std::tr1::shared_ptr<Thread> m_listener;
    std::vector<std::tr1::shared_ptr<ServerSocket> > m_peers;
};

}
}

#endif
