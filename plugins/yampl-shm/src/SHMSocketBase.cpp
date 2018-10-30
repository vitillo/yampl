#include <cmath>

#include "yampl/Exceptions.h"
#include "PipeSocketBase.h"

using namespace std;

namespace yampl{
namespace shm{

PipeSocketBase::PipeSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_size(1048576), m_name("/" + channel.name), m_semantics(semantics), m_deallocator(deallocator), m_isRecvPending(false), m_receiveSize(0), m_receiveBuffer(NULL){
}

PipeSocketBase::~PipeSocketBase(){
  free(m_receiveBuffer);
}

void PipeSocketBase::openSocket(bool isProducer){
  if(!m_memory){
    m_memory.reset(new SharedMemory(m_name.c_str(), m_size));
    m_queue.reset(new RingBuffer(m_size, m_memory->getMemory(), isProducer));
  }
}

void PipeSocketBase::send(SendArgs &args){
  size_t chunkSize = min(m_queue->size() - 2*sizeof(size_t), args.size);
  size_t bytesWritten = 0;

  // write message size
  m_queue->enqueue(&args.size, sizeof(args.size));
  
  // MOServerSocket needs to be notified after the first message part in order to avoid a deadlock
  if(args.custom){
    static_cast<Semaphore *>(args.custom)->up(1);
  }

  // write message in chunks
  while(bytesWritten != args.size){
    m_queue->enqueue(&chunkSize, sizeof(chunkSize));
    m_queue->enqueue((char *)args.buffer + bytesWritten, chunkSize);

    bytesWritten += chunkSize;
    chunkSize = min(m_queue->size() - sizeof(chunkSize), args.size - bytesWritten);
  }

  if(m_semantics == MOVE_DATA && !args.overrideSemantics)
    m_deallocator(args.buffer, args.hint);
}

ssize_t PipeSocketBase::recv(RecvArgs &args){
  size_t bytesRead = 0;

  // read message size
  if(!m_isRecvPending){
    m_queue->dequeue(&m_receiveSize, sizeof(m_receiveSize));
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

  // read message in chunks
  while(bytesRead != m_receiveSize){
    size_t chunkSize = 0;

    m_queue->dequeue(&chunkSize, sizeof(chunkSize));
    m_queue->dequeue((char *)*args.buffer + bytesRead, chunkSize);

    bytesRead += chunkSize;
  }

  m_isRecvPending = false;
  return bytesRead;
}

}
}

