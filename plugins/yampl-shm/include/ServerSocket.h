#ifndef YAMPL_SHM_SERVERSOCKET_H
#define YAMPL_SHM_SERVERSOCKET_H

#include "yampl/generic/ServerSocketBase.h"
#include "SimpleServerSocket.h"

namespace yampl{
namespace shm{

class ServerSocket : public yampl::ServerSocketBase<SimpleServerSocket>{
  public:
    ServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    void accept(SimpleServerSocket *socket);
    virtual ssize_t recv(RecvArgs &args);

  protected:
    typedef ServerSocketBase<SimpleServerSocket>::PeerList PeerList;

    void notifyPeerDisconnection(PeerList::iterator it);

  private:
    std::tr1::shared_ptr<SharedMemory> m_memory;
    std::tr1::shared_ptr<Semaphore> m_semaphore;
    PeerList::iterator m_nextPeerToVisit;
    bool m_isRecvPending;
};

inline ServerSocket::ServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : yampl::ServerSocketBase<SimpleServerSocket>(channel, semantics, deallocator, std::tr1::bind(&ServerSocket::accept, this, std::tr1::placeholders::_1)), m_isRecvPending(false){
  m_memory.reset(new SharedMemory(channel.name + "_sem", sizeof(int)));
  m_semaphore.reset(new Semaphore((int *)m_memory->getMemory()));
  m_nextPeerToVisit = m_peers.begin();
}

inline void ServerSocket::accept(SimpleServerSocket *socket){
  // Send synchronization message
  static_cast<ISocket *>(socket)->send();
}

}
}

#endif
