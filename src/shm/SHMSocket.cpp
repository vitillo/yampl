#include <iostream>

#include "yampl/Exceptions.h"
#include "yampl/shm/SHMSocket.h"

using namespace std;

namespace yampl{
namespace shm{

PipeSocketBase::PipeSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_size(1024), m_fd(-1), m_name("/" + channel.name), m_semantics(semantics), m_deallocator(deallocator), m_receiveSize(0), m_receiveBuffer(NULL){
}

PipeSocketBase::~PipeSocketBase(){
  munmap((char *)m_buffer - sizeof(bool), m_size + sizeof(bool));
  close(m_fd);

  free(m_receiveBuffer);
}

void PipeSocketBase::send(void *buffer, size_t size, discriminator_t *discriminator, void *hint){
  size_t bytesWritten = 0;

  while(bytesWritten != sizeof(size)){
    bytesWritten += m_queue->enqueue(&size, sizeof(size));
  }

  for(bytesWritten = 0; bytesWritten != size;){
    bytesWritten += m_queue->enqueue((char *)buffer + bytesWritten, size - bytesWritten);
  }

  if(m_semantics == MOVE_DATA)
    m_deallocator(buffer, hint);
}

ssize_t PipeSocketBase::recv(void **buffer, size_t size, discriminator_t *discriminator){
  size_t bytesRead = 0;

  while(bytesRead != sizeof(m_receiveSize)){
    bytesRead += m_queue->dequeue(&m_receiveSize, sizeof(m_receiveSize));
  }

  if(m_semantics == MOVE_DATA){
    if((m_receiveBuffer = realloc(m_receiveBuffer, m_receiveSize)) == 0)
      throw ErrnoException("Receive failed");

    *buffer = m_receiveBuffer;
  }else{
    if(*buffer && size < m_receiveSize)
      throw InvalidSizeException();
    else if(!*buffer)
      *buffer = malloc(m_receiveSize);
  }

  for(bytesRead = 0; bytesRead != m_receiveSize;){
    bytesRead += m_queue->dequeue((char *)*buffer + bytesRead, m_receiveSize - bytesRead);
  }

  m_receiveSize = 0;
  return bytesRead;
}

}
}

