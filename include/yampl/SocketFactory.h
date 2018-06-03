#ifndef YAMPL_SOCKETFACTORY_H
#define YAMPL_SOCKETFACTORY_H

#include "yampl/ISocketFactory.h"
#include "plugin/IObject.h"
#include "plugin/PluginArbiter.h"

#include <vector>

namespace yampl
{
    using plugin::PluginArbiter;

    namespace ZMQ {
      class SocketFactory;
    }

    namespace pipe {
      class SocketFactory;
    }

    class SocketFactory : public ISocketFactory
    {
        private:
            std::string module_base_path;
            std::shared_ptr<PluginArbiter> arbiter;
            std::vector<PluginArbiter::Handle> factory_handle_list;

            SocketFactory & operator=(const SocketFactory &);

        public:
            static constexpr char const* SHM_MODULE_NAME  = "yampl-shm";
            static constexpr char const* ZMQ_MODULE_NAME  = "yampl-zmq";
            static constexpr char const* PIPE_MODULE_NAME = "yampl-pipe";

            SocketFactory() noexcept;
            explicit SocketFactory(std::string) noexcept;

            ~SocketFactory() override;

            virtual ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator, std::string const& name = DEFAULT_ID);
            virtual ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator);
    };

}

#endif
