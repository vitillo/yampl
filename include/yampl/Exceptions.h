#ifndef YAMPL_EXCEPTIONS_H
#define YAMPL_EXCEPTIONS_H

#include <exception>
#include <cstdio>
#include <cstring>

#include <errno.h>

namespace yampl{

class Exception: public std::exception{
  public:
    Exception(const char * msg) : m_msg(msg){}
    virtual ~Exception() throw() {}

    virtual const char * what() const throw(){
      return m_msg;
    }

  protected:
    const char * m_msg;
};

class UnsupportedException: public Exception{
  public:
  UnsupportedException() : Exception("This feature is not supported yet"){}
};

class InvalidOperationException: public Exception{
  public:
  InvalidOperationException(const char *msg = "Operation not permitted"): Exception(msg){}
};

class InvalidSizeException: public Exception{
  public:
  InvalidSizeException(): Exception("Buffer size is not sufficient"){}
};

class UnroutableException : public Exception{
  public:
    UnroutableException(): Exception("Destination peer not found"){}
};

class ErrnoException : public Exception{
  public:
    ErrnoException(const char *msg = "System call error"): Exception(msg), m_errno(errno) {
    }

    ErrnoException(int err, const char *msg = "System call error"): Exception(msg), m_errno(err){}

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
