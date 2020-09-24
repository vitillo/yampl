#include <zmq.hpp>

#include "SocketFactory.h"
#include "ClientSocket.h"
#include "ServerSocket.h"

namespace yampl
{
    namespace zeromq
    {
        /****************** HOOK_*Object callbacks **/
        opaque_ptr HOOK_CreateObject(object_init_params* params)
	{
	    static SocketFactory _singleton;
            return reinterpret_cast<opaque_ptr>(&_singleton);
        }

        hook_exec_status HOOK_DestroyObject(opaque_ptr obj)
        {
            return HOOK_STATUS_SUCCESS;
        }

        /*************************** SocketFactory **/
        SocketFactory::SocketFactory() : m_context(new zmq::context_t(1)) { }

        SocketFactory::~SocketFactory() {
            delete m_context;
        }

        ISocket *SocketFactory::createClientSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *), const std::string& name) {
            return new ClientSocket(channel, m_context, semantics, deallocator, name);
        }

        ISocket *SocketFactory::createServerSocket(Channel channel, Semantics semantics, void (*deallocator)(void *, void *)) {
            return new ServerSocket(channel, m_context, semantics, deallocator);
        }

  }
}
