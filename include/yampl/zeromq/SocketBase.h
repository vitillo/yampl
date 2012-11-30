#ifndef YAMPL_ZEROMQ_SOCKETBASE_H
#define YAMPL_ZEROMQ_SOCKETBASE_H

#include "yampl/ISocket.h"
#include "yampl/Channel.h"
#include "yampl/ISocketFactory.h"

namespace zmq{
  class context_t;
  class socket_t;
  class message_t;
}

namespace yampl{
namespace zeromq{

class SocketBase : public ISocket{
  public:
    virtual ~SocketBase();
    
  protected:
    SocketBase(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *), int type);

    Channel m_channel;
    Semantics m_semantics;
    void (*m_deallocator)(void *, void *);
    zmq::socket_t *m_socket;
    zmq::message_t *m_message;
    bool m_isRecvPending;

  private:
    SocketBase(const SocketBase &);
    SocketBase & operator=(const SocketBase &);
};

}
}

#endif
