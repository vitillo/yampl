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
    int m_fd;
    std::string m_name;
    void *m_buffer;
    std::tr1::shared_ptr<RingBuffer> m_queue;

    PipeSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

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
      if((m_fd = shm_open(m_name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRWXU)) == -1)
	throw ErrnoException("Shared memory object already exists");

      if(ftruncate(m_fd, m_size) == -1)
	throw ErrnoException("Failed to set the size of the shared memory object");

      if((m_buffer = mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0)) == NULL)
	throw ErrnoException("Failed to mmap shared object");

      m_queue.reset(new RingBuffer(m_size, m_buffer));
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
      openSocket();
      return PipeSocketBase::recv(buffer, size, discriminator);
    }

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);

    void openSocket(){
      if(m_fd != -1)
	return;

      while((m_fd = shm_open(m_name.c_str(), O_RDWR, S_IRWXU)) == -1 && errno == ENOENT);
      if(m_fd == -1)
	throw ErrnoException("Failed to open shared memory object");

      if(ftruncate(m_fd, m_size) == -1)
	throw ErrnoException("Failed to set the size of the shared memory object");

      if((m_buffer = mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0)) == NULL)
	throw ErrnoException("Failed to mmap shared object");

      m_queue.reset(new RingBuffer(m_size, m_buffer));
    }
};

typedef ClientSocket<ProducerSocket, ConsumerSocket> ClientSocket;
typedef ServerSocket<ProducerSocket, ConsumerSocket> ServerSocket;

}
}

#endif
