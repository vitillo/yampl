#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Exceptions.h"
#include "shm/SHMSocket.h"

using namespace std;

namespace YAMPL{
namespace shm{

PipeSocketBase::PipeSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_semantics(semantics), m_deallocator(deallocator), m_size(1024), m_doUnlink(false), m_fd(-1), m_name("/" + channel.name), m_receiveSize(0), m_receiveBuffer(NULL){
  if((m_fd = shm_open(m_name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRWXU)) == -1){
    m_doUnlink = true;
  }else if((m_fd = shm_open(m_name.c_str(), O_RDWR | O_CREAT, S_IRWXU)) == -1){
    throw ErrnoException("Failed to open shared memory object");
  }

  if(ftruncate(m_fd, m_size) == -1)
    throw ErrnoException("Failed to set the size of the shared memory object");

  if((m_buffer = mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0)) == NULL)
    throw ErrnoException("Failed to mmap shared object");

  m_queue.reset(new RingBuffer(m_size, m_buffer));
}

PipeSocketBase::~PipeSocketBase(){
  if(m_doUnlink){
    shm_unlink(m_name.c_str());
  }
  munmap((char *)m_buffer - sizeof(bool), m_size + sizeof(bool));
  close(m_fd);

  free(m_receiveBuffer);
}

void PipeSocketBase::send(void *buffer, size_t size, void *hint){
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

size_t PipeSocketBase::recv(void **buffer, size_t size){
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

