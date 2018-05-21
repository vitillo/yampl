/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   IObject.hpp
 * @brief  Base interface for plugin objects
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
        class IObject
        {
            public:
                static const uint32_t __OBJECT_VERSION;

                IObject() {}
                virtual ~IObject() {}
        };
    }
}

#endif // PLUGA_TEST_IPLUGIN_HPP
