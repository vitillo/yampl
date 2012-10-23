#ifndef PIPESOCKET_H
#define PIPESOCKET_H

#include <unistd.h>

#include <string>
#include <deque>
#include <atomic>
#include <thread>
#include <vector>
#include <memory>

#include "ISocket.h"
#include "Exceptions.h"
#include "utils/SpinLock.h"
#include "utils/Poller.h"

namespace IPC{
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
    RawPipe(const RawPipe &) = delete;
    ~RawPipe();

    RawPipe & operator=(const RawPipe &) = delete;

  private:
    const std::string m_name;
    bool m_doUnlink = false;
};

class PipeSocketBase : public ISocket{
  friend MOServerSocket;

  public:
    PipeSocketBase(const PipeSocketBase &) = delete;
    virtual ~PipeSocketBase();

    virtual void send(void *buffer, size_t size, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size = 0);

    PipeSocketBase & operator=(PipeSocketBase &) = delete;

  protected:
    PipeSocketBase(const Channel &channel, Mode type, bool hasOwnership, bool fastTransfer, void (*deallocator)(void *, void *));

  private:
    Mode m_mode;
    bool m_hasOwnership;
    bool m_fast;
    size_t m_receiveSize = 0;
    void *m_receiveBuffer = NULL;
    SpinLock m_lock;
    std::thread m_ctlThread;
    std::atomic<bool> m_destroy = {false};
    std::shared_ptr<RawPipe> m_transferPipe;
    std::shared_ptr<RawPipe> m_ctlPipe;
    std::deque<std::pair<const void *, const void *>> m_pendingBuffers;
};

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, bool hasOwnership, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, PIPE_PUSH, hasOwnership, fastTransfer, deallocator){}
    ProducerSocket(const ProducerSocket &) = delete;

    virtual void send(void *buffer, size_t size, void *hint = 0){
      PipeSocketBase::send(buffer, size, hint);
    }

    virtual size_t recv(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }

    ProducerSocket & operator=(const ProducerSocket &) = delete;
};

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, bool hasOwnership) : PipeSocketBase(channel, PIPE_PULL, hasOwnership, true, 0){}
    ConsumerSocket(const ConsumerSocket &) = delete;

    virtual void send(void *buffer, size_t size, void *hint = 0){
      throw InvalidOperationException();
    }

    virtual size_t recv(void **buffer, size_t size = 0){
      return PipeSocketBase::recv(buffer, size);
    }

    ConsumerSocket & operator=(const ConsumerSocket &) = delete;
};

class ServiceSocketBase : public ISocket{
  friend MOServerSocket;

  public:
    ServiceSocketBase(const ServiceSocketBase &) = delete;
    virtual ~ServiceSocketBase();

    virtual void send(void *buffer, size_t size, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size = 0);

    ServiceSocketBase & operator=(const ServiceSocketBase &) = delete;

  protected:
    ServiceSocketBase(const Channel &channel, bool hasOwnership, bool fastTransfer, void (*deallocator)(void *, void *), Mode mode);

  private:
    ISocket *m_reqSocket = 0;
    ISocket *m_repSocket = 0;
    bool m_receiveCompleted;
    Mode m_mode;
};

class ClientSocket : public ServiceSocketBase{
  public:
    ClientSocket(const Channel &channel, bool hasOwnership, bool fastTransfer, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, hasOwnership, fastTransfer, deallocator, PIPE_CLIENT){}
    ClientSocket(const ClientSocket &) = delete;

    ClientSocket & operator=(const ClientSocket &) = delete;
};

class ServerSocket : public ServiceSocketBase{
  public:
    ServerSocket(const Channel &channel, bool hasOwnership, bool fastTransfer, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, hasOwnership, fastTransfer, deallocator, PIPE_SERVER){}
    ServerSocket(const ServerSocket &) = delete;

    ServerSocket & operator=(const ServerSocket &) = delete;
};

class MOClientSocket: public ISocket{
  public:
    MOClientSocket(const Channel& channel, bool hasOwnership, bool fastTransfer, void (*deallocator)(void *, void *));
    MOClientSocket(const MOClientSocket &) = delete;
    virtual ~MOClientSocket();

    virtual void send(void *buffer, size_t size, void *hint = 0){
      m_private->send(buffer, size, hint);
    }
    virtual size_t recv(void **buffer, size_t size = 0){
      return m_private->recv(buffer, size);
    }

    MOClientSocket & operator=(const MOClientSocket &) = delete;

  private:
    pid_t m_pid = getpid();
    ISocket *m_server = 0;
    ISocket *m_private = 0;
};

class MOServerSocket: public ISocket{
  public:
    MOServerSocket(const Channel& channel, bool hasOwnership, bool fastTransfer, void (*deallocator)(void *, void *));
    MOServerSocket(const MOServerSocket &) = delete;
    virtual ~MOServerSocket();

    virtual void send(void *buffer, size_t size, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size = 0);

    MOServerSocket & operator=(const MOServerSocket &) = delete;

  private:
    Poller m_peerPoll;
    ServerSocket *m_currentPeer = 0;
    std::thread m_listener;
    std::atomic<bool> m_destroy = {false};
    std::vector<std::shared_ptr<ServerSocket>> m_peers;
};

}
}

#endif
