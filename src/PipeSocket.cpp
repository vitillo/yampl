#define _GNU_SOURCE 1

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <cstdlib>

#include "PipeSocket.h"

using namespace std;

namespace IPC{

PipeBaseSocket::PipeBaseSocket(const Channel &channel, Mode mode, bool hasOwnership, bool fastTransfer) : m_pipename("/tmp/fifo_" + channel.name), m_mode(mode), m_hasOwnership(hasOwnership), m_fast(fastTransfer), m_receiveBuffer(0){

  if(mkfifo(m_pipename.c_str(), S_IRWXU) == -1 && errno != EEXIST)
    throw ErrnoException("Can't create FIFO", errno);

  // Deadlock prevention, open FIFO in RD and WR mode (O_RDWR unspecified for FIFO)
  int r, w;
  if((r = open(m_pipename.c_str(), O_RDONLY | O_NONBLOCK)) == -1)
    throw ErrnoException("Failed to open FIFO");
  if((w = open(m_pipename.c_str(), O_WRONLY)) == -1)
    throw ErrnoException("Failed to open FIFO");

  fcntl(r, F_SETFL, fcntl(m_pipe, F_GETFL) & ~O_NONBLOCK);

  if(m_mode == PIPE_READ){
    m_pipe = r;
    m_peer = w;
  }else{
    m_pipe = w;
    m_peer = r;
  }

  fcntl(m_pipe, F_SETFL, fcntl(m_pipe, F_GETFL) & ~O_NONBLOCK);

  //TODO: feature added in 2.6.35
  //fcntl(m_pipe, F_SETPIPE_SZ, 1048576);
}

PipeBaseSocket::~PipeBaseSocket(){
  close(m_peer);
  close(m_pipe);
  delete m_receiveBuffer;
}

void PipeBaseSocket::send(const void *buffer, size_t size, void *hint){
  size_t bytesWritten = 0;
  struct iovec vec;

  while(bytesWritten != sizeof(size)){
    ssize_t sent = TEMP_FAILURE_RETRY(write(m_pipe, &size, sizeof(size)));

    if(sent == -1)
      throw ErrnoException("Send failed");

    bytesWritten += sent;
  }

  for(bytesWritten = 0; bytesWritten != size;){
    ssize_t sent = 0;
    vec.iov_base = ((char *) buffer) + bytesWritten;
    vec.iov_len = size - bytesWritten;

    if(m_fast){
      if((sent = vmsplice(m_pipe, &vec, 1, 0)) == -1)
        throw ErrnoException("Send failed");
    }else{
      if((sent = TEMP_FAILURE_RETRY(write(m_pipe, (char *) buffer + bytesWritten, size - bytesWritten))) == -1)
	throw ErrnoException("Send failed");
    }

    bytesWritten += sent;
  }
}

size_t PipeBaseSocket::receive(void **buffer, size_t size){
  static size_t msg_size = 0;
  size_t bytesRead = 0;


  while(bytesRead != sizeof(msg_size)){
    ssize_t r = TEMP_FAILURE_RETRY(read(m_pipe, &msg_size, sizeof(msg_size)));

    if(r == -1)
      throw ErrnoException("Receive failed");

    bytesRead += r;
  }

  if(m_hasOwnership){
    if((m_receiveBuffer = realloc(m_receiveBuffer, msg_size)) == 0)
      throw ErrnoException("Receive failed");

    *buffer = m_receiveBuffer;
  }else{
    if(*buffer && size < msg_size)
      throw InvalidSizeException();
    else if(!*buffer)
      *buffer = malloc(msg_size);
  }

  for(bytesRead = 0; bytesRead != msg_size;){
    ssize_t r = TEMP_FAILURE_RETRY(read(m_pipe, (char *)*buffer + bytesRead, msg_size - bytesRead));

    if(r == -1)
      throw ErrnoException("Receive failed");

    bytesRead += r;
  }

  msg_size = 0;
  return bytesRead;
}

}

