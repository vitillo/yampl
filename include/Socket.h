#ifndef IPC_SOCKET_H
#define IPC_SOCKET_H

#include <unistd.h>
#include <zmq.hpp>
#include <iostream>

#include "Channel.h"
#include "Exceptions.h"

namespace IPC{

void defaultDeallocator(void *, void *);

class ISocket{
  public:
    virtual void send(const void *buffer, size_t size, void *hint = NULL) = 0;
    virtual size_t receive(void **buffer, size_t size = 0) = 0;
    virtual ~ISocket(){}
};

}

#endif
