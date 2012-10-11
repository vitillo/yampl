#ifndef PIPESOCKETFACTORY_H
#define PIPESOCKETFACTORY_H

#include "SocketFactory.h"

namespace IPC{

class PipeSocketFactory : public ISocketFactory{
  public:
    PipeSocketFactory(bool zerocopy = true) : m_zerocopy(zerocopy){}

    virtual ISocket *createProducerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createConsumerSocket(Channel channel, bool ownership = true);
    virtual ISocket *createClientSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createServerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    bool m_zerocopy;
};

}

#endif
