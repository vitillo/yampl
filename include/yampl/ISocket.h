#ifndef YAMPL_ISOCKET_H
#define YAMPL_ISOCKET_H

#include <unistd.h>

#include <string>

using namespace std;

#include "yampl/Exceptions.h"

namespace yampl{

class ISocket{
  public:
    typedef std::string discriminator_t;

    virtual ~ISocket(){}

    /**
     * Sends a message (void *) to one of its peers (buffer-blocking). When multiple peers are connected to the same channel, the peer to which the message is sent to is identified with a discriminator.
     *
     * @param buffer the message that contains the data to send
     * @param size the size of the message to send
     * @param discriminator the identifier of the peer to which the message has to be sent, if equals to zero then the implementation will choose the last peer from which it has received a message, in the case of a ServerSocket, and the only peer in the case of a ClientSocket
     * @param hint the pointer is passed to the deallocator and can be used to implement complex deallocation policies
     */
    virtual void send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0) = 0;

    /**
     * Tries to send a message to one of its peers.
     *
     * @param timeout number of milliseconds to wait for a message to be sent; if timeout is 0 then try_send will return immediately while if timeout is -1 then it might wait indefinitely just like send() if the underlying transmission buffer is full
     * @return true if the message was sent, false otherwise
     */
    virtual bool try_send(void *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0, long timeout = 0){
      throw UnsupportedException();
    }

    /**
     * Receives a message from one of its peers (blocking). The identifier of the sending peer is returned through the discriminator parameter.
     *
     * @param buffer pointer to the buffer in which the receiving message will be stored; if buffer points to a NULL-pointer then the implementation will allocate a buffer for the message to be received
     * @param size the size of the buffer; the size is ignored if buffer points to a NULL-pointer
     * @param discriminator the identifier of the peer from which the data has been received
     * @return the size of the received message
     */
    virtual size_t recv(void **buffer, size_t size, discriminator_t *discriminator = 0) = 0;

    /**
     * Tries to receive a message from one of its peers.
     *
     * @param timeout number of milliseconds to wait for a message; if timeout is 0 then try_recv will return immediately while if timeout is -1 then it will wait indefinitely just like recv()
     * @return the size of the received message if successful or -1 if no message was received
     */
    virtual size_t try_recv(void **buffer, size_t size, discriminator_t *discriminator = 0, long timeout = 0){
      throw UnsupportedException();
    }

    template <typename T>
    void send(T *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0){
      send((void *)buffer, size, discriminator, hint);
    }

    template <typename T>
    void try_send(T *buffer, size_t size, discriminator_t *discriminator = 0, void *hint = 0, long timeout = 0){
      try_send((void *)buffer, size, discriminator, hint, timeout);
    }

    template <typename T>
    size_t recv(T **buffer, size_t size = 0, discriminator_t* discriminator = 0){
      return recv((void **)buffer, size, discriminator);
    }

    template <typename T>
    size_t try_recv(T **buffer, size_t size = 0, discriminator_t* discriminator = 0){
      return try_recv((void **)buffer, size, discriminator);
    }

    template <typename T>
    void send(T &value, discriminator_t *discriminator = 0, void *hint = 0){
      //C++11 is_trivially_copyable
      if(__has_trivial_copy(T))
	send(&value, sizeof(value), discriminator, hint);
      else
	throw InvalidOperationException();
    }

    template <typename T>
    void try_send(T &value, discriminator_t *discriminator = 0, void *hint = 0, long timeout = 0){
      if(__has_trivial_copy(T))
	try_send(&value, sizeof(value), discriminator, hint, timeout);
      else
	throw InvalidOperationException();
    }

    template <typename T>
    T & recv(discriminator_t *discriminator = 0){
      if(__has_trivial_copy(T)){
	T *buffer = 0;
	recv(&buffer);
	return *buffer;
      }else
	throw InvalidOperationException();
    }

    template <typename T>
    T & try_recv(discriminator_t *discriminator = 0){
      if(__has_trivial_copy(T)){
	T *buffer = 0;
	try_recv(&buffer);
	return *buffer;
      }else
	throw InvalidOperationException();
    }

  private:
    ISocket & operator=(const ISocket &);
};

}

#endif
