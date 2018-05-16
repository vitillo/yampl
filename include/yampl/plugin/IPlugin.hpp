/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   IPlugin.hpp
 *
 */

#ifndef PLUGA_TEST_IPLUGIN_HPP
#define PLUGA_TEST_IPLUGIN_HPP

#include <cstdint>

namespace yampl
{
    namespace plugin
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
