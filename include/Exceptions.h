#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <cstdio>
#include <cstring>

#include <errno.h>

namespace IPC{

class IPCException: public std::exception{
  public:
    IPCException(const char * msg) : m_msg(msg){}
    virtual ~IPCException() throw() {}

    virtual const char * what() const throw(){
      return m_msg;
    }

  protected:
    const char * m_msg;
};

class UnsupportedException: public IPCException{
  public:
  UnsupportedException() : IPCException("This feature is not supported yet"){}
};

class InvalidOperationException: public IPCException{
  public:
  InvalidOperationException(): IPCException("Operation not permitted"){}
};

class InvalidSizeException: public IPCException{
  public:
  InvalidSizeException(): IPCException("Buffer size is not sufficient"){}
};

class ErrnoException : public IPCException{
  public:
    ErrnoException(const char * msg = "System call error"): IPCException(msg), m_errno(errno) {
    }

    int getErrno(){
      return m_errno;
    }

    virtual const char * what() const throw(){
      snprintf(m_errnoMsg, s_maxMsgSize, "%s: %s", m_msg, strerror(m_errno));
      return m_errnoMsg;
    }

  private:
    static const size_t s_maxMsgSize = 100;
    mutable char m_errnoMsg[s_maxMsgSize];
    int m_errno;
};

}

#endif
