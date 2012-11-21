#include "yampl/Exceptions.h"
#include "yampl/shm/SHMSocket.h"

using namespace std;

namespace yampl{
namespace shm{

PipeSocketBase::PipeSocketBase(const Channel &channel, Semantics semantics, void (*deallocator)(void *, void *)) : m_size(1048576), m_name("/" + channel.name), m_semantics(semantics), m_deallocator(deallocator), m_receiveSize(0), m_receiveBuffer(NULL){
}

PipeSocketBase::~PipeSocketBase(){
  free(m_receiveBuffer);
}

void PipeSocketBase::openSocket(bool create){
  if(!m_memory){
    m_memory.reset(new SharedMemory(m_name.c_str(), m_size));
    m_queue.reset(new RingBuffer(m_size, m_memory->getMemory()));
  }
}

void PipeSocketBase::send(SendArgs &args){
  size_t bytesWritten = 0;

  // write message size
  while(m_queue->emptySize() < sizeof(args.size));
  m_queue->enqueue(&args.size, sizeof(args.size), false);

  // write message in chunks
  while(bytesWritten != args.size){
    while(m_queue->emptySize() < sizeof(size_t));

    size_t availSize = m_queue->emptySize();
    size_t chunkSize = min(availSize - sizeof(args.size), args.size - bytesWritten);

    if(chunkSize == 0)
      continue;

    m_queue->enqueue(&chunkSize, sizeof(chunkSize), true);
    m_queue->enqueue((char *)args.buffer + bytesWritten, chunkSize, false);

    bytesWritten += chunkSize;
  }

  if(m_semantics == MOVE_DATA)
    m_deallocator(args.buffer, args.hint);
}

ssize_t PipeSocketBase::recv(RecvArgs &args){
  size_t bytesRead = 0;

  while(m_queue->dequeue(&m_receiveSize, sizeof(m_receiveSize)) == 0);

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

  while(bytesRead != m_receiveSize){
    size_t chunkSize = 0;

    while(m_queue->dequeue(&chunkSize, sizeof(chunkSize)) == 0);
    m_queue->dequeue((char *)*args.buffer + bytesRead, chunkSize);

    bytesRead += chunkSize;
  }

  return args.size;
}

}
}

