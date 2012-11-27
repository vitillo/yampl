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
#include "yampl/generic/ServiceSocket.h"
#include "yampl/generic/MOClientSocket.h"
#include "yampl/generic/MOServerSocket.h"

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

typedef ClientSocket<ProducerSocket, ConsumerSocket> ClientSocket;
typedef ServerSocket<ProducerSocket, ConsumerSocket> ServerSocket;

class MOClientSocket : public yampl::MOClientSocket<ClientSocket>{
  public: 
    MOClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : yampl::MOClientSocket<ClientSocket>(channel, semantics, deallocator, name){
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
      yampl::MOClientSocket<ClientSocket>::send(args);
    }

  private:
    std::tr1::shared_ptr<SharedMemory> m_memory;
    std::tr1::shared_ptr<Semaphore> m_semaphore;
};

class MOServerSocket : public yampl::MOServerSocket<ServerSocket>{
  public:
    MOServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : yampl::MOServerSocket<ServerSocket>(channel, semantics, deallocator), m_nextPeerToVisit(0), m_isRecvPending(false){
      m_memory.reset(new SharedMemory(channel.name + "_sem", sizeof(int)));
      m_semaphore.reset(new Semaphore((int *)m_memory->getMemory()));
    }

    virtual void listenTo(std::tr1::shared_ptr<ServerSocket> socket){
      // Send synchronization message
      static_cast<ISocket *>(socket.get())->send(' ');
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
	std::tr1::shared_ptr<ServerSocket> peer = m_peers[i];
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
