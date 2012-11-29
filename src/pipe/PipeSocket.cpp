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

PipeSocketBase::PipeSocketBase(const Channel &channel, Mode mode, Semantics semantics, void (*deallocator)(void *, void *)) : m_mode(mode), m_semantics(semantics), m_isRecvPending(false), m_receiveSize(0), m_receiveBuffer(NULL), m_destroy(false){
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
    m_ctlThread->join();
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
  if(!m_isRecvPending){
    m_transferPipe->read(&m_receiveSize, sizeof(m_receiveSize));
    m_isRecvPending = true;
  }

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

  m_isRecvPending = false;
  return bytesRead;
}

}
}

