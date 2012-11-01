#ifndef IPC_ISOCKET_H
#define IPC_ISOCKET_H

#include <unistd.h>

#include "Exceptions.h"

namespace IPC{

class ISocket{
  public:
    virtual ~ISocket(){}

    virtual void send(void *buffer, size_t size, void *hint = 0) = 0;
    virtual size_t recv(void **buffer, size_t size = 0) = 0;

    template <typename T>
    void send(const T *buffer, size_t size, void *hint = 0){
      send((void *)buffer, size, hint);
    }

    template <typename T>
    size_t recv(T **buffer, size_t size = 0){
      return recv((void **)buffer, size);
    }

    template <typename T>
    void send(const T &value, void *hint = 0){
      //C++11 is_trivially_copyable
      if(__has_trivial_copy(T))
	send(&value, sizeof(value), hint);
      else
	throw InvalidOperationException();
    }

    template <typename T>
    T & recv(){
      //C++11 is_trivially_copyable
      if(__has_trivial_copy(T)){
	T *buffer = 0;
	recv(&buffer);
	return *buffer;
      }else
	throw InvalidOperationException();
    }

  private:
    ISocket & operator=(const ISocket &);
};

}

#endif
