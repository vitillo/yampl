#ifndef YAMPL_SHAREDMEMORY_H
#define YAMPL_SHAREDMEMORY_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>

namespace yampl{

class SharedMemory{
  public:
    SharedMemory(const std::string& name, size_t size, bool create) : m_name(name), m_size(size), m_fd(-1), m_buffer(0){
      if(create){
	if((m_fd = shm_open(m_name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRWXU)) == -1)
	  throw ErrnoException("Shared memory object already exists");
      }else{
        while((m_fd = shm_open(m_name.c_str(), O_RDWR, S_IRWXU)) == -1 && errno == ENOENT);
	  if(m_fd == -1)
	    throw ErrnoException("Failed to open shared memory object");
      }

      if(ftruncate(m_fd, m_size) == -1)
	throw ErrnoException("Failed to set the size of the shared memory object");

      if((m_buffer = mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0)) == NULL)
	throw ErrnoException("Failed to mmap shared object");
    }

    ~SharedMemory(){
      if(munmap((char *)m_buffer, m_size))
	throw ErrnoException();

      if(close(m_fd))
	throw ErrnoException();
    }

    void *getMemory(){
      return m_buffer;
    }

  private:
    std::string m_name;
    size_t m_size;
    int m_fd;
    void *m_buffer;
};

}

#endif
