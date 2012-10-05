#ifndef IPC_PIPESOCKET_H
#define IPC_PIPESOCKET_H

#include <string>

#include "Socket.h"

namespace IPC{

enum Mode{
  PIPE_READ,
  PIPE_WRITE
};

class PipeBaseSocket : public ISocket{
  public:
    virtual ~PipeBaseSocket();

    virtual void send(const void *buffer, size_t size, void *hint = NULL);
    virtual size_t receive(void **buffer, size_t size = 0);

  protected:
    PipeBaseSocket(const Channel &channel, Mode type, bool ownership = true, bool fastTransfer = true);

  private:
    std::string m_pipename;
    Mode m_mode;
    bool m_hasOwnership;
    bool m_fast;
    int m_pipe;
};

class PipeProducerSocket : public PipeBaseSocket{
  public:
    PipeProducerSocket(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeBaseSocket(channel, PIPE_WRITE, ownership, fastTransfer){}


    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      PipeBaseSocket::send(buffer, size, hint);
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }
};

class PipeConsumerSocket : public PipeBaseSocket{
  public:
    PipeConsumerSocket(const Channel &channel, bool ownership) : PipeBaseSocket(channel, PIPE_READ, ownership){}


    virtual void send(const void *buffer, size_t size, void *hint = NULL){
      throw InvalidOperationException();
    }

    virtual size_t receive(void **buffer, size_t size = 0){
      return PipeBaseSocket::receive(buffer, size);
    }
};


}

#endif
