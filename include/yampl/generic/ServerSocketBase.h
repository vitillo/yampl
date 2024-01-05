#ifndef YAMPL_MOSERVERSOCKET_H
#define YAMPL_MOSERVERSOCKET_H

#include <tr1/memory>
#include <tr1/functional>
#include <list>
#include <map>

#include "yampl/ISocket.h"
#include "yampl/ISocketFactory.h"
#include "yampl/Channel.h"
#include "yampl/utils/RawPipe.h"
#include "yampl/utils/Thread.h"
#include "yampl/utils/Poller.h"
#include "yampl/utils/SpinLock.h"
#include "yampl/utils/UUID.h"

namespace yampl{

template <typename T>
class ServerSocketBase: public ISocket{
  public:
    ServerSocketBase(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::tr1::function<void(T*)> &accept);
    virtual ~ServerSocketBase() noexcept(false);

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args) = 0;

  protected:
    typedef std::list<std::tr1::shared_ptr<T> > PeerList;
    typedef std::map<std::string, typename PeerList::iterator> IdToPeerMap;
    typedef std::map<std::tr1::shared_ptr<T>, std::string> PeerToIdMap;
    typedef std::map<T*, std::tr1::shared_ptr<T> > RawToPeerMap;

    virtual void notifyPeerDisconnection(typename PeerList::iterator it);

    std::tr1::shared_ptr<T> m_currentPeer;
    SpinLock m_lock;
    RawToPeerMap m_rawToPeer;
    IdToPeerMap m_idToPeer;
    PeerToIdMap m_peerToId;
    PeerList m_peers;

  private:
    ServerSocketBase(const ServerSocketBase &);
    ServerSocketBase & operator=(const ServerSocketBase &);

    void listenerThreadFun(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));
    void openConnection(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *), const UUID &uuid);
    void closeConnection(const UUID &uuid);

    std::tr1::shared_ptr<Thread> m_listener;
    std::tr1::function<void(T*)> m_accept; // needed because we can't call a virtual function in the constructor
};

template <typename T>
inline ServerSocketBase<T>::ServerSocketBase(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::tr1::function<void(T*)> &accept) : m_accept(accept){
  m_listener.reset(new Thread(std::tr1::bind(&ServerSocketBase::listenerThreadFun, this, channel, semantics, deallocator)));
}

template <typename T>
inline ServerSocketBase<T>::~ServerSocketBase() noexcept(false) {
  m_listener->cancel();
  m_listener->join();
}

template <typename T>
void ServerSocketBase<T>::send(SendArgs &args){
  std::tr1::shared_ptr<T> peer;

  if(args.peerId){
    typename IdToPeerMap::iterator elem;

    if((elem = m_idToPeer.find(*args.peerId)) != m_idToPeer.end()){
      peer = *elem->second;
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

template <typename T>
void ServerSocketBase<T>::openConnection(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *), const UUID &uuid){
  Channel peerChannel(channel.name + "_" + (std::string)uuid, channel.context);
  std::tr1::shared_ptr<T> peer(new T(peerChannel, semantics, deallocator));

  m_lock.lock();
  typename PeerList::iterator i = m_peers.insert(m_peers.begin(), peer);
  m_rawToPeer[peer.get()] = peer;
  m_idToPeer[uuid] = i;
  m_peerToId[peer] = uuid;
  m_lock.unlock();

  m_accept(peer.get());
}

template <typename T>
void ServerSocketBase<T>::closeConnection(const UUID &uuid){
  typename PeerList::iterator i = m_idToPeer.at(uuid);
  std::tr1::shared_ptr<T> peer = *i;

  m_lock.lock();
  m_rawToPeer.erase(peer.get());
  m_peerToId.erase(peer);
  m_idToPeer.erase(uuid);
  notifyPeerDisconnection(i);
  m_peers.erase(i);
  m_lock.unlock();
}

template <typename T>
void ServerSocketBase<T>::notifyPeerDisconnection(typename PeerList::iterator it){
}

template <typename T>
void ServerSocketBase<T>::listenerThreadFun(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)){
  char opcode;
  Poller poller;
  RawPipe listener("/tmp/" + channel.name + "_announce");

  poller.add(listener.getReadFD());

  while(true){
    if(poller.poll() == -1){
      continue;
    }

    Thread::disableCancelState();
    listener.read(&opcode, sizeof(opcode));
    UUID id = UUID::readFrom(listener);

    if(opcode == 'o'){
      openConnection(channel, semantics, deallocator, id);
    }else if(opcode == 'c'){
      closeConnection(id);
    }else{
      assert(false); 
    }

    Thread::enableCancelState();
  }
}

}

#endif
