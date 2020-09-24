#ifndef YAMPL_ZEROMQ_SERVERSOCKET_H
#define YAMPL_ZEROMQ_SERVERSOCKET_H

#include "SocketBase.h"

namespace yampl{
namespace zeromq{

class ServerSocket : public SocketBase{
  public:
    ServerSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *));
    virtual ~ServerSocket();
 
    virtual void send(SendArgs& args);
    virtual ssize_t recv(RecvArgs& args);

  private:
    ServerSocket(const ServerSocket &);
    ServerSocket & operator=(const ServerSocket &);

    void sendMessage(zmq::message_t &message, const std::string *peerId);

    zmq::message_t *m_lastAddress;
};

}
}

#endif
