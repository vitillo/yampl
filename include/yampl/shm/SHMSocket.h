#ifndef YAMPL_SHM_SOCKET_H
#define YAMPL_SHM_SOCKET_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <tr1/memory>

#include "yampl/ISocket.h"
#include "yampl/ISocketFactory.h"
#include "yampl/Channel.h"
#include "yampl/utils/RingBuffer.h"
#include "yampl/utils/SharedMemory.h"
#include "yampl/utils/Futex.h"
#include "yampl/generic/ClientSocket.h"
#include "yampl/generic/ServerSocket.h"
#include "yampl/generic/SimpleClientSocket.h"
#include "yampl/generic/SimpleServerSocket.h"

namespace yampl{
namespace shm{

class PipeSocketBase : public ISocket{
  public:
    virtual ~PipeSocketBase();

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

  protected:
    size_t m_size;
    std::string m_name;
    std::tr1::shared_ptr<SharedMemory> m_memory;
    std::tr1::shared_ptr<RingBuffer> m_queue;

    PipeSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *));

    void openSocket(bool isProducer);

  private:
    PipeSocketBase(const PipeSocketBase &);
    PipeSocketBase & operator=(PipeSocketBase &);

    Semantics m_semantics;
    void (*m_deallocator)(void *, void *);
    bool m_isRecvPending;
    size_t m_receiveSize;
    void *m_receiveBuffer;
};

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, semantics, deallocator){
      openSocket(true);
     }

    virtual void send(SendArgs &args){
      PipeSocketBase::send(args);
    }

    virtual ssize_t recv(RecvArgs &args){
      throw InvalidOperationException();
    }

  private:
    ProducerSocket(const ProducerSocket &);
    ProducerSocket & operator=(const ProducerSocket &);
};

class ConsumerSocket : public PipeSocketBase{
  public:
    ConsumerSocket(const Channel &channel, Semantics semantics) : PipeSocketBase(channel, semantics, 0){
    }

    virtual bool hasPendingMessages(){
      openSocket(false);
      return !m_queue->empty();
    }

    virtual void send(SendArgs &args){
      throw InvalidOperationException();
    }

    virtual ssize_t recv(RecvArgs &args){
      openSocket(false);
      return PipeSocketBase::recv(args);
    }

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);
};

typedef SimpleClientSocket<ProducerSocket, ConsumerSocket> SimpleClientSocket;
typedef SimpleServerSocket<ProducerSocket, ConsumerSocket> SimpleServerSocket;

class ClientSocket : public yampl::ClientSocket<SimpleClientSocket>{
  public: 
    ClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : yampl::ClientSocket<SimpleClientSocket>(channel, semantics, deallocator, name){
      m_memory.reset(new SharedMemory(channel.name + "_sem", sizeof(int)));
      m_semaphore.reset(new Semaphore((int *)m_memory->getMemory()));
    }

    void syncWithServer(){
      static bool sync = true;

      if(sync){
	static_cast<ISocket *>(this)->recv<char>();
	sync = false;
      }
    }

    virtual void send(SendArgs &args){
      syncWithServer();
      args.custom = m_semaphore.get();
      yampl::ClientSocket<SimpleClientSocket>::send(args);
    }

  private:
    std::tr1::shared_ptr<SharedMemory> m_memory;
    std::tr1::shared_ptr<Semaphore> m_semaphore;
};

class ServerSocket : public yampl::ServerSocket<SimpleServerSocket>{
  public:
    ServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : yampl::ServerSocket<SimpleServerSocket>(channel, semantics, deallocator, std::tr1::bind(&ServerSocket::accept, this, std::tr1::placeholders::_1)), m_nextPeerToVisit(0), m_isRecvPending(false){
      m_memory.reset(new SharedMemory(channel.name + "_sem", sizeof(int)));
      m_semaphore.reset(new Semaphore((int *)m_memory->getMemory()));
    }

    void accept(SimpleServerSocket *socket){
      // Send synchronization message
      static_cast<ISocket *>(socket)->send(' ');
    }

    virtual ssize_t recv(RecvArgs &args){
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

  private:
    std::tr1::shared_ptr<SharedMemory> m_memory;
    std::tr1::shared_ptr<Semaphore> m_semaphore;
    size_t m_nextPeerToVisit;
    bool m_isRecvPending;
};

}
}

#endif
