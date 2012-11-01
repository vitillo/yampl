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
}

SocketBase::~SocketBase(){
  if(m_doUnlink){
    shm_unlink(m_name.c_str());
  }
    
  close(m_fd);
}

void SocketBase::send(void *buffer, size_t size, void *hint){

}

size_t SocketBase::recv(void **buffer, size_t size){
  return 0;
}

}
}

