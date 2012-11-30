#include "yampl/shm/ServerSocket.h"

namespace yampl{
namespace shm{

ssize_t ServerSocket::recv(RecvArgs &args){
  if(!m_isRecvPending){
    m_semaphore->down(1, args.timeout);
    m_isRecvPending = true;
  }
  
  // Using a futex and iterating over all open sockets should still be faster than using e.g. eventfd and performing a systemcall to poll. The whole point of using a SHMSocket is to keep the latency as low as possible.
  size_t i = m_nextPeerToVisit;
  m_lock.lock();

  for(size_t j = 0; j != m_peers.size(); i = (i + 1) % m_peers.size(), j++){
    std::tr1::shared_ptr<SimpleServerSocket> peer = m_peers[i];
    ConsumerSocket *consumer = peer->getConsumerSocket();

    if(consumer->hasPendingMessages()){
      m_currentPeer = peer.get();
      m_nextPeerToVisit = (i + 1) % m_peers.size();
      break;
    }
  }

  m_lock.unlock();

  // If a receive is pending we need to proceed reading from the same socket the next time recv is called.
  try{
    size_t ret = m_currentPeer->recv(args);

    if(args.peerIdOut){
      *args.peerIdOut = m_peerToId[m_currentPeer];
    }

    m_isRecvPending = false;
    return ret;
  }catch(InvalidSizeException){
    m_nextPeerToVisit = (m_nextPeerToVisit - 1) % m_peers.size();
    throw InvalidSizeException();
  }
}

}
}

