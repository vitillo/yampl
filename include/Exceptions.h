#ifndef IPC_EXCEPTIONS_H
#define IPC_EXCEPTIONS_H

#include <exception>

namespace IPC{

class IPCException: public std::exception{
  public:
    IPCException(const char * msg) : m_msg(msg){}
    virtual ~IPCException() throw() {}

    virtual const char * what() const throw(){
      return m_msg;
    }

  private:
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

}

#endif
