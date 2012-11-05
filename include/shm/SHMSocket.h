#ifndef YAMPL_SHM_SOCKET_H
#define YAMPL_SHM_SOCKET_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <tr1/memory>

#include "ISocket.h"
#include "ISocketFactory.h"
#include "Channel.h"
#include "utils/RingBuffer.h"

namespace YAMPL{
namespace shm{

class PipeSocketBase : public ISocket{
  public:
    virtual ~PipeSocketBase();

    virtual void send(void *buffer, size_t size, void *hint = 0);
    virtual size_t recv(void **buffer, size_t size = 0);

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
    ConsumerSocket(const Channel &channel, Semantics semantics) : PipeSocketBase(channel, semantics, 0){
    }

    virtual void send(void *buffer, size_t size, void *hint = 0){
      throw InvalidOperationException();
    }

    virtual size_t recv(void **buffer, size_t size = 0){
      openSocket();
      return PipeSocketBase::recv(buffer, size);
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

}
}

#endif
