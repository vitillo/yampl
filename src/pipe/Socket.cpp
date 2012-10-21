#define _GNU_SOURCE 1

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <string>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <algorithm>

#include "Channel.h"
#include "pipe/Socket.h"

using namespace std;

namespace IPC{
namespace pipe{
    
PipeSocketBase::Pipe PipeSocketBase::openPipe(const std::string& name){
  Pipe pipe;

  if(mkfifo(name.c_str(), S_IRWXU) == -1 && errno != EEXIST)
    throw ErrnoException("Can't create FIFO");

  // Deadlock prevention, open FIFO in RD and WR mode (O_RDWR unspecified for FIFO)
  if((pipe.read = open(name.c_str(), O_RDONLY | O_NONBLOCK)) == -1)
    throw ErrnoException("Failed to open FIFO");
  if((pipe.write = open(name.c_str(), O_WRONLY)) == -1)
    throw ErrnoException("Failed to open FIFO");

  if((fcntl(pipe.read, F_SETFL, fcntl(pipe.read, F_GETFL) & ~O_NONBLOCK)) == -1)
    throw ErrnoException("Failed to update FIFO");

  return pipe;
}

PipeSocketBase::PipeSocketBase(const Channel &channel, Mode mode, bool isOwner, bool fastTransfer, void (*deallocator)(void *, void *)) : m_mode(mode), m_isOwner(isOwner), m_fast(fastTransfer), m_receiveBuffer(0){
  const string& transferPipeName = "/tmp/fifo_" + channel.name;
  const string& ctlPipeName = transferPipeName + "_ctl";
  
  m_transferPipe = openPipe(transferPipeName);
  m_ctlPipe = openPipe(ctlPipeName);

  if(m_mode == PIPE_PULL){
    #ifdef F_SETPIPE_SZ

    //feature added in Linux 2.6.35
    if(fcntl(m_transferPipe.write, F_SETPIPE_SZ, 1048576) == -1)
      throw ErrnoException("Failed to set the pipe buffer size");

    #endif
  }else if(m_mode == PIPE_PUSH){
    m_ctlThread = new std::thread([this, deallocator] {
      int signalPoll;
      if((signalPoll = epoll_create(1)) == -1)
	throw ErrnoException("Failed to create epoll handle");

      epoll_event signalEv;
      signalEv.events = EPOLLIN;
      
      if(epoll_ctl(signalPoll, EPOLL_CTL_ADD, m_ctlPipe.read, &signalEv) == -1)
	throw ErrnoException("Failed to add fd to epoll handle");

      while(!m_destroy){
	const void *buffer = 0;
	int signal = 0;

	switch(epoll_wait(signalPoll, &signalEv, 1, 500)){
	  case 0:
	    continue;
	  case -1:
	    throw ErrnoException("Failed to wait on epoll handle");
	  default:
	    break;
	}

	if(TEMP_FAILURE_RETRY(read(m_ctlPipe.read, &signal, 1)) == -1)
	  throw ErrnoException("Failed to receive ctl signal");

	m_lock.lock();
	buffer = m_pendingBuffers.front();
	m_pendingBuffers.pop_front();
	m_lock.unlock();

	deallocator((void *)buffer, NULL);
      }

    close(signalPoll);
  });
  }else
    throw InvalidOperationException();
}

PipeSocketBase::~PipeSocketBase(){
  if(m_mode == PIPE_PUSH){
    m_destroy = true;
    m_ctlThread->join();
  }

  close(m_transferPipe.read);
  close(m_transferPipe.write);
  free(m_receiveBuffer);
}

void PipeSocketBase::send(const void *buffer, size_t size, void *hint){
  size_t bytesWritten = 0;
  struct iovec vec;

  m_lock.lock();
  m_pendingBuffers.push_back(buffer);
  m_lock.unlock();

  while(bytesWritten != sizeof(size)){
    ssize_t sent = TEMP_FAILURE_RETRY(write(m_transferPipe.write, &size, sizeof(size)));

    if(sent == -1)
      throw ErrnoException("Send failed");

    bytesWritten += sent;
  }

  if(!m_isOwner){
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
      if((sent = vmsplice(m_transferPipe.write, &vec, 1, 0)) == -1)
        throw ErrnoException("Send failed");
    }else{
      if((sent = TEMP_FAILURE_RETRY(write(m_transferPipe.write, (char *) buffer + bytesWritten, size - bytesWritten))) == -1)
	throw ErrnoException("Send failed");
    }

    bytesWritten += sent;
  }
}

size_t PipeSocketBase::receive(void **buffer, size_t size){
  static size_t msg_size = 0;
  size_t bytesRead = 0;


  while(bytesRead != sizeof(msg_size)){
    ssize_t r = TEMP_FAILURE_RETRY(read(m_transferPipe.read, &msg_size, sizeof(msg_size)));

    if(r == -1)
      throw ErrnoException("Receive failed");

    bytesRead += r;
  }

  if(m_isOwner){
    if((m_receiveBuffer = realloc(m_receiveBuffer, msg_size)) == 0)
      throw ErrnoException("Receive failed");

    *buffer = m_receiveBuffer;
  }else{
    if(*buffer && size < msg_size)
      throw InvalidSizeException();
    else if(!*buffer)
      *buffer = malloc(msg_size);
  }

  for(bytesRead = 0; bytesRead != msg_size;){
    ssize_t r = TEMP_FAILURE_RETRY(read(m_transferPipe.read, (char *)*buffer + bytesRead, msg_size - bytesRead));

    if(r == -1)
      throw ErrnoException("Receive failed");

    bytesRead += r;
  }

  if(write(m_ctlPipe.write, " ", 1) == -1)
    throw ErrnoException("Receive failed");

  msg_size = 0;
  return bytesRead;
}

ServiceSocketBase::ServiceSocketBase(const Channel &channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *), Mode mode) : m_mode(mode){
  if(channel.topology != ONE_TO_ONE)
    throw UnsupportedException();

  Channel req = channel, rep = channel;
  req.name = req.name + "_req";
  rep.name = rep.name + "_rep";

  if(mode == PIPE_CLIENT){
    m_reqSocket = new ProducerSocket(req, ownership, fastTransfer, deallocator);
    m_repSocket = new ConsumerSocket(rep, ownership); 
    m_receiveCompleted = true;
  }else if(mode == PIPE_SERVER){
    m_reqSocket = new ConsumerSocket(req, ownership); 
    m_repSocket = new ProducerSocket(rep, ownership, fastTransfer, deallocator);
    m_receiveCompleted = false;
  }else
    throw InvalidOperationException();
}

