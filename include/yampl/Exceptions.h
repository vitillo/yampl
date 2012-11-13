#ifndef YAMPL_EXCEPTIONS_H
#define YAMPL_EXCEPTIONS_H

#include <exception>
#include <cstdio>
#include <cstring>

#include <errno.h>

namespace yampl{

class YAMPLException: public std::exception{
  public:
    YAMPLException(const char * msg) : m_msg(msg){}
    virtual ~YAMPLException() throw() {}

    virtual const char * what() const throw(){
      return m_msg;
    }

  protected:
    const char * m_msg;
};

class UnsupportedException: public YAMPLException{
  public:
  UnsupportedException() : YAMPLException("This feature is not supported yet"){}
};

class InvalidOperationException: public YAMPLException{
  public:
  InvalidOperationException(const char *msg = "Operation not permitted"): YAMPLException(msg){}
};

class InvalidSizeException: public YAMPLException{
  public:
  InvalidSizeException(): YAMPLException("Buffer size is not sufficient"){}
};

class ErrnoException : public YAMPLException{
  public:
    ErrnoException(const char *msg = "System call error"): YAMPLException(msg), m_errno(errno) {
    }

    ErrnoException(int err, const char *msg = "System call error"): YAMPLException(msg), m_errno(err){}

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
