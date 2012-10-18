#ifndef PIPESOCKET_H
#define PIPESOCKET_H

#include <string>

#include "../Socket.h"

namespace IPC{
namespace pipe{

enum Mode{
  PIPE_PULL,
  PIPE_PUSH,
  PIPE_CLIENT,
  PIPE_SERVER
};

class PipeSocketBase : public ISocket{
  public:
    virtual ~PipeSocketBase();

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

  protected:
    PipeSocketBase(const Channel &channel, Mode type, bool ownership = true, bool fastTransfer = true);

  private:
    std::string m_pipename;
    Mode m_mode;
    bool m_hasOwnership;
    bool m_fast;
    int m_pipe;
    int m_peer;
    void *m_receiveBuffer;
};

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, PIPE_PUSH, ownership, fastTransfer){}

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
  public:
    virtual ~ServiceSocketBase();

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

  protected:
    ServiceSocketBase(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *), Mode mode);

  private:
    bool m_receiveCompleted;
    Mode m_mode;
    ISocket *m_reqSocket;
    ISocket *m_repSocket;
};

class ClientSocket : public ServiceSocketBase{
  public:
    ClientSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, ownership, fastTransfer, deallocator, PIPE_CLIENT){}
};

class ServerSocket : public ServiceSocketBase{
  public:
    ServerSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : ServiceSocketBase(channel, ownership, fastTransfer, deallocator, PIPE_SERVER){}
};


}
}

#endif
