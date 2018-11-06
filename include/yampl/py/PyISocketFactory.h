/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PyISocketFactory.h
 * @brief  ISocketFactory wrapper for Pybind11
 */

#include "yampl/ISocketFactory.h"

#include <functional>

#ifndef YAMPL_PYISOCKETFACTORY_H
#define YAMPL_PYISOCKETFACTORY_H

namespace yampl
{
    using deallocator_fn_t = std::function<void (void*, void*)>;

    namespace py
    {
        /**
         * @brief Python wrapper class for yampl::ISocketFactory
         */
        class PyISocketFactory : public ISocketFactory
        {
            public:
                using ISocketFactory::ISocketFactory;

                virtual ISocket* createClientSocket(Channel channel, Semantics semantics = COPY_DATA, deallocator_fn_t deallocator = defaultDeallocator, const std::string& name = DEFAULT_ID) = 0;
                virtual ISocket* createServerSocket(Channel channel, Semantics semantics = COPY_DATA, deallocator_fn_t deallocator = defaultDeallocator) = 0;
        protected:
            ISocket *createClientSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator, const std::string& name = DEFAULT_ID) override {}
            ISocket *createServerSocket(Channel channel, Semantics semantics = COPY_DATA, void (*deallocator)(void *, void *) = defaultDeallocator) override {}
        };
    }

    using create_client_socket_t_ = yampl::ISocket* (py::PyISocketFactory::*)(yampl::Channel, yampl::Semantics, yampl::deallocator_fn_t, std::string const&);
    using create_server_socket_t_ = yampl::ISocket* (py::PyISocketFactory::*)(yampl::Channel, yampl::Semantics, yampl::deallocator_fn_t);
}

#endif // YAMPL_PYISOCKETFACTORY_H
