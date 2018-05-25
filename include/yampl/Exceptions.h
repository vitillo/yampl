#ifndef YAMPL_EXCEPTIONS_H
#define YAMPL_EXCEPTIONS_H

#include "plugin/DynamicModule.hpp"
#include "plugin/PluginArbiter.hpp"
#include "plugin/PluginApi.h"

#include <exception>
#include <cstdio>
#include <cstring>
#include <memory>

#include <errno.h>

namespace yampl
{
    class Exception: public std::exception
    {
      public:
        Exception(const char* msg) : m_msg(msg) { }
        virtual ~Exception() throw() = default;

        const char * what() const throw() override {
          return m_msg.c_str();
        }

      protected:
        const std::string m_msg;
    };

    class UnsupportedException: public Exception
    {
        public:
            UnsupportedException() : Exception("This feature is not supported yet") {}
            /**
             * @author Ayoub Chouak
             */
            UnsupportedException(char* const what) : Exception(what) {}
    };

    class InvalidOperationException: public Exception{
      public:
      InvalidOperationException(const char *msg = "Operation not permitted"): Exception(msg){}
    };

    class InvalidSizeException: public Exception{
      public:
      InvalidSizeException(): Exception("Buffer size is not sufficient"){}
    };

    class UnroutableException : public Exception{
      public:
        UnroutableException(): Exception("Destination peer not found"){}
    };

    class ErrnoException : public Exception{
      public:
        ErrnoException(const char *msg = "System call error"): Exception(msg), m_errno(errno) {
        }

        ErrnoException(int err, const char *msg = "System call error"): Exception(msg), m_errno(err){}

        int getErrno(){
          return m_errno;
        }

        virtual const char * what() const throw(){
          snprintf(m_errnoMsg, s_maxMsgSize, "%s: %s", m_msg, strerror(m_errno));
          return m_errnoMsg;
        }

      private:
        static const size_t s_maxMsgSize = 100;
        mutable char m_errnoMsg[s_maxMsgSize];
        int m_errno;
    };

    namespace plugin
    {
        class DynamicModuleException : public Exception
        {
            protected:
                hook_exec_status_ex _internal_status;
            public:
                explicit DynamicModuleException(char const* what) noexcept
                        : Exception(what)
                {
                    _internal_status.status = HOOK_STATUS_UNKNOWN;
                    _internal_status.status_code = static_cast<uint32_t>(-1);
                }

                DynamicModuleException(const char* what, hook_exec_status_ex internal_status)
                        : Exception(what)
                        , _internal_status(internal_status)
                {

                }

                virtual hook_exec_status_ex internal_status() const {
                    return _internal_status;
                }
        };

        /**
         * DynamicModule exceptions
         */
        class DynamicModuleLoadException : public DynamicModuleException
        {
            public:
                explicit DynamicModuleLoadException(char const* what = "Module load failed unexpectedly") noexcept
                        : DynamicModuleException(what)
                { }
        };

        class DynamicModuleSymbolException : public DynamicModuleException
        {
            private:
                DynamicModule::SymbolError _error_type;

            public:
                explicit DynamicModuleSymbolException(char const* what, DynamicModule::SymbolError error_type) noexcept
                        : DynamicModuleException(what)
                        , _error_type(error_type)
                { }

                virtual DynamicModule::SymbolError error_type() const noexcept {
                    return _error_type;
                }
        };

        /**
         * PluginArbiter exceptions
         */
        class PluginArbiterException : public Exception
        {
            protected:
                PluginStatus _status;

            public:
                explicit PluginArbiterException(char const* what = "PluginArbiter failed loading the plugin") noexcept
                        : PluginArbiterException(std::string(what))
                { }

                PluginArbiterException(PluginArbiterException const& rhs) noexcept
                    : PluginArbiterException(std::string(rhs.what()), rhs.status()) {

                };

                PluginArbiterException(char const* what, PluginStatus status) noexcept
                    : PluginArbiterException(std::string(what), status)
                { }

                PluginArbiterException(std::string what) noexcept
                    : PluginArbiterException(std::move(what), PluginStatus::Unknown)
                {

                }

                PluginArbiterException(std::string what, PluginStatus status) noexcept
                    : m_msg(what)
                    , _status(status)
                {

                }

                PluginStatus status() const {
                    return _status;
                }
        };
    }
}

#endif
