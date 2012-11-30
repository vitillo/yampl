#ifndef YAMPL_ZMQ_SERVERSOCKET_H
#define YAMPL_ZMQ_SERVERSOCKET_H

#include "yampl/ZMQ/SocketBase.h"

namespace yampl{
namespace ZMQ{

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
