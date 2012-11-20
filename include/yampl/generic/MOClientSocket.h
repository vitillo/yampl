#ifndef YAMPL_MOCLIENTSOCKET_H
#define YAMPL_MOCLIENTSOCKET_H

#include <unistd.h>

#include "yampl/ISocket.h"
#include "yampl/Channel.h"
#include "yampl/utils/RawPipe.h"
#include "yampl/utils/utils.h"

namespace yampl{

template <typename T>
class MOClientSocket: public ISocket{
  public:
    MOClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : m_pid(getpid()), m_announce("/tmp/" + channel.name + "_announce"), m_private(0){
      Channel priv(channel.name + "_" + to_string(getpid()));

      m_private = new T(priv, semantics, deallocator, name);
      m_announce.write(&m_pid, sizeof(m_pid));
    }

    virtual ~MOClientSocket(){
      delete m_private;
    }

    virtual void send(SendArgs &args){
      m_private->send(args);
    }

    virtual ssize_t recv(RecvArgs &args){
      return m_private->recv(args);
    }

  private:
    MOClientSocket(const MOClientSocket &);
    MOClientSocket & operator=(const MOClientSocket &);

    pid_t m_pid;
    RawPipe m_announce;
    ISocket *m_private;
};


}

#endif
