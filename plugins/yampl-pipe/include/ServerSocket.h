#ifndef YAMPL_PIPE_SERVERSOCKET_H
#define YAMPL_PIPE_SERVERSOCKET_H
#include <iostream>

#include "yampl/generic/ServerSocketBase.h"
#include "SimpleServerSocket.h"

namespace yampl{
namespace pipe{

class ServerSocket : public yampl::ServerSocketBase<SimpleServerSocket>{
  public:
    ServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    void accept(SimpleServerSocket *socket);
    virtual ssize_t recv(RecvArgs &args);

  private:
    ServerSocket(const ServerSocket &);
    ServerSocket & operator=(const ServerSocket &);

    Poller m_peerPoll;
};

inline ServerSocket::ServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : yampl::ServerSocketBase<SimpleServerSocket>(channel, semantics, deallocator, std::tr1::bind(&ServerSocket::accept, this, std::tr1::placeholders::_1)){
}

inline void ServerSocket::accept(SimpleServerSocket *socket){
  m_peerPoll.add(socket->getConsumerSocket()->m_transferPipe->getReadFD(), socket);
}

inline ssize_t ServerSocket::recv(RecvArgs &args){
 SimpleServerSocket *peer;

 if(!m_peerPoll.poll((void **)&peer, args.timeout)){
    return -1;
  }else{
    m_lock.lock();
    m_currentPeer = m_rawToPeer[peer];

    if(args.peerIdOut)
      *args.peerIdOut = m_peerToId[m_currentPeer];

    m_lock.unlock();
    return m_currentPeer->recv(args);
  }
}

}
}

#endif
