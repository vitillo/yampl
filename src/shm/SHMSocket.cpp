#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Exceptions.h"
#include "shm/SHMSocket.h"

using namespace std;

namespace IPC{
namespace shm{

SocketBase::SocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_size(1024), m_doUnlink(false), m_fd(-1), m_name("/" + channel.name){
  if((m_fd = shm_open(m_name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRWXU)) == -1){
    m_doUnlink = true;
  }else if((m_fd = shm_open(m_name.c_str(), O_RDWR | O_CREAT, S_IRWXU)) == -1){
    throw ErrnoException("Failed to open shared memory object");
  }

  if(ftruncate(m_fd, m_size) == -1)
    throw ErrnoException("Failed to set the size of the shared memory object");

  if((m_buffer = mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0)) == NULL)
    throw ErrnoException("Failed to mmap shared object");

  m_flag = (bool*)m_buffer;
  m_buffer = (char *)m_buffer + sizeof(bool);
  m_size -= sizeof(bool);
}

SocketBase::~SocketBase(){
  if(m_doUnlink){
    shm_unlink(m_name.c_str());
  }
  munmap((char *)m_buffer - sizeof(bool), m_size + sizeof(bool));
  close(m_fd);
}

void SocketBase::send(void *buffer, size_t size, void *hint){
  size_t offset = 0;
  size_t len = min(size, m_size - sizeof(size));

  while(!__sync_bool_compare_and_swap(m_flag, 0, 0)) //Acquire barrier not needed on x86 but is still needed for the compiler
    while(*m_flag); //Loop over cacheline

  memcpy(m_buffer, &size, sizeof(size));
  memcpy((char *)m_buffer + sizeof(size), buffer, len);
  *m_flag = true; 

  __sync_bool_compare_and_swap(m_flag, 0, 1); // Release barrier not needed on x86 but is still needed for the compiler
  offset += len;

  while(offset != size){
    while(!__sync_bool_compare_and_swap(m_flag, 0, 0))
      while(!*m_flag);

    len = min(size, m_size);
    memcpy(m_buffer, (char *)buffer + offset, len);
    offset += len;

    __sync_bool_compare_and_swap(m_flag, 0, 1);
  }
}

size_t SocketBase::recv(void **buffer, size_t size){
  return 0;
}

}
}

