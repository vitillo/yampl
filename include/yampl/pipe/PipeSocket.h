#ifndef YAMPL_PIPE_SOCKET_H
#define YAMPL_PIPE_SOCKET_H

#include <unistd.h>

#include <string>
#include <deque>
#include <vector>
#include <tr1/memory>
#include <tr1/functional>

#include "yampl/ISocket.h"
#include "yampl/ISocketFactory.h"
#include "yampl/Exceptions.h"
#include "yampl/utils/SpinLock.h"
#include "yampl/utils/Poller.h"
#include "yampl/utils/Thread.h"
#include "yampl/utils/RawPipe.h"
#include "yampl/generic/ServiceSocket.h"
#include "yampl/generic/MOClientSocket.h"
#include "yampl/generic/MOServerSocket.h"

namespace yampl{
namespace pipe{

enum Mode{
  PIPE_PULL,
  PIPE_PUSH,
  PIPE_CLIENT,
  PIPE_SERVER
};

class MOServerSocket;

class PipeSocketBase : public ISocket{
  friend class MOServerSocket;

  public:
    virtual ~PipeSocketBase();

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

  protected:
    PipeSocketBase(const Channel &channel, Mode type, Semantics semantics, void (*deallocator)(void *, void *));

  private:
    PipeSocketBase(const PipeSocketBase &);
    PipeSocketBase & operator=(PipeSocketBase &);

    void ctlThreadFun(void (*deallocator)(void *, void *));
 
    Mode m_mode;
    Semantics m_semantics;
    bool m_isRecvPending;
    size_t m_receiveSize;
    void *m_receiveBuffer;
    bool m_destroy;
    SpinLock m_lock;
    std::tr1::shared_ptr<Thread> m_ctlThread;
    std::tr1::shared_ptr<RawPipe> m_transferPipe;
    std::tr1::shared_ptr<RawPipe> m_ctlPipe;
    std::deque<std::pair<void *, void *> > m_pendingBuffers;
};

class ProducerSocket : public PipeSocketBase{
  public:
    ProducerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : PipeSocketBase(channel, PIPE_PUSH, semantics, deallocator){}

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
    ConsumerSocket(const Channel &channel, Semantics semantics) : PipeSocketBase(channel, PIPE_PULL, semantics, 0){}

    virtual void send(SendArgs &args){
      throw InvalidOperationException();
    }

    virtual ssize_t recv(RecvArgs &args){
      return PipeSocketBase::recv(args);
    }

  private:
    ConsumerSocket(const ConsumerSocket &);
    ConsumerSocket & operator=(const ConsumerSocket &);

};

typedef ClientSocket<ProducerSocket, ConsumerSocket> ClientSocket;
typedef ServerSocket<ProducerSocket, ConsumerSocket> ServerSocket;

typedef MOClientSocket<ClientSocket> MOClientSocket;

class MOServerSocket : public yampl::MOServerSocket<ServerSocket>{
  public:
    MOServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : yampl::MOServerSocket<ServerSocket>(channel, semantics, deallocator){}

    virtual void listenTo(std::tr1::shared_ptr<ServerSocket> socket){
      m_peerPoll.add(socket->getConsumerSocket()->m_transferPipe->getReadFD(), socket.get());
    }

    virtual ssize_t recv(RecvArgs &args){
       if(!m_peerPoll.poll((void **)&m_currentPeer, args.timeout)){
	return -1;
      }else{
	if(args.peerIdOut){
	  *args.peerIdOut = m_peerToId[m_currentPeer];
	}

	return m_currentPeer->recv(args);
      }
    }

  private:
    Poller m_peerPoll;
};

}
}

#endif
