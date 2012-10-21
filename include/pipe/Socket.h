#ifndef PIPESOCKET_H
#define PIPESOCKET_H

#include <unistd.h>

#include <string>
#include <deque>
#include <atomic>
#include <thread>
#include <vector>

#include "../Socket.h"
#include "../SpinLock.h"
#include "../Exceptions.h"

namespace IPC{
namespace pipe{

enum Mode{
  PIPE_PULL,
  PIPE_PUSH,
  PIPE_CLIENT,
  PIPE_SERVER
};

class MOServerSocket;

class PipeSocketBase : public ISocket{
  friend MOServerSocket;

  public:
    PipeSocketBase(const PipeSocketBase &) = delete;
    virtual ~PipeSocketBase();

    virtual void send(const void *buffer, size_t size, void *hint = 0);
    virtual size_t receive(void **buffer, size_t size = 0);

    PipeSocketBase & operator=(PipeSocketBase &) = delete;

  protected:
    PipeSocketBase(const Channel &channel, Mode type, bool hasOwnership, bool fastTransfer, void (*deallocator)(void *, void *));

  private:
    struct Pipe{
      int read;
      int write;
    };

    Mode m_mode;
    bool m_isOwner;
    bool m_fast;
    Pipe m_transferPipe;
    Pipe m_ctlPipe;
    void *m_receiveBuffer;
    SpinLock m_lock;
    std::thread *m_ctlThread = 0;
    std::atomic<bool> m_destroy = {false};
    std::deque<const void *> m_pendingBuffers;

    Pipe openPipe(const std::string& name);
};

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, bool hasOwnership, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, PIPE_PUSH, hasOwnership, fastTransfer, deallocator){}
    ProducerSocket(const ProducerSocket &) = delete;

    virtual void send(const void *buffer, size_t size, void *hint = 0){
      PipeSocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }

    ProducerSocket & operator=(const ProducerSocket &) = delete;
};

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, bool hasOwnership) : PipeSocketBase(channel, PIPE_PULL, hasOwnership, true, 0){}
    ConsumerSocket(const ConsumerSocket &) = delete;

    virtual void send(const void *buffer, size_t size, void *hint = 0){
      throw InvalidOperationException();
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return PipeSocketBase::receive(buffer, size);
    }

    ConsumerSocket & operator=(const ConsumerSocket &) = delete;
};

class ServiceSocketBase : public ISocket{
  friend MOServerSocket;

  public:
    ServiceSocketBase(const ServiceSocketBase &) = delete;
    virtual ~ServiceSocketBase();

    virtual void send(const void *buffer, size_t size, void *hint = 0);
    virtual size_t receive(void **buffer, size_t size = 0);

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

    virtual void send(const void *buffer, size_t size, void *hint = 0){
      m_private->send(buffer, size, hint);
    }
    virtual size_t receive(void **buffer, size_t size = 0){
      return m_private->receive(buffer, size);
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

    virtual void send(const void *buffer, size_t size, void *hint = 0);
    virtual size_t receive(void **buffer, size_t size = 0);

    MOServerSocket & operator=(const MOServerSocket &) = delete;

  private:
    int m_peerPoll;
    ServerSocket *m_currentPeer = 0;
    std::thread m_listener;
    std::atomic<bool> m_destroy = {false};
    std::vector<ServerSocket *> m_peers;
};

}
}

#endif
