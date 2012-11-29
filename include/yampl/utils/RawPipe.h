#ifndef YAMPL_RAWPIPE_H
#define YAMPL_RAWPIPE_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "yampl/Exceptions.h"

namespace yampl{

class RawPipe{
  public:
    RawPipe(const std::string &name){
      if(mkfifo(name.c_str(), S_IRWXU) == -1 && errno != EEXIST)
	throw ErrnoException("Failed to create FIFO");

      if(errno != EEXIST)
	m_doUnlink = true;

      // Deadlock prevention, open FIFO in RD and WR mode (O_RDWR unspecified for FIFO)
      if((readfd = open(name.c_str(), O_RDONLY | O_NONBLOCK)) == -1)
	throw ErrnoException("Failed to open FIFO for reading");
      if((writefd = open(name.c_str(), O_WRONLY)) == -1){
	close(readfd);
	throw ErrnoException("Failed to open FIFO for writing");
      }

      if((fcntl(readfd, F_SETFL, fcntl(readfd, F_GETFL) & ~O_NONBLOCK)) == -1){
	close(readfd);
	close(writefd);
	throw ErrnoException("Failed to update FIFO");
      }
    }

    ~RawPipe(){
      close(readfd);
      close(writefd);

      if(m_doUnlink){
	unlink(m_name.c_str());
      }
    }

    size_t read(void *buffer, size_t size){
      size_t bytesRead = 0;

      while(bytesRead != size){
	ssize_t r = TEMP_FAILURE_RETRY(::read(readfd, (char *)buffer + bytesRead, size - bytesRead));

	if(r == -1)
	  throw ErrnoException("Read from pipe failed");

	bytesRead += r;
      }

      return bytesRead;
    }

    void write(const void *buffer, size_t size){
      size_t bytesWritten = 0;

      while(bytesWritten != size){
	ssize_t sent = TEMP_FAILURE_RETRY(::write(this->writefd, (char *)buffer + bytesWritten, size - bytesWritten));

	if(sent == -1)
	  throw ErrnoException("Write to pipe failed");

	bytesWritten += sent;
      }
    }

    void writeSplice(const void *buffer, size_t size){
      struct iovec vec;
      size_t bytesWritten = 0;

      for(bytesWritten = 0; bytesWritten != size;){
	ssize_t sent = 0;
	vec.iov_base = ((char *) buffer) + bytesWritten;
	vec.iov_len = size - bytesWritten;

	if((sent = vmsplice(writefd, &vec, 1, 0)) == -1)
	  throw ErrnoException("Send failed");

	bytesWritten += sent;
      }
    }

    int getReadFD(){
      return readfd;
    }

    int getWriteFD(){
      return writefd;
    }

  private:
    RawPipe(const RawPipe &);
    RawPipe & operator=(const RawPipe &);

    const std::string m_name;
    bool m_doUnlink;
    int readfd;
    int writefd;
};

}

#endif
