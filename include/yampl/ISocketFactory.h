#ifndef YAMPL_ISOCKETFACTORY_H
#define YAMPL_ISOCKETFACTORY_H

#include "yampl/Channel.h"
#include "yampl/ISocket.h"
#include "yampl/plugin/IObject.hpp"

#include <stdlib.h>

namespace yampl{

inline void defaultDeallocator(void *buffer, void *){
  free(buffer);
}

enum Semantics{
  COPY_DATA = 0,
  MOVE_DATA
};

class ISocketFactory : public plugin::IObject
{
    public:
        ISocketFactory() {}
        ~ISocketFactory() override { }

    /**
     * Creates a ClientSocket. A ClientSocket can be connected to at most a 
     * single ServerSocket through a Channel.
     *
     * @param channel communication channel
     * @param semantics if equals COPY_DATA the sent messages are copied and the 
     *        sender retains ownership; if equals MOVE_DATA the implementantion 
     *        acquires ownership
     * @param deallocator the deallocation policy to invoke when the implementation 
     *        has to free the memory of a message it owns
     * @param name the identifier name of the Socket
     * @return a ClientSocket
     */
    virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator, const std::string& name = DEFAULT_ID) = 0;

    ISocket *createClientSocket(Channel channel, const std::string& name){
      return createClientSocket(channel, COPY_DATA, defaultDeallocator, name);
    }

    ISocket *createClientSocket(Channel channel, Semantics semantics, const std::string& name){
      return createClientSocket(channel, semantics, defaultDeallocator, name);
    }

    /**
     * Creates a ServerSocket. A ServerSocket can be connected to zero ore more 
     * ClientSockets through a channel.
     *
     * @param channel communication channel
     * @param semantics if equals COPY_DATA the sent messages are copied and the 
     *        sender retains ownership; if equals MOVE_DATA the implementantion 
     *        acquires ownership
     * @param deallocator the deallocation policy to invoke when the implementation 
     *        has to free the memory of a message it owns
     * @return a ServerSocket
     */
    virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator) = 0;

  private:
    ISocketFactory & operator=(const ISocketFactory &);
};

}

#endif
