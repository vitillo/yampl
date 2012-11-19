#define _GNU_SOURCE 1

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <tr1/functional>

#include "yampl/utils/utils.h"
#include "yampl/Channel.h"
#include "yampl/pipe/PipeSocket.h"

using namespace std;

namespace yampl{
namespace pipe{

void PipeSocketBase::ctlThreadFun(void (*deallocator)(void *, void *)){
  Poller poller;
  poller.add(m_ctlPipe->getReadFD());

  while(!m_destroy){
    if(poller.poll() == -1){
      continue;
    }

    char signal = 0;
    m_ctlPipe->read(&signal, 1);

    m_lock.lock();
    std::pair<void *, void *> entry = m_pendingBuffers.front();
    m_pendingBuffers.pop_front();
    m_lock.unlock();

    if(m_semantics == MOVE_DATA)
      deallocator((void *)entry.first, (void *)entry.second);
    else
      free(entry.first);
  }
}

PipeSocketBase::PipeSocketBase(const Channel &channel, Mode mode, Semantics semantics, void (*deallocator)(void *, void *)) : m_mode(mode), m_semantics(semantics), m_receiveSize(0), m_receiveBuffer(NULL), m_destroy(false){
  const string& transferPipeName = "/tmp/fifo_" + channel.name;
  const string& ctlPipeName = transferPipeName + "_ctl";
  
  m_transferPipe.reset(new RawPipe(transferPipeName));
  m_ctlPipe.reset(new RawPipe(ctlPipeName));

  if(m_mode == PIPE_PULL){
#ifdef F_SETPIPE_SZ
    //feature added in Linux 2.6.35
    if(fcntl(m_transferPipe->getWriteFD(), F_SETPIPE_SZ, 1048576) == -1)
      throw ErrnoException("Failed to set the pipe's buffer size");
#endif
  }else if(m_mode == PIPE_PUSH)
    m_ctlThread.reset(new Thread(tr1::bind(&PipeSocketBase::ctlThreadFun, this, deallocator)));
  else
    throw InvalidOperationException();
}

PipeSocketBase::~PipeSocketBase(){
  if(m_mode == PIPE_PUSH){
    m_destroy = true;
    m_ctlThread->cancel();
  }

  free(m_receiveBuffer);
}

void PipeSocketBase::send(SendArgs &args){
  void *buffer = args.buffer;
  
  m_transferPipe->write(&args.size, sizeof(args.size));

  if(m_semantics == COPY_DATA){
    void *tmp = NULL;

    if((tmp = malloc(args.size)) == NULL)
      throw ErrnoException("Send failed");

    buffer = memcpy(tmp, args.buffer, args.size);
  }

  m_lock.lock();
  m_pendingBuffers.push_back(make_pair(buffer, args.hint));
  m_lock.unlock();

  m_transferPipe->writeSplice(buffer, args.size);
}

ssize_t PipeSocketBase::recv(RecvArgs &args){
  m_transferPipe->read(&m_receiveSize, sizeof(m_receiveSize));

  if(m_semantics == MOVE_DATA){
    if((m_receiveBuffer = realloc(m_receiveBuffer, m_receiveSize)) == 0)
      throw ErrnoException("Receive failed");

    *args.buffer = m_receiveBuffer;
  }else{
    if(args.allocate){
      *args.buffer = malloc(m_receiveSize);
    }else if(args.size < m_receiveSize){
      throw InvalidSizeException();
    }
  }

  size_t bytesRead = m_transferPipe->read(*args.buffer, m_receiveSize);
  m_ctlPipe->write(" ", 1);

  return bytesRead;
}

MOClientSocket::MOClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : m_pid(getpid()), m_announce(channel.name + "_announce"), m_private(0){
  Channel priv(channel.name + "_" + to_string(getpid()));

  m_private = new ClientSocket(priv, semantics, deallocator, name);
  m_announce.write(&m_pid, sizeof(m_pid));
}

MOClientSocket::~MOClientSocket(){
  delete m_private;
}

void MOServerSocket::listenerThreadFun(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)){
  RawPipe listener(channel.name + "_announce");
  Poller poller;

  poller.add(listener.getReadFD());

  while(!m_destroy){
    if(poller.poll() == -1)
      continue;

    pid_t pid;
    listener.read(&pid, sizeof(pid));

    Channel peerChannel(channel.name + "_" + to_string(pid));
    tr1::shared_ptr<ServerSocket> peer(new ServerSocket(peerChannel, semantics, deallocator));
    m_peers.push_back(peer);

    m_peerPoll.add(((ConsumerSocket *)(peer->getConsumerSocket()))->m_transferPipe->getReadFD(), peer.get());
  }
}

MOServerSocket::MOServerSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_currentPeer(0), m_destroy(false){
  m_listener.reset(new Thread(tr1::bind(&MOServerSocket::listenerThreadFun, this, channel, semantics, deallocator)));
}

MOServerSocket::~MOServerSocket(){
  m_destroy = true;
  m_listener->cancel();
}

void MOServerSocket::send(SendArgs &args){
  if(!m_currentPeer)
    throw UnroutableException();

  m_currentPeer->send(args);
  m_currentPeer = 0;
}

ssize_t MOServerSocket::recv(RecvArgs &args){
  if(!m_peerPoll.poll((void **)&m_currentPeer, args.timeout)){
    return -1;
  }else{
    return m_currentPeer->recv(args);
  }
}

}
}

