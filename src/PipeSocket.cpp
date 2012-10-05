#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>

#include "PipeSocket.h"

using namespace std;

namespace IPC{

PipeBaseSocket::PipeBaseSocket(const Channel &channel, Mode mode, bool hasOwnership, bool fastTransfer) : m_pipename("/tmp/fifo_" + channel.name), m_mode(mode), m_hasOwnership(hasOwnership), m_fast(fastTransfer){
  if(mkfifo(m_pipename.c_str(), S_IRWXU) == -1 && errno != EEXIST)
    throw ErrnoException("Can't create FIFO.", errno);

  if((m_pipe = open(m_pipename.c_str(), m_mode == PIPE_READ ? O_RDONLY : O_WRONLY)) == -1)
    throw;

  fcntl(m_pipe, F_SETPIPE_SZ, 1048576);
}

PipeBaseSocket::~PipeBaseSocket(){

}

void PipeBaseSocket::send(const void *buffer, size_t size, void *hint){
  size_t sent = 0;
  struct iovec vec;

  write(m_pipe, &size, sizeof(size));

  while(sent != size){
    vec.iov_base = ((char *) buffer) + sent;
    vec.iov_len = size - sent;
    if(m_fast)
      sent += vmsplice(m_pipe, &vec, 1, 0);
    else
      sent += write(m_pipe, (char *) buffer + sent, size - sent);
  }
}

size_t PipeBaseSocket::receive(void **buffer, size_t size){
  static size_t msg_size = 0;
  static void *receive_buffer = 0;
  size_t bytesRead = 0;

  if(!msg_size && !read(m_pipe, &msg_size, sizeof(msg_size)))
      throw IPCException("Receive failed, FIFO closed by writer");

  if(m_hasOwnership){
    if((receive_buffer = realloc(receive_buffer, msg_size)) == 0)
      throw ErrnoException("Receive failed");
    *buffer = receive_buffer;
  }else{
    if(*buffer && size < msg_size)
      throw InvalidSizeException();
    else if(!*buffer)
      *buffer = malloc(msg_size);
  }

  while(bytesRead != msg_size){
    ssize_t r = 0;
    if((r = read(m_pipe, (char *)*buffer + bytesRead, msg_size - bytesRead)) == -1)
      throw ErrnoException("Receive failed");
    else if(r == 0)
      throw IPCException("Receive failed, FIFO closed by writer");

    bytesRead += r;
  }

  msg_size = 0;
  return bytesRead;
}

}

