/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PySocketFactory.h
 * @brief  SocketFactory wrapper for Pybind11
 */

#include "PyISocketFactory.h"

#include "yampl/SocketFactory.h"
#include "yampl/Exceptions.h"

#ifndef YAMPL_PYSOCKETFACTORY_H
#define YAMPL_PYSOCKETFACTORY_H

namespace yampl
{
    using deallocator_t = decltype(yampl::defaultDeallocator);

    namespace py
    {
        /**
         * @brief Python wrapper class for yampl::SocketFactory
         */
        class PySocketFactory : public PyISocketFactory
        {
            protected:
                SocketFactory factory;
            public:
                PySocketFactory() : factory() { }
                ~PySocketFactory() override = default;

                /**
                 * @see  SocketFactory::createClientSocket
                 * @note deallocator must point to a valid non-temporary function (eg. not a lambda)
                 */
                ISocket* createClientSocket(Channel channel, Semantics semantics = COPY_DATA, deallocator_fn_t deallocator = defaultDeallocator, const std::string& name = DEFAULT_ID) override
                {
                    // Forward
                    return factory.createClientSocket(channel, semantics, *deallocator.template target<deallocator_t*>(), name);
                }

                /**
                * @see  SocketFactory::createServerSocket
                * @note deallocator must point to a valid non-temporary function (eg. not a lambda)
                */
                ISocket* createServerSocket(Channel channel, Semantics semantics = COPY_DATA, deallocator_fn_t deallocator = defaultDeallocator) override
                {
                    // Forward
                    auto f = factory.createServerSocket(channel, semantics, *deallocator.template target<deallocator_t*>());
                    return f;
                }
        };
    }

    using create_client_socket_t = yampl::ISocket* (py::PySocketFactory::*)(yampl::Channel, yampl::Semantics, yampl::deallocator_fn_t, std::string const&);
    using create_server_socket_t = yampl::ISocket* (py::PySocketFactory::*)(yampl::Channel, yampl::Semantics, yampl::deallocator_fn_t);
}

#endif // YAMPL_PYSOCKETFACTORY_H
