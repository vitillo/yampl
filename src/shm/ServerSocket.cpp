#include "yampl/shm/ServerSocket.h"

namespace yampl{
namespace shm{

void ServerSocket::notifyPeerDisconnection(PeerList::iterator it){
#ifndef NDEBUG
  std::tr1::shared_ptr<SimpleServerSocket> peer = *it;
  ConsumerSocket *consumer = peer->getConsumerSocket();
  assert(consumer && !consumer->hasPendingMessages());
#endif

  m_nextPeerToVisit = ++it;
}

ssize_t ServerSocket::recv(RecvArgs &args){
  if(!m_isRecvPending){
    m_semaphore->down(1, args.timeout);
    m_isRecvPending = true;
  }
  
  // Using a futex and iterating over all open sockets should still be faster than using e.g. eventfd and performing a systemcall to poll. The whole point of using a SHMSocket is to keep the latency as low as possible.
  m_lock.lock();
  bool spurious = true;

  for(size_t i = 0; i < m_peers.size(); i++){
    if(m_nextPeerToVisit == m_peers.end()) 
      m_nextPeerToVisit = m_peers.begin();

    std::tr1::shared_ptr<SimpleServerSocket> peer = *m_nextPeerToVisit;
    ConsumerSocket *consumer = peer->getConsumerSocket();

    if(consumer->hasPendingMessages()){
      m_currentPeer = peer;
      spurious = false;
      break;
    }

    m_nextPeerToVisit++;
  }

  assert(!spurious);

  // If a receive is pending we need to proceed reading from the same socket the next time recv is called.
  try{
    size_t ret = m_currentPeer->recv(args);

    if(args.peerIdOut){
      *args.peerIdOut = m_peerToId[m_currentPeer];
    }

    m_isRecvPending = false;
    m_lock.unlock();

    return ret;
  }catch(InvalidSizeException){
    //TODO: Perform next receive from same peer
    m_lock.unlock();

    throw InvalidSizeException();
  }
}

}
}

