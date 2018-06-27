#ifndef YAMPL_PIPE_SOCKETFACTORY_H
#define YAMPL_PIPE_SOCKETFACTORY_H

#include "yampl/ISocketFactory.h"

namespace yampl
{
    namespace pipe
    {
	opaque_ptr HOOK_CreateObject(object_init_params*);
        hook_exec_status HOOK_DestroyObject(opaque_ptr);

        class SocketFactory : public ISocketFactory
	{
            public:
                SocketFactory();

                virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator, const std::string& name = DEFAULT_ID);
                virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);

           private:
               SocketFactory(const SocketFactory &);
               SocketFactory & operator=(const SocketFactory &);
        };

        inline SocketFactory::SocketFactory() {}

    }
}

#endif
