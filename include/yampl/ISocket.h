#ifndef YAMPL_ISOCKET_H
#define YAMPL_ISOCKET_H

#include <unistd.h>
#include <iostream>

#include "yampl/Exceptions.h"

namespace yampl{

class ISocket{
  public:
    virtual ~ISocket(){}

    /**
     * Sends a message (void *) to one of its peers (buffer-blocking). When 
     * multiple peers are connected to the same channel, the peer to which the 
     * message is sent can be specified with peerID.
     *
     * @param buffer the message that contains the data to send
     * @param size the size of the message to send
     * @param peerID the identifier of the peer to which the message has 
     *        to be sent, if equals to zero then the implementation will choose 
     *        the last peer from which it has received a message, in the case of 
     *        a ServerSocket, and the only available peer, in the case of a 
     *        ClientSocket
     * @param hint this argument is passed to the deallocator and can be used to 
     *        implement complex deallocation policies
     *
     * @throw UnroutableException if the message can't be sent to the specified 
     *        peerID (e.g. peer might be temporarly unavailable)
     * @throw ErrnoException if an unrecovable error occurs
     */
    virtual void send(void *buffer, size_t size, const std::string &peerID = NO_ID, void *hint = 0) = 0;

    /**
     * Tries to send a message to one of its peers.
     *
     * @param timeout number of milliseconds to wait for a message to be sent; 
     *        if timeout is 0 then try_send will return immediately while 
     *        if timeout is -1 then it might wait indefinitely just like send() 
     *        if the underlying transmission buffer is full
     * @return true if the message was sent, false otherwise
     */
    virtual bool try_send(void *buffer, size_t size, const std::string &peerID = NO_ID, void *hint = 0, long timeout = 0){
      throw UnsupportedException();
    }

    /**
     * Receives a message from one of its peers (blocking). The identifier of 
     * the sending peer can be specified through the peerID parameter. If peerID
     * is a NULL-pointer then the implementation will return the first available 
     * message from any peer and return the sender's ID through peerID.
     *
     * @param buffer pointer to the buffer in which the receiving message will 
     *        be stored; if buffer is a NULL-pointer then the implementation 
     *        will allocate a buffer for the message to be received
     * @param size the size of the buffer; the size is ignored if buffer is a 
     *        NULL-pointer
     * @param peerID if peerID is an empty string then the identifier of the 
     *        peer from which the data has been received will be returned; 
     *        otherwise peerID specifies the sending peer
     * @return the size of the received message
     *
     * @throw InvalidSizeException if the buffer size is not sufficient to store 
     *        the incoming message 
     * @throw ErrnoException if an unrecovable error occurs
     */
    virtual ssize_t recv(void *&buffer, size_t size, const std::string *&peerID = NO_ID_PTR) = 0;

    /**
     * Tries to receive a message from one of its peers.
     *
     * @param timeout number of milliseconds to wait for a message; if timeout 
     *        is 0 then try_recv will return immediately while if timeout is -1 
     *        then it will wait indefinitely just like recv()
     * @return the size of the received message if successful or -1 if no message 
     *         was received
     */
    virtual ssize_t try_recv(void *&buffer, size_t size, const std::string *&peerID = NO_ID_PTR, long timeout = 0){
      throw UnsupportedException();
    }

    template <typename T>
    void send(T *buffer, size_t size, const std::string &peerID = NO_ID, void *hint = 0){
      send((void *)buffer, size, peerID, hint);
    }

    template <typename T>
    void try_send(T *buffer, size_t size, const std::string &peerID = NO_ID, void *hint = 0, long timeout = 0){
      try_send((void *)buffer, size, peerID, hint, timeout);
    }

    template <typename T>
    ssize_t recv(T *&buffer, size_t size = 0, const std::string *&peerID = NO_ID_PTR){
      void **tmp = (void **)&buffer;
      return recv(*tmp, size, peerID);
    }

    template <typename T, int N>
    ssize_t recv(T (&buffer)[N], const std::string *&peerID = NO_ID_PTR){
      T *ptr = &buffer[0];
      return recv(ptr, N, peerID);
    }

    template <typename T>
    ssize_t try_recv(T *&buffer, size_t size = 0, const std::string *&peerID = NO_ID_PTR){
      void **tmp = (void **)&buffer;
      return try_recv(*tmp, size, peerID);
    }

    template <typename T, int N>
    ssize_t try_recv(T (&buffer)[N], const std::string *&peerID = NO_ID_PTR){
      T *ptr = &buffer[0];
      return try_recv(ptr, N, peerID);
    }

    template <typename T>
    void send(const T &value, const std::string &peerID = NO_ID, void *hint = 0){
      //C++11 is_trivially_copyable
      if(__has_trivial_copy(T))
	send(&value, sizeof(value), peerID, hint);
      else
	throw InvalidOperationException("Type is not trivially copiable");
    }

    template <typename T>
    void try_send(const T &value, const std::string &peerID = NO_ID, void *hint = 0, long timeout = 0){
      if(__has_trivial_copy(T))
	try_send(&value, sizeof(value), peerID, hint, timeout);
      else
	throw InvalidOperationException();
    }

    template <typename T>
    T & recv(const std::string *&peerID = NO_ID_PTR){
      if(__has_trivial_copy(T)){
	T *buffer = 0;
	recv(buffer, sizeof(T), peerID);
	return *buffer;
      }else
	throw InvalidOperationException("Type is not trivially copiable");
    }

    template <typename T>
    T & try_recv(const std::string *&peerID = NO_ID_PTR, long timeout = 0){
      if(__has_trivial_copy(T)){
	T *buffer = 0;
	try_recv(buffer, sizeof(T), peerID, timeout);
	return *buffer;
      }else
	throw InvalidOperationException("Type is not trivially copiable");
    }

    void send(const std::string &msg, const std::string &peerID = NO_ID, void *hint = 0){
      send(msg.c_str(), msg.size() + 1);
    }

  protected:
    static const std::string NO_ID;
    static const std::string *NO_ID_PTR;

  private:
    ISocket & operator=(const ISocket &);
};

}

#endif
