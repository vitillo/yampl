/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PyBindModule.cpp
 * @brief  PyBind11 bindings for YAMPL
 */

#include "config.h"

#include "yampl/Channel.h"
#include "yampl/ISocket.h"
#include "yampl/ISocketFactory.h"
#include "yampl/SocketFactory.h"
#include "yampl/utils/Globals.h"

#include "yampl/py/PyCast.h"
#include "yampl/py/PySocketFactory.h"
#include "yampl/py/PySocket.h"

#include <functional>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace py::literals;

template<typename R, typename... Args>
constexpr auto make_pythunk(std::function<R (Args...)> fn) {
    return [&](Args... x) { return fn(x...); };
}

template<typename R, typename... Args>
constexpr auto make_pythunk(R (*fn)(Args...)) {
    return [&](Args... x) { return fn(x...); };
}

PYBIND11_MODULE(yampl, self)
{
    self.doc() = R"pbdoc(
        YAMPL Bindings for Python
        --------------------------

        .. currentmodule:: yampl

        .. autosummary::
           :toctree: _generate
           Context
           Channel
           Semantics
           ISocket
           PySocket
           ClientSocket
           ServerSocket
           ISocketFactory
           SocketFactory
    )pbdoc";

    self.attr("__version__") = YAMPL_PY_VERSION;

    /**
    * @see yampl::Context
    */
    py::enum_<yampl::Context>(self, "Context", R"pbdoc(Context)pbdoc")
    .value("THREAD", yampl::Context::THREAD)
    .value("LOCAL_SHM", yampl::Context::LOCAL_SHM)
    .value("LOCAL_PIPE", yampl::Context::LOCAL_PIPE)
    .value("LOCAL", yampl::Context::LOCAL)
    .value("DISTRIBUTED", yampl::Context::DISTRIBUTED);

    /**
    * @see yampl::Channel
    */
    py::class_<yampl::Channel>(self, "Channel", R"pbdoc(Channel)pbdoc")
    .def(py::init<const std::string&, yampl::Context>(), "name"_a, "context"_a)
    .def_readwrite("name", &yampl::Channel::name)
    .def_readwrite("context", &yampl::Channel::context);

    /**
    * @see yampl::Semantics
    */
    py::enum_<yampl::Semantics>(self, "Semantics", R"pbdoc(Buffer copy semantics)pbdoc")
    .value("COPY_DATA", yampl::Semantics::COPY_DATA)
    .value("MOVE_DATA", yampl::Semantics::MOVE_DATA);


    /**
    * @see  yampl::ISocket
    * @note abstract
    */
    py::class_<yampl::ISocket>(self, "ISocket", R"pbdoc(Socket abstract base class)pbdoc")
    .def("send", static_cast<yampl::py::send0_t>(&yampl::ISocket::send), "buffer"_a, "size"_a, "hint"_a = nullptr)
    .def("recv", static_cast<yampl::py::recv0_t>(&yampl::ISocket::recv), "buffer"_a, "peerId"_a = yampl::DEFAULT_ID)
    .def("tryRecv", static_cast<yampl::py::try_recv0_t>(&yampl::ISocket::tryRecv), "buffer"_a, "timeout"_a = 0, "peerId"_a = yampl::DEFAULT_ID);

    /**
    * @see  yampl::py::PySocket
    */
    py::class_<yampl::py::PySocket>(self, "PySocket", R"pbdoc(ISocket wrapper class)pbdoc")
    .def(py::init<std::string, std::string>(), "name"_a, "context"_a)
    .def("send", &yampl::py::PySocket::send, "message"_a)
    .def("send_raw", &yampl::py::PySocket::send_raw, "message"_a)
    .def("recv", &yampl::py::PySocket::recv)
    .def("recv_raw", &yampl::py::PySocket::recv_raw)
    .def("try_recv", &yampl::py::PySocket::try_recv, "timeout"_a = 0)
    .def("try_recv_raw", &yampl::py::PySocket::try_recv_raw, "timeout"_a = 0);

    /**
    * @see  yampl::py::ClientSocket
    */
    py::class_<yampl::py::ClientSocket, yampl::py::PySocket>(self, "ClientSocket", R"pbdoc(ClientSocket wrapper class)pbdoc")
    .def(py::init<std::string, std::string>(), "name"_a, "context"_a);

    /**
    * @see  yampl::py::ServerSocket
    */
    py::class_<yampl::py::ServerSocket, yampl::py::PySocket>(self, "ServerSocket", R"pbdoc(ServerSocket wrapper class)pbdoc")
    .def(py::init<std::string, std::string>(), "name"_a, "context"_a);

    /**
    * @see  yampl::py::Py(I)SocketFactory
    */
    std::function<void (void*, void*)> deallocator = yampl::defaultDeallocator;

    py::class_<yampl::py::PyISocketFactory>(self, "ISocketFactory", R"pbdoc(Socket factory base class)pbdoc")
    .def("createClientSocket", static_cast<yampl::create_client_socket_t_>(&yampl::py::PyISocketFactory::createClientSocket),
                 "channel"_a, "semantics"_a = yampl::Semantics::COPY_DATA,
                 "deallocator"_a = deallocator, "name"_a = yampl::DEFAULT_ID)
    .def("createServerSocket", static_cast<yampl::create_server_socket_t_>(&yampl::py::PyISocketFactory::createServerSocket),
                 "channel"_a, "semantics"_a = yampl::Semantics::COPY_DATA,
                 "deallocator"_a = deallocator);

    py::class_<yampl::py::PySocketFactory, yampl::py::PyISocketFactory>(self, "SocketFactory", R"pbdoc(Socket factory class)pbdoc")
    .def(py::init<>())
    .def("createClientSocket", static_cast<yampl::create_client_socket_t>(&yampl::py::PySocketFactory::createClientSocket),
         "channel"_a, "semantics"_a = yampl::Semantics::COPY_DATA,
         "deallocator"_a = deallocator, "name"_a = yampl::DEFAULT_ID)
    .def("createServerSocket", static_cast<yampl::create_server_socket_t>(&yampl::py::PySocketFactory::createServerSocket),
         "channel"_a, "semantics"_a = yampl::Semantics::COPY_DATA,
         "deallocator"_a = deallocator);
}