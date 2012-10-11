#ifndef PIPESOCKET_H
#define PIPESOCKET_H

#include <string>

#include "Socket.h"

namespace IPC{

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

class PipeProducerSocket : public PipeSocketBase{
  public:
    PipeProducerSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, PIPE_PUSH, ownership, fastTransfer){}


    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      PipeSocketBase::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }
};

class PipeConsumerSocket : public PipeSocketBase{
  public:
    PipeConsumerSocket(const Channel &channel, bool ownership) : PipeSocketBase(channel, PIPE_PULL, ownership){}


    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      throw InvalidOperationException();
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return PipeSocketBase::receive(buffer, size);
    }
};

class PipeServiceSocketBase : public ISocket{
  public:
    virtual ~PipeServiceSocketBase();

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

  protected:
    PipeServiceSocketBase(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *), Mode mode);

  private:
    bool m_receiveCompleted;
    Mode m_mode;
    ISocket *m_reqSocket;
    ISocket *m_repSocket;
};

class PipeClientSocket : public PipeServiceSocketBase{
  public:
    PipeClientSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeServiceSocketBase(channel, ownership, fastTransfer, deallocator, PIPE_CLIENT){}
};

class PipeServerSocket : public PipeServiceSocketBase{
  public:
    PipeServerSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeServiceSocketBase(channel, ownership, fastTransfer, deallocator, PIPE_SERVER){}
};

}

#endif
