#ifndef YAMPL_RINGBUFFER_H
#define YAMPL_RINGBUFFER_H

#include <cstring>
#include <iostream>

namespace YAMPL{
/*
 * Lock and wait-free implementation of a multithreaded single-producer, single-consumer ring buffer. The buffer needs to be zero-initialized.
 */
class RingBuffer{
  public:
    RingBuffer(size_t size, void *buffer): m_size(size), m_buffer((char *)buffer), /**m_head(((size_t *)m_buffer)[0]), *m_tail((*(size_t *)&m_buffer[size - sizeof(size_t)])), */m_initialized(false){
      /* Try to use different cachelines for m_head and m_tail to avoid cache ping-pong */
      m_head = &((size_t *)m_buffer)[0];
      m_tail = (size_t *)&m_buffer[size - sizeof(size_t)];
      m_buffer += 2*sizeof(size_t);
      m_size -= 2*sizeof(size_t);
    }

    size_t enqueue(void *buffer, size_t size){
      if((*m_head + 1) % m_size == *m_tail)
	return 0;
      
      size_t empty = emptySize();
      size = empty < size ? empty : size;

      size_t tail = enqueueBuffer(buffer, size, *m_tail);

      // A compiler barrier has to be issued even though stores are ordered on x86
      // to ensure that the stores are not reorderded at compilation time
      asm volatile("" ::: "memory"); 
      *m_tail = tail;

      return size;
    }

    size_t dequeue(void *buffer, size_t size){
      if(*m_head == *m_tail)
	return 0;

      size_t head = dequeueBuffer(buffer, size, *m_head);
      
      asm volatile("" ::: "memory"); 
      *m_head = head;
      
      return size;
    }


  private:
    size_t m_size;
    char *m_buffer;
    size_t *m_head;
    size_t *m_tail;
    bool m_initialized;

    size_t emptySize(){
      if(*m_tail >= *m_head)
	return m_size - *m_tail + *m_head - 1; 
      else
	return *m_head - *m_tail - 1;
    }

    size_t enqueueBuffer(void *buffer, size_t size, size_t tail){
      if(tail + size > m_size){
	size_t p1 = m_size - tail;
	size_t p2 = size - p1;

	memcpy(m_buffer + tail, buffer, p1);
	memcpy(m_buffer, (char *)buffer + p1, p2);

	return p2;
      }
      else{
	memcpy(m_buffer + tail, buffer, size);
	return tail + size;
      }
    }

    size_t dequeueBuffer(void *buffer, size_t size, size_t head){
      if(head + size > m_size){
	size_t p1 = m_size - head;
	size_t p2 = size - p1;

	memcpy(buffer, m_buffer + head, p1);
	memcpy((char *)buffer + p1, m_buffer, p2);

	return p2;
      }else{
	memcpy(buffer, m_buffer + head, size);

	return head + size;
      }
    }
};

}

#endif
