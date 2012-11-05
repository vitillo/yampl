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

#include "utils/utils.h"
#include "Channel.h"
#include "pipe/PipeSocket.h"

using namespace std;

namespace YAMPL{
namespace pipe{

RawPipe::RawPipe(const std::string &name) : m_name(name), m_doUnlink(false){
  if(mkfifo(name.c_str(), S_IRWXU) == -1 && errno != EEXIST)
    throw ErrnoException("Failed to create FIFO");

  if(errno != EEXIST)
    m_doUnlink = true;

  // Deadlock prevention, open FIFO in RD and WR mode (O_RDWR unspecified for FIFO)
  if((read = open(name.c_str(), O_RDONLY | O_NONBLOCK)) == -1)
    throw ErrnoException("Failed to open FIFO for reading");
  if((write = open(name.c_str(), O_WRONLY)) == -1){
    close(read);
    throw ErrnoException("Failed to open FIFO for writing");
  }

  if((fcntl(read, F_SETFL, fcntl(read, F_GETFL) & ~O_NONBLOCK)) == -1){
    close(read);
    close(write);
    throw ErrnoException("Failed to update FIFO");
  }
}

RawPipe::~RawPipe(){
  close(read);
  close(write);

  if(m_doUnlink)
    unlink(m_name.c_str());
}

void PipeSocketBase::ctlThreadFun(void (*deallocator)(void *, void *)){
  Poller poller;
  poller.add(m_ctlPipe->read);

  while(!m_destroy){
    if(poller.poll() == -1)
      continue;

    char signal = 0;

    if(TEMP_FAILURE_RETRY(read(m_ctlPipe->read, &signal, 1)) == -1)
      throw ErrnoException("Failed to receive ctl signal");

    m_lock.lock();
    std::pair<void *, void *> entry = m_pendingBuffers.front();
    m_pendingBuffers.pop_front();
    m_lock.unlock();

    deallocator((void *)entry.first, (void *)entry.second);
  }
}

PipeSocketBase::PipeSocketBase(const Channel &channel, Mode mode, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *)) : m_mode(mode), m_semantics(semantics), m_fast(fastTransfer), m_receiveSize(0), m_receiveBuffer(NULL), m_destroy(false){
  const string& transferPipeName = "/tmp/fifo_" + channel.name;
  const string& ctlPipeName = transferPipeName + "_ctl";
  
  m_transferPipe.reset(new RawPipe(transferPipeName));
  m_ctlPipe.reset(new RawPipe(ctlPipeName));

  if(m_mode == PIPE_PULL){
#ifdef F_SETPIPE_SZ
    //feature added in Linux 2.6.35
    if(fcntl(m_transferPipe->write, F_SETPIPE_SZ, 1048576) == -1)
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
    m_ctlThread->join();
  }

  free(m_receiveBuffer);
}

void PipeSocketBase::send(void *buffer, size_t size, void *hint){
  size_t bytesWritten = 0;
  struct iovec vec;

  m_lock.lock();
  m_pendingBuffers.push_back(make_pair(buffer, hint));
  m_lock.unlock();

  while(bytesWritten != sizeof(size)){
    ssize_t sent = TEMP_FAILURE_RETRY(write(m_transferPipe->write, &size, sizeof(size)));

    if(sent == -1)
      throw ErrnoException("Send failed");

    bytesWritten += sent;
  }

  if(m_semantics == COPY_DATA){
    void *tmp = NULL;

    if((tmp = malloc(size)) == NULL)
      throw ErrnoException("Send failed");

    buffer = memcpy(tmp, buffer, size);
  }

  for(bytesWritten = 0; bytesWritten != size;){
    ssize_t sent = 0;
    vec.iov_base = ((char *) buffer) + bytesWritten;
    vec.iov_len = size - bytesWritten;

    if(m_fast){
      if((sent = vmsplice(m_transferPipe->write, &vec, 1, 0)) == -1)
        throw ErrnoException("Send failed");
    }else{
      if((sent = TEMP_FAILURE_RETRY(write(m_transferPipe->write, (char *) buffer + bytesWritten, size - bytesWritten))) == -1)
	throw ErrnoException("Send failed");
    }

    bytesWritten += sent;
  }
}

size_t PipeSocketBase::recv(void **buffer, size_t size){
  size_t bytesRead = 0;

  while(bytesRead != sizeof(m_receiveSize)){
    ssize_t r = TEMP_FAILURE_RETRY(read(m_transferPipe->read, &m_receiveSize, sizeof(m_receiveSize)));

    if(r == -1)
      throw ErrnoException("Receive failed");

    bytesRead += r;
  }

  if(m_semantics == MOVE_DATA){
    if((m_receiveBuffer = realloc(m_receiveBuffer, m_receiveSize)) == 0)
      throw ErrnoException("Receive failed");

    *buffer = m_receiveBuffer;
  }else{
    if(*buffer && size < m_receiveSize)
      throw InvalidSizeException();
    else if(!*buffer)
      *buffer = malloc(m_receiveSize);
  }

  for(bytesRead = 0; bytesRead != m_receiveSize;){
    ssize_t r = TEMP_FAILURE_RETRY(read(m_transferPipe->read, (char *)*buffer + bytesRead, m_receiveSize - bytesRead));

    if(r == -1)
      throw ErrnoException("Receive failed");

    bytesRead += r;
  }

  if(write(m_ctlPipe->write, " ", 1) == -1)
    throw ErrnoException("Receive failed");

  m_receiveSize = 0;
  return bytesRead;
}

ServiceSocketBase::ServiceSocketBase(const Channel &channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *), Mode mode) : m_reqSocket(0), m_repSocket(0), m_mode(mode){
  if(channel.topology != ONE_TO_ONE)
    throw UnsupportedException();

  Channel req = channel, rep = channel;
  req.name = req.name + "_req";
  rep.name = rep.name + "_rep";

  if(mode == PIPE_CLIENT){
    m_reqSocket = new ProducerSocket(req, semantics, fastTransfer, deallocator);
    m_repSocket = new ConsumerSocket(rep, semantics); 
    m_receiveCompleted = true;
  }else if(mode == PIPE_SERVER){
    m_reqSocket = new ConsumerSocket(req, semantics); 
    m_repSocket = new ProducerSocket(rep, semantics, fastTransfer, deallocator);
    m_receiveCompleted = false;
  }else
    throw InvalidOperationException();
}

