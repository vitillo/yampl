#ifndef PIPESOCKETFACTORY_H
#define PIPESOCKETFACTORY_H

#include "../SocketFactory.h"

namespace IPC{
namespace pipe{

class SocketFactory : public ISocketFactory{
  public:
    SocketFactory(bool zerocopy = true) : m_zerocopy(zerocopy){}

    virtual ISocket *createProducerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createConsumerSocket(Channel channel, bool ownership = true);
    virtual ISocket *createClientSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);
    virtual ISocket *createServerSocket(Channel channel, bool ownership = true, void (*deallocator)(void *, void *) = defaultDeallocator);

  private:
    bool m_zerocopy;
};

}
}

#endif
