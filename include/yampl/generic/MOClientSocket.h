#ifndef YAMPL_MOCLIENTSOCKET_H
#define YAMPL_MOCLIENTSOCKET_H

#include "yampl/ISocket.h"
#include "yampl/Channel.h"
#include "yampl/utils/RawPipe.h"
#include "yampl/utils/UUID.h"

namespace yampl{

template <typename T>
class MOClientSocket: public ISocket{
  public:
    MOClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : m_announce("/tmp/" + channel.name + "_announce"), m_private(0), m_uuid(name){
      Channel priv(channel.name + "_" + (std::string)m_uuid);

      m_private = new T(priv, semantics, deallocator, name);
      m_uuid.writeTo(m_announce);
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

    RawPipe m_announce;
    ISocket *m_private;
    UUID m_uuid;
};


}

#endif
