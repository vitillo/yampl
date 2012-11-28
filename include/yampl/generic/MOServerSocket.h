#ifndef YAMPL_MOSERVERSOCKET_H
#define YAMPL_MOSERVERSOCKET_H

#include <tr1/memory>
#include <vector>
#include <map>

#include "yampl/ISocket.h"
#include "yampl/utils/RawPipe.h"
#include "yampl/utils/Thread.h"
#include "yampl/utils/Poller.h"
#include "yampl/utils/SpinLock.h"

namespace yampl{

template <typename T>
class MOServerSocket: public ISocket{
  public:
    MOServerSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_currentPeer(0), m_destroy(false){
      m_listener.reset(new Thread(std::tr1::bind(&MOServerSocket::listenerThreadFun, this, channel, semantics, deallocator)));
    }

    virtual ~MOServerSocket(){
      m_destroy = true;
      m_listener->cancel();
    }

    virtual void send(SendArgs &args){
      T *peer;

      if(args.peerId){
	typename IdToPeerMap::iterator elem;

	if((elem = m_idToPeer.find(*args.peerId)) != m_idToPeer.end()){
	  peer = elem->second;
	}else{
	  throw UnroutableException();
	}
      }else if(!m_currentPeer){
	throw UnroutableException();
      }else{
	peer = m_currentPeer;
      }

      peer->send(args);
    }

    virtual ssize_t recv(RecvArgs &args) = 0;

  protected:
    typedef std::map<std::string, T*> IdToPeerMap;
    typedef std::map<T*, std::string> PeerToIdMap;

    T *m_currentPeer;
    SpinLock m_lock;
    std::vector<std::tr1::shared_ptr<T> > m_peers;
    IdToPeerMap m_idToPeer;
    PeerToIdMap m_peerToId;

    virtual void listenTo(std::tr1::shared_ptr<T> socket) = 0;

  private:
    MOServerSocket(const MOServerSocket &);
    MOServerSocket & operator=(const MOServerSocket &);

    void listenerThreadFun(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)){
      Poller poller;
      RawPipe listener("/tmp/" + channel.name + "_announce");

      poller.add(listener.getReadFD());

      while(!m_destroy){
	if(poller.poll() == -1)
	  continue;

	UUID id = UUID::readFrom(listener);
	Channel peerChannel(channel.name + "_" + (std::string)id);
	std::tr1::shared_ptr<T> peer(new T(peerChannel, semantics, deallocator));

	m_lock.lock();
	m_peers.push_back(peer);
	m_idToPeer[id] = peer.get();
	m_peerToId[peer.get()] = id;
	m_lock.unlock();

	listenTo(peer);
      }
    }

    bool m_destroy;
    std::tr1::shared_ptr<Thread> m_listener;
};

}

#endif
