#ifndef IPC_SHM_SOCKET_H
#define IPC_SHM_SOCKET_H

#include <string>
#include <tr1/memory>

#include "ISocket.h"
#include "ISocketFactory.h"
#include "Channel.h"
#include "utils/RingBuffer.h"

namespace IPC{
namespace shm{

class PipeSocketBase : public ISocket{
  public:
    virtual ~PipeSocketBase();

    virtual void send(void *buffer, size_t size, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size = 0);

  protected:
    PipeSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

  private:
    PipeSocketBase(const PipeSocketBase &);
    PipeSocketBase & operator=(PipeSocketBase &);

    Semantics m_semantics;
    void (*m_deallocator)(void *, void *);
    size_t m_size;
    bool m_doUnlink;
    int m_fd;
    std::string m_name;
    std::tr1::shared_ptr<RingBuffer> m_queue;
    size_t m_receiveSize;
    void *m_receiveBuffer;

    void *m_buffer;
};

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, semantics, deallocator){}

    virtual void send(void *buffer, size_t size, void *hint = 0){
      PipeSocketBase::send(buffer, size, hint);
    }

    virtual size_t recv(void **buffer, size_t size = 0){
      throw InvalidOperationException();
    }

  private:
    ProducerSocket(const ProducerSocket &);
    ProducerSocket & operator=(const ProducerSocket &);
};

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, Semantics semantics) : PipeSocketBase(channel, semantics, 0){}

    virtual void send(void *buffer, size_t size, void *hint = 0){
      throw InvalidOperationException();
    }

    virtual size_t recv(void **buffer, size_t size = 0){
      return PipeSocketBase::recv(buffer, size);
    }

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);

};

}
}

#endif
