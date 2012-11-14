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

void PipeSocketBase::send(void *buffer, size_t size, discriminator_t *discriminator, void *hint){
  size_t bytesWritten = 0;
  struct iovec vec;

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

  m_lock.lock();
  m_pendingBuffers.push_back(make_pair(buffer, hint));
  m_lock.unlock();

  for(bytesWritten = 0; bytesWritten != size;){
    ssize_t sent = 0;
    vec.iov_base = ((char *) buffer) + bytesWritten;
    vec.iov_len = size - bytesWritten;

    if((sent = vmsplice(m_transferPipe->write, &vec, 1, 0)) == -1)
      throw ErrnoException("Send failed");

    bytesWritten += sent;
  }
}

ssize_t PipeSocketBase::recv(void **buffer, size_t size, discriminator_t *discriminator){
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

MOClientSocket::MOClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_pid(getpid()), m_server(0), m_private(0){
  Channel announce(channel.name + "_announce");
  Channel priv(channel.name + "_" + to_string(getpid()));

  m_private = new ClientSocket(priv, semantics, deallocator);
  m_server = new ProducerSocket(announce, MOVE_DATA, voidDeallocator);
  m_server->send(&m_pid, sizeof(m_pid));
}

MOClientSocket::~MOClientSocket(){
  delete m_server;
  delete m_private;
}

void MOServerSocket::listenerThreadFun(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)){
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
    tr1::shared_ptr<ServerSocket> peer(new ServerSocket(peerChannel, semantics, deallocator));
    m_peers.push_back(peer);

    m_peerPoll.add(((ConsumerSocket *)(peer->getConsumerSocket()))->m_transferPipe->read, peer.get());
  }
}

MOServerSocket::MOServerSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_currentPeer(0), m_destroy(false){
  m_listener.reset(new Thread(tr1::bind(&MOServerSocket::listenerThreadFun, this, channel, semantics, deallocator)));
}

MOServerSocket::~MOServerSocket(){
  m_destroy = true;
  m_listener->join();
}

void MOServerSocket::send(void *buffer, size_t size, discriminator_t *discriminator, void *hint){
  if(!m_currentPeer)
    throw InvalidOperationException();

  m_currentPeer->send(buffer, size, discriminator, hint);
  m_currentPeer = 0;
}

ssize_t MOServerSocket::recv(void **buffer, size_t size, discriminator_t *discriminator){
  return try_recv(buffer, size, discriminator, -1);
}

ssize_t MOServerSocket::try_recv(void **buffer, size_t size, discriminator_t *discriminator, long timeout){
  if(m_currentPeer)
    throw InvalidOperationException();

  if(!m_peerPoll.poll((void **)&m_currentPeer, timeout)){
    return -1;
  }else{
    return m_currentPeer->recv(buffer, size, discriminator);
  }
}

}
}

