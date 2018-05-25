#include "yampl/SocketFactory.h"
#include "yampl/utils/utils.h"

#include <cstdlib>

namespace yampl
{
    std::string DEFAULT_ID = "";
    
    SocketFactory::SocketFactory() noexcept
        : arbiter(PluginArbiter::get_instance())
        , module_base_path(get_plugin_base_dir())
    {

    }

    SocketFactory::SocketFactory(std::string base_path) noexcept
        : arbiter(PluginArbiter::get_instance())
        , module_base_path(std::move(base_path))
    {

    }

    SocketFactory::~SocketFactory() = default;

    ISocket* SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name)
    {
        ISocket* socket = nullptr;
        PluginArbiter::Handle handle;

        if (channel.context == LOCAL_PIPE) {
            PluginArbiter::Handle handle = arbiter->load(dir_path_normalize(module_base_path), PIPE_MODULE_NAME, PluginArbiter::DiscoveryMode::Recurse);
        }
        else if (channel.context == LOCAL_SHM) {
            PluginArbiter::Handle handle = arbiter->load(dir_path_normalize(module_base_path), SHM_MODULE_NAME, PluginArbiter::DiscoveryMode::Recurse);
        }
        else {
            PluginArbiter::Handle handle = arbiter->load(dir_path_normalize(module_base_path), ZMQ_MODULE_NAME, PluginArbiter::DiscoveryMode::Recurse);
        }

        ISocketFactory* socket_factory = reinterpret_cast<ISocketFactory*>(handle.create_object<ISocketFactory>(OBJ_PROTO_SK_FACTORY));

        if (socket_factory != nullptr) {
            socket = socket_factory->createClientSocket(channel, semantics, deallocator);
            factory_handle_list.push_back(handle);
        }

        return socket;
    }

    ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *))
    {
        ISocket *socket;
        PluginArbiter::Handle handle;

        if (channel.context == LOCAL_PIPE) {
            PluginArbiter::Handle handle = arbiter->load(dir_path_normalize(module_base_path), PIPE_MODULE_NAME, PluginArbiter::DiscoveryMode::Recurse);
        }
        else if (channel.context == LOCAL_SHM) {
            PluginArbiter::Handle handle = arbiter->load(dir_path_normalize(module_base_path), SHM_MODULE_NAME, PluginArbiter::DiscoveryMode::Recurse);
        }
        else {
            PluginArbiter::Handle handle = arbiter->load(dir_path_normalize(module_base_path), ZMQ_MODULE_NAME, PluginArbiter::DiscoveryMode::Recurse);
        }

        ISocketFactory* socket_factory = reinterpret_cast<ISocketFactory*>(handle.create_object<ISocketFactory>(OBJ_PROTO_SK_FACTORY));

        if (socket_factory != nullptr) {
            socket = socket_factory->createServerSocket(channel, semantics, deallocator);
            factory_handle_list.push_back(handle);
        }

        return socket;
    }
}
