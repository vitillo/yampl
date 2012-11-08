#ifndef YAMPL_ISOCKET_H
#define YAMPL_ISOCKET_H

#include <unistd.h>

#include <string>

#include "Exceptions.h"

namespace YAMPL{

class ISocket{
  public:
    typedef std::string discriminator_t;

    virtual ~ISocket(){}

    virtual void send(void *buffer, size_t size, const discriminator_t *discriminator = 0, void *hint = 0) = 0;
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0) = 0;

    template <typename T>
    void send(const T *buffer, size_t size, const discriminator_t *discriminator = 0, void *hint = 0){
      send((void *)buffer, size, discriminator, hint);
    }

    template <typename T>
    size_t recv(T **buffer, size_t size = 0, discriminator_t* discriminator = 0){
      return recv((void **)buffer, size, discriminator);
    }

    template <typename T>
    void send(const T &value, const discriminator_t *discriminator = 0, void *hint = 0){
      //C++11 is_trivially_copyable
      if(__has_trivial_copy(T))
	send(&value, sizeof(value), discriminator, hint);
      else
	throw InvalidOperationException();
    }

    template <typename T>
    T & recv(discriminator_t *discriminator = 0){
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
