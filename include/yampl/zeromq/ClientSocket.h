#ifndef YAMPL_ZEROMQ_CLIENTSOCKET_H
#define YAMPL_ZEROMQ_CLIENTSOCKET_H

#include "yampl/zeromq/SocketBase.h"

namespace yampl{
namespace zeromq{

class ClientSocket : public SocketBase{
  public:
    ClientSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name);
    virtual ~ClientSocket();

    virtual void send(SendArgs& args);
    virtual ssize_t recv(RecvArgs& args);

  private:
    ClientSocket(const ClientSocket &);
    ClientSocket & operator=(const ClientSocket &);

    void connect();

    bool m_isConnected;
};

}
}

#endif
