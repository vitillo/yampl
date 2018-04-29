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
        /**
         * @brief Yampl plugin base interface
         */
        class IPlugin
        {
            public:
                IPlugin() {}
                virtual ~IPlugin() {}
        };
    }
}

#endif // PLUGA_TEST_IPLUGIN_HPP
