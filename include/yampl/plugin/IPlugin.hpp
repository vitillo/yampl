/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   IPlugin.hpp
 *
 */

#ifndef PLUGA_TEST_IPLUGIN_HPP
#define PLUGA_TEST_IPLUGIN_HPP

#include <scy/pluga/pluga.h>
#include <cstdint>

namespace scy
{
    namespace pluga
    {
        enum class InvokeStatus
        {
            SUCCESS,
            FAILURE
        };

        struct InvokeResult
        {
            InvokeStatus status;
            uint32_t     extra;
        };

        class IPlugin
        {
            public:
                IPlugin() {}
                virtual ~IPlugin() {}

                virtual InvokeResult Invoke(const uint8_t* proc_id, const uint8_t* packed_args, uint32_t args_length) = 0;
        };
    }
}

#endif //PLUGA_TEST_IPLUGIN_HPP
