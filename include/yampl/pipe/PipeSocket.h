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
#include "yampl/utils/RawPipe.h"
#include "yampl/generic/ServiceSocket.h"

namespace yampl{
namespace pipe{

enum Mode{
  PIPE_PULL,
  PIPE_PUSH,
  PIPE_CLIENT,
  PIPE_SERVER
};

class MOServerSocket;

class PipeSocketBase : public ISocket{
  friend class MOServerSocket;

  public:
    virtual ~PipeSocketBase();

    virtual void send(void *buffer, size_t size, const std::string &peerID = NO_ID, void *hint = 0);
    virtual ssize_t recv(void *&buffer, size_t size, const std::string *&peerID = NO_ID_PTR);

  protected:
    PipeSocketBase(const Channel &channel, Mode type, Semantics semantics, void (*deallocator)(void *, void *));

  private:
    PipeSocketBase(const PipeSocketBase &);
    PipeSocketBase & operator=(PipeSocketBase &);

    void ctlThreadFun(void (*deallocator)(void *, void *));
 
    Mode m_mode;
    Semantics m_semantics;
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
    ProducerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, PIPE_PUSH, semantics, deallocator){}

    virtual void send(void *buffer, size_t size, const std::string &peerID = NO_ID, void *hint = 0){
      PipeSocketBase::send(buffer, size, peerID, hint);
    }

    virtual ssize_t recv(void *&buffer, size_t size, const std::string *&peerID = NO_ID_PTR){
      throw InvalidOperationException();
    }

  private:
    ProducerSocket(const ProducerSocket &);
    ProducerSocket & operator=(const ProducerSocket &);
};

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, Semantics semantics) : PipeSocketBase(channel, PIPE_PULL, semantics, 0){}

    virtual void send(void *buffer, size_t size, const std::string &peerID = NO_ID, void *hint = 0){
      throw InvalidOperationException();
    }

    virtual ssize_t recv(void *&buffer, size_t size, const std::string *&peerID = NO_ID_PTR){
      return PipeSocketBase::recv(buffer, size, peerID);
    }

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);

};

typedef ClientSocket<ProducerSocket, ConsumerSocket> ClientSocket;
typedef ServerSocket<ProducerSocket, ConsumerSocket> ServerSocket;

class MOClientSocket: public ISocket{
  public:
    MOClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *));
    virtual ~MOClientSocket();

    virtual void send(void *buffer, size_t size, const std::string &peerID = NO_ID, void *hint = 0){
      m_private->send(buffer, size, peerID, hint);
    }

    virtual ssize_t recv(void *&buffer, size_t size, const std::string *&peerID = NO_ID_PTR){
      return m_private->recv(buffer, size, peerID);
    }

  private:
    MOClientSocket(const MOClientSocket &);
    MOClientSocket & operator=(const MOClientSocket &);

    pid_t m_pid;
    RawPipe m_announce;
    ISocket *m_private;
};

class MOServerSocket: public ISocket{
  public:
    MOServerSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *));
    virtual ~MOServerSocket();

    virtual void send(void *buffer, size_t size, const std::string &peerID = NO_ID, void *hint = 0);
    virtual ssize_t recv(void *&buffer, size_t size, const std::string *&peerID = NO_ID_PTR);
    virtual ssize_t try_recv(void *&buffer, size_t size, const std::string *&peerID = NO_ID_PTR, long timeout = 0);

  private:
    MOServerSocket(const MOServerSocket &);
    MOServerSocket & operator=(const MOServerSocket &);

    void listenerThreadFun(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    Poller m_peerPoll;
    ServerSocket *m_currentPeer;
    bool m_destroy;
    std::tr1::shared_ptr<Thread> m_listener;
    std::vector<std::tr1::shared_ptr<ServerSocket> > m_peers;
};

}
}

#endif
