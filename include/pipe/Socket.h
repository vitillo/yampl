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
    virtual ~PipeSocketBase();
    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

  protected:
    PipeSocketBase(const Channel &channel, Mode type, bool ownership = true, bool fastTransfer = true, void (*deallocator)(void *, void *) = defaultDeallocator);

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
    std::atomic<bool> m_destroy = {false};
    std::thread *m_ctlThread = 0;
    SpinLock m_lock;
    void *m_receiveBuffer;
    std::deque<const void *> m_pendingBuffers;

    Pipe openPipe(const std::string& name);
};

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, PIPE_PUSH, ownership, fastTransfer, deallocator){}

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      PipeSocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }
};

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, bool ownership) : PipeSocketBase(channel, PIPE_PULL, ownership){}

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      throw InvalidOperationException();
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return PipeSocketBase::receive(buffer, size);
    }
};

class ServiceSocketBase : public ISocket{
  friend MOServerSocket;

  public:
    virtual ~ServiceSocketBase();
    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

  protected:
    ServiceSocketBase(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *), Mode mode);

  private:
    ISocket *m_reqSocket;
    ISocket *m_repSocket;
    bool m_receiveCompleted;
    Mode m_mode;
};

class ClientSocket : public ServiceSocketBase{
  public:
    ClientSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, ownership, fastTransfer, deallocator, PIPE_CLIENT){}
};

class ServerSocket : public ServiceSocketBase{
  public:
    ServerSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, ownership, fastTransfer, deallocator, PIPE_SERVER){}
};

class MOClientSocket: public ISocket{
  public:
    MOClientSocket(const Channel& channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *));
    virtual ~MOClientSocket();

    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      m_private->send(buffer, size, hint);
    }
    virtual size_t receive(void **buffer, size_t size = 0){
      return m_private->receive(buffer, size);
    }

  private:
    pid_t m_pid = getpid();
    ISocket *m_server = 0;
    ISocket *m_private = 0;
};

class MOServerSocket: public ISocket{
  public:
    MOServerSocket(const Channel& channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *));
    virtual ~MOServerSocket();

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

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
