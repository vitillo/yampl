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
#include "yampl/generic/ClientSocket.h"
#include "yampl/generic/ServerSocket.h"
#include "yampl/generic/SimpleClientSocket.h"
#include "yampl/generic/SimpleServerSocket.h"

namespace yampl{
namespace pipe{

enum Mode{
  PIPE_PULL,
  PIPE_PUSH,
  PIPE_CLIENT,
  PIPE_SERVER
};

class ServerSocket;

class PipeSocketBase : public ISocket{
  friend class ServerSocket;

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

typedef SimpleClientSocket<ProducerSocket, ConsumerSocket> SimpleClientSocket;
typedef SimpleServerSocket<ProducerSocket, ConsumerSocket> SimpleServerSocket;

typedef ClientSocket<SimpleClientSocket> ClientSocket;

class ServerSocket : public yampl::ServerSocket<SimpleServerSocket>{
  public:
    ServerSocket(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : yampl::ServerSocket<SimpleServerSocket>(channel, semantics, deallocator, std::tr1::bind(&ServerSocket::accept, this, std::tr1::placeholders::_1)){}

    void accept(SimpleServerSocket *socket){
      m_peerPoll.add(socket->getConsumerSocket()->m_transferPipe->getReadFD(), socket);
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
