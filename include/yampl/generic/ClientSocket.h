#ifndef YAMPL_MOCLIENTSOCKET_H
#define YAMPL_MOCLIENTSOCKET_H

#include "yampl/ISocket.h"
#include "yampl/Channel.h"
#include "yampl/utils/RawPipe.h"
#include "yampl/utils/UUID.h"

namespace yampl{

template <typename T>
class ClientSocket: public ISocket{
  public:
    ClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name);
    virtual ~ClientSocket();

    virtual void send(SendArgs &args);
    virtual ssize_t recv(RecvArgs &args);

  private:
    ClientSocket(const ClientSocket &);
    ClientSocket & operator=(const ClientSocket &);

    RawPipe m_announce;
    ISocket *m_private;
    UUID m_uuid;
};

template <typename T>
inline ClientSocket<T>::ClientSocket(const Channel& channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) : m_announce("/tmp/" + channel.name + "_announce"), m_private(0), m_uuid(name){
  Channel priv(channel.name + "_" + (std::string)m_uuid, channel.context);

  m_private = new T(priv, semantics, deallocator, name);
  m_uuid.writeTo(m_announce);
}

template <typename T>
inline ClientSocket<T>::~ClientSocket(){
  delete m_private;
}

template <typename T>
inline void ClientSocket<T>::send(SendArgs &args){
  m_private->send(args);
}

template <typename T>
inline ssize_t ClientSocket<T>::recv(RecvArgs &args){
  return m_private->recv(args);
}

}

#endif
