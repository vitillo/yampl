/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PySocket.h
 * @brief  Socket wrapper for PyBind11
 */

#ifndef YAMPL_PYSOCKET_H
#define YAMPL_PYSOCKET_H

#include "yampl/ISocket.h"

#include "yampl/py/PyCast.h"
#include "yampl/py/PySocketFactory.h"

#include <algorithm>
#include <memory>
#include <string>
#include <tuple>

namespace py_ = pybind11;

namespace yampl
{
    namespace py
    {
        using send0_t = void (yampl::ISocket::*)(void*, size_t, void*);
        using recv0_t = ssize_t (yampl::ISocket::*)(void*&, std::string&);
        using try_recv0_t = ssize_t (yampl::ISocket::*)(void*&, long, std::string&);

        /**
         * @brief Python wrapper class for yampl::ISocket
         */
        class PySocket
        {
            protected:
                std::unique_ptr<ISocket> socket;
                std::unique_ptr<PySocketFactory> factory;
                Channel channel;
                byte_buffer data;

                auto _recv_impl(bool raw = false, bool try_impl = false, long timeout = 0)
                {
                    char* data = nullptr;
                    ssize_t size;

                    if (try_impl)
                        size = socket->tryRecv(data, timeout);
                    else
                        size = socket->recv(data);

                    byte_buffer buffer { reinterpret_cast<byte_buffer::data_type*>(data), size };

                    py_::object obj;

                    // Decode the data iff not calling the try* implementation or recv was successful
                    if (!try_impl || size != -1)
                    {
                        if (raw)
                            obj = py_::reinterpret_borrow<py_::object>(py_::cast(buffer));
                        else
                            obj = py_::make_tuple(size, pickler::loads(buffer));
                    }
                    else
                        obj = py_::make_tuple(-1, py_::none());

                    // Free the allocated memory and return a tuple containing the read bytes and the object
                    if (size != -1)
                        defaultDeallocator(reinterpret_cast<void*>(data), nullptr);
                    return obj;
                }
            public:
                virtual ~PySocket() {}

                /**
                 *
                 * @param name utf-8 encoded name
                 * @param context channel context
                 */
                PySocket(std::string name, std::string context)
                {
                    Context ctx;

                    std::transform(context.begin(), context.end(), context.begin(), ::toupper);

                    // Decode the context string to a yampl::Context
                    if (context == "THREAD")
                        ctx = Context::THREAD;
                    else if (context == "LOCAL")
                        ctx = Context::LOCAL;
                    else if (context == "LOCAL_SHM")
                        ctx = Context::LOCAL_SHM;
                    else if (context == "LOCAL_PIPE")
                        ctx = Context::LOCAL_PIPE;
                    else if (context == "DISTRIBUTED")
                        ctx = Context::DISTRIBUTED;
                    else
                        throw py_::value_error(context + " is not a valid context.");

                    channel.name = name;
                    channel.context = ctx;

                    factory = std::make_unique<PySocketFactory>();
                }

                void send(py_::object message)
                {
                    // Pickle
                    byte_buffer raw = pickler::dumps(message).cast<byte_buffer>();

                    auto data = raw.getBuffer();

                    this->data = raw;
                    socket->send(raw.getBuffer(), raw.getSize());
                }

                void send_raw(std::string message)
                {
                    socket->send(message);
                }

                auto recv()
                {
                    return _recv_impl();
                }

                auto try_recv(long timeout = 0)
                {
                    return _recv_impl(false, true, timeout);
                }

                auto recv_raw()
                {
                    return _recv_impl(true);
                }

                auto try_recv_raw(long timeout)
                {
                    return _recv_impl(true, true, timeout);
                }
        };

       /**
        * @brief ClientSocket class
        */
        class ClientSocket : public PySocket
        {
            public:
                ClientSocket(std::string name, std::string context)
                    : PySocket(name, context)
                {
                    socket = std::unique_ptr<ISocket>(factory->createClientSocket(channel));
                }
        };

       /**
        * @brief ServerSocket class
        */
        class ServerSocket : public PySocket
        {
            public:
                ServerSocket(std::string name, std::string context)
                    : PySocket(name, context)
                {
                    socket = std::unique_ptr<ISocket>(factory->createServerSocket(channel));
                }
        };
    };
}

#endif // YAMPL_PYSOCKET_H