ServiceSocketBase::~ServiceSocketBase(){
  delete m_reqSocket;
  delete m_repSocket;
}

void ServiceSocketBase::send(void *buffer, size_t size, void *hint){
  if(m_receiveCompleted){
    (m_mode == PIPE_CLIENT ? m_reqSocket : m_repSocket)->send(buffer, size, hint);
    m_receiveCompleted = false;
  }else
    throw UnsupportedException();
}

size_t ServiceSocketBase::recv(void **buffer, size_t size){
  if(m_receiveCompleted)
    throw UnsupportedException();
  else{
    size_t ret = (m_mode == PIPE_CLIENT ? m_repSocket : m_reqSocket)->recv(buffer, size);
    m_receiveCompleted = true;
    return ret;
  }
}

MOClientSocket::MOClientSocket(const Channel& channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *)) : m_pid(getpid()), m_server(0), m_private(0){
  Channel announce(channel.name + "_announce");
  Channel priv(channel.name + "_" + to_string(getpid()));

  m_private = new ClientSocket(priv, semantics, fastTransfer, deallocator);
  m_server = new ProducerSocket(announce, MOVE_DATA, fastTransfer, voidDeallocator);
  m_server->send(&m_pid, sizeof(m_pid));
}

MOClientSocket::~MOClientSocket(){
  delete m_server;
  delete m_private;
}

void MOServerSocket::listenerThreadFun(const Channel &channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *)){
  Poller poller;
  Channel announce(channel.name + "_announce");
  tr1::shared_ptr<ConsumerSocket> listener(new ConsumerSocket(announce, COPY_DATA));

  poller.add(listener->m_transferPipe->read);

  while(!m_destroy){
    if(poller.poll() == -1)
      continue;

    pid_t pid, *pidptr = &pid;
    listener->recv((void **)&pidptr, sizeof(pid));

    Channel peerChannel(channel.name + "_" + to_string(pid));
    tr1::shared_ptr<ServerSocket> peer(new ServerSocket(peerChannel, semantics, fastTransfer, deallocator));
    m_peers.push_back(peer);

    m_peerPoll.add(((ConsumerSocket *)(peer->m_reqSocket))->m_transferPipe->read, peer.get());
  }
}

MOServerSocket::MOServerSocket(const Channel& channel, Semantics semantics, bool fastTransfer, void (*deallocator)(void *, void *)) : m_currentPeer(0), m_destroy(false){
  m_listener.reset(new Thread(tr1::bind(&MOServerSocket::listenerThreadFun, this, channel, semantics, fastTransfer, deallocator)));
}

MOServerSocket::~MOServerSocket(){
  m_destroy = true;
  m_listener->join();
}

void MOServerSocket::send(void *buffer, size_t size, void *hint){
  if(!m_currentPeer)
    throw InvalidOperationException();

  m_currentPeer->send(buffer, size, hint);
  m_currentPeer = 0;
}

size_t MOServerSocket::recv(void **buffer, size_t size){
  if(m_currentPeer)
    throw InvalidOperationException();

  m_peerPoll.poll((void **)&m_currentPeer, -1);
  return m_currentPeer->recv(buffer, size);
}

}
}