ServiceSocketBase::~ServiceSocketBase(){
  delete m_reqSocket;
  delete m_repSocket;
}

void ServiceSocketBase::send(const void *buffer, size_t size, void *hint){
  if(m_receiveCompleted){
    (m_mode == PIPE_CLIENT ? m_reqSocket : m_repSocket)->send(buffer, size, hint);
    m_receiveCompleted = false;
  }else
    throw UnsupportedException();
}

size_t ServiceSocketBase::receive(void **buffer, size_t size){
  if(m_receiveCompleted)
    throw UnsupportedException();
  else{
    size_t ret = (m_mode == PIPE_CLIENT ? m_repSocket : m_reqSocket)->receive(buffer, size);
    m_receiveCompleted = true;
    return ret;
  }
}

MOClientSocket::MOClientSocket(const Channel& channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)){
  Channel announce(channel.name + "_announce");
  m_private = new ClientSocket({channel.name + "_" + to_string(m_pid)}, ownership, fastTransfer, deallocator);
  m_server = new ProducerSocket(announce, true, fastTransfer, [] (void *, void *) { return; });
  m_server->send(&m_pid, sizeof(m_pid));
}

MOClientSocket::~MOClientSocket(){
  delete m_server;
  delete m_private;
}

MOServerSocket::MOServerSocket(const Channel& channel, bool ownership, bool fastTransfer, void (*deallocator)(void *, void *)){
  Channel announce(channel.name + "_announce");

  if((m_peerPoll = epoll_create(1)) == -1)
    throw ErrnoException("Failed to create epoll handle");

  m_listener = std::thread([this, announce, channel, ownership, fastTransfer, deallocator] {
    ConsumerSocket *listener = new ConsumerSocket(announce, false);
    
    int listenerPoll;
    if((listenerPoll = epoll_create(1)) == -1)
      throw ErrnoException("Failed to create epoll handle");

    epoll_event listenerEv;
    listenerEv.events = EPOLLIN;
    
    if(epoll_ctl(listenerPoll, EPOLL_CTL_ADD, listener->m_transferPipe.read, &listenerEv) == -1)
      throw ErrnoException("Failed to add fd to epoll handle");

    while(!m_destroy){
      switch(epoll_wait(listenerPoll, &listenerEv, 1, 500)){
        case 0:
	  continue;
        case -1:
	  throw ErrnoException("Failed to wait on epoll handle");
        default:
	  break;
      }

      pid_t pid, *pidptr = &pid;
      listener->receive((void **)&pidptr, sizeof(pid));

      ServerSocket *peer = new ServerSocket({channel.name + "_" + to_string(pid)}, ownership, fastTransfer, deallocator);
      m_peers.push_back(peer);

      epoll_event ev;
      ev.events = EPOLLIN;
      ev.data.ptr = peer;
      
      if(epoll_ctl(m_peerPoll, EPOLL_CTL_ADD, ((ConsumerSocket *)(peer->m_reqSocket))->m_transferPipe.read, &ev) == -1)
        throw ErrnoException("Failed to add fd to epoll handle");
    }

    close(listenerPoll);
  });
}

MOServerSocket::~MOServerSocket(){
  m_destroy = true;
  m_listener.join();

  for(auto peer : m_peers)
    delete peer;

  close(m_peerPoll);
}

void MOServerSocket::send(const void *buffer, size_t size, void *hint){
  if(!m_currentPeer)
    throw InvalidOperationException();

  m_currentPeer->send(buffer, size, hint);
  m_currentPeer = 0;
}

size_t MOServerSocket::receive(void **buffer, size_t size){
  if(m_currentPeer)
    throw InvalidOperationException();

  epoll_event event;
  if(epoll_wait(m_peerPoll, &event, 1, -1) == -1)
    throw ErrnoException();

  m_currentPeer = (ServerSocket *)event.data.ptr;
  return m_currentPeer->receive(buffer, size);
}

}
}

