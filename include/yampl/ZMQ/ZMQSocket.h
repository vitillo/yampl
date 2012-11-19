#ifndef YAMPL_ZMQ_SOCKET_H
#define YAMPL_ZMQ_SOCKET_H

#include <tr1/unordered_map>

#include "yampl/Channel.h"
#include "yampl/ISocket.h"
#include "yampl/ISocketFactory.h"
#include "yampl/Exceptions.h"

namespace zmq{
  class context_t;
  class socket_t;
  class message_t;
}

namespace yampl{
namespace ZMQ{

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
    std::string m_identity;

  private:
    SocketBase(const SocketBase &);
    SocketBase & operator=(const SocketBase &);
};

class ClientSocket : public SocketBase{
  public:
    ClientSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *));
    virtual ~ClientSocket();

    virtual void send(SendArgs& args);
    virtual ssize_t recv(RecvArgs& args);

  private:
    bool m_isConnected;

    ClientSocket(const ClientSocket &);
    ClientSocket & operator=(const ClientSocket &);
    void connect();
 };

class ServerSocket : public SocketBase{
  public:
    ServerSocket(Channel channel, zmq::context_t *context, Semantics semantics, void (*deallocator)(void *, void *));
    virtual ~ServerSocket();
 
    virtual void send(SendArgs& args);
    virtual ssize_t recv(RecvArgs& args);

  private:
    ServerSocket(const ClientSocket &);
    ServerSocket & operator=(const ClientSocket &);
    void sendMessage(zmq::message_t &message, const std::string *peerId);

    zmq::message_t *m_lastAddress;
 };

}
}

#endif
