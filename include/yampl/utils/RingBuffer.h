#ifndef YAMPL_RINGBUFFER_H
#define YAMPL_RINGBUFFER_H

#include <sys/types.h>

#include <cstring>
#include <memory>

#include "yampl/utils/Semaphore.h"

namespace yampl{
/*
 * Lock-free implementation of a multithreaded single-producer, single-consumer ring buffer. The buffer needs to be zero-initialized.
 */
class RingBuffer{
  public:
    RingBuffer(size_t size, void *buffer, bool isProducer): m_size(size), m_buffer((char *)buffer), m_head(0), m_tail(0), m_initialized(false){
      // Use different cachelines for the semaphores to avoid pingponging
      size_t n = m_size / sizeof(int);
      size_t padding = m_size - n * sizeof(int);

      m_fillCount.reset(new Semaphore(&((int *)m_buffer)[0]));
      m_emptyCount.reset(new Semaphore(&((int *)m_buffer)[n - 1]));
      m_buffer += sizeof(int);
      m_size = m_size - 2*sizeof(int) - padding;

      if(isProducer){
	m_emptyCount->up(m_size);
      }
    }

    size_t enqueue(void *buffer, size_t size, bool cont = false){
      m_emptyCount->down(size);
      enqueueBuffer(buffer, size);
      m_fillCount->up(size);
      return size;
    }

    size_t dequeue(void *buffer, size_t size){
      m_fillCount->down(size);
      dequeueBuffer(buffer, size);
      m_emptyCount->up(size);
      return size;
    }

    bool empty(){
      return m_fillCount->value() == 0;
    }

    size_t size(){
      return m_size;
    }

  private:
    size_t m_size;
    char *m_buffer;
    size_t m_head;
    size_t m_tail;
    bool m_initialized;
    std::tr1::shared_ptr<Semaphore> m_fillCount;
    std::tr1::shared_ptr<Semaphore> m_emptyCount;

    void enqueueBuffer(void *buffer, size_t size){
      if(m_tail + size > m_size){
	size_t p1 = m_size - m_tail;
	size_t p2 = size - p1;

	memcpy(m_buffer + m_tail, buffer, p1);
	memcpy(m_buffer, (char *)buffer + p1, p2);

	m_tail = p2;
      }
      else{
	memcpy(m_buffer + m_tail, buffer, size);

	m_tail = m_tail + size;
      }
    }

    void dequeueBuffer(void *buffer, size_t size){
      if(m_head + size > m_size){
	size_t p1 = m_size - m_head;
	size_t p2 = size - p1;

	memcpy(buffer, m_buffer + m_head, p1);
	memcpy((char *)buffer + p1, m_buffer, p2);

	m_head = p2;
      }else{
	memcpy(buffer, m_buffer + m_head, size);

	m_head = m_head + size;
      }
    }
};

}

#endif
