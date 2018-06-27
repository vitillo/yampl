#ifndef YAMPL_SHM_SOCKETFACTORY_H
#define YAMPL_SHM_SOCKETFACTORY_H

#include "yampl/ISocketFactory.h"
#include "yampl/plugin/PluginApi.h"

namespace yampl
{
    namespace shm
    {
        opaque_ptr HOOK_CreateObject(object_init_params*);
        hook_exec_status HOOK_DestroyObject(opaque_ptr);
	
        class SocketFactory : public ISocketFactory
	{
          public:
            static constexpr char const* PLUGIN_VERSION = "1.0";

            virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator, const std::string& name = DEFAULT_ID);
            virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);

          private:
            SocketFactory & operator=(const SocketFactory &);
        };
    }
}

#endif
