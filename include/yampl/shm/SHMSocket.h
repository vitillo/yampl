#ifndef YAMPL_SHM_SOCKET_H
#define YAMPL_SHM_SOCKET_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <tr1/memory>

#include "yampl/ISocket.h"
#include "yampl/ISocketFactory.h"
#include "yampl/Channel.h"
#include "yampl/utils/RingBuffer.h"
#include "yampl/utils/SharedMemory.h"
#include "yampl/generic/ServiceSocket.h"

namespace yampl{
namespace shm{

class PipeSocketBase : public ISocket{
  public:
    virtual ~PipeSocketBase();

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0);
    virtual ssize_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0);

  protected:
    size_t m_size;
    std::string m_name;
    std::tr1::shared_ptr<SharedMemory> m_memory;
    std::tr1::shared_ptr<RingBuffer> m_queue;

    PipeSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    void openSocket(bool create);

  private:
    PipeSocketBase(const PipeSocketBase &);
    PipeSocketBase & operator=(PipeSocketBase &);

    Semantics m_semantics;
    void (*m_deallocator)(void *, void *);
    size_t m_receiveSize;
    void *m_receiveBuffer;
};

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, semantics, deallocator){
      openSocket(true);
     }

    ~ProducerSocket(){
      shm_unlink(m_name.c_str());
    }

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator, void *hint){
      PipeSocketBase::send(buffer, size, discriminator, hint);
    }

    virtual ssize_t recv(void **buffer, size_t size, discriminator_t *discriminator){
      throw InvalidOperationException();
    }

  private:
    ProducerSocket(const ProducerSocket &);
    ProducerSocket & operator=(const ProducerSocket &);
};

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, Semantics semantics) : PipeSocketBase(channel, semantics, 0){
    }

    virtual void send(void *buffer, size_t size, discriminator_t *discriminator, void *hint){
      throw InvalidOperationException();
    }

    virtual ssize_t recv(void **buffer, size_t size, discriminator_t *discriminator){
      openSocket(false);
      return PipeSocketBase::recv(buffer, size, discriminator);
    }

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);
};

typedef ClientSocket<ProducerSocket, ConsumerSocket> ClientSocket;
typedef ServerSocket<ProducerSocket, ConsumerSocket> ServerSocket;

}
}

#endif
