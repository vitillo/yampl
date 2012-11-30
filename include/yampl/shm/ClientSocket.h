#ifndef YAMPL_SHM_CLIENTSOCKET_H
#define YAMPL_SHM_CLIENTSOCKET_H

#include "yampl/generic/ClientSocket.h"
#include "yampl/shm/SimpleClientSocket.h"

namespace yampl{
namespace shm{

class ClientSocket : public yampl::ClientSocket<SimpleClientSocket>{
  public: 
    ClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name);

    void syncWithServer();
    virtual void send(SendArgs &args);

  private:
    std::tr1::shared_ptr<SharedMemory> m_memory;
    std::tr1::shared_ptr<Semaphore> m_semaphore;
};

inline ClientSocket::ClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : yampl::ClientSocket<SimpleClientSocket>(channel, semantics, deallocator, name){
  m_memory.reset(new SharedMemory(channel.name + "_sem", sizeof(int)));
  m_semaphore.reset(new Semaphore((int *)m_memory->getMemory()));
}

inline void ClientSocket::syncWithServer(){
  static bool sync = true;

  if(sync){
    static_cast<ISocket *>(this)->recv<char>();
    sync = false;
  }
}

inline void ClientSocket::send(SendArgs &args){
  syncWithServer();
  args.custom = m_semaphore.get();
  yampl::ClientSocket<SimpleClientSocket>::send(args);
}

}
}

#endif
