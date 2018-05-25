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
         * @brief Plugin object base interface
         *
         */
        class IObject
        {
            public:
                static const uint32_t __OBJECT_VERSION;

                IObject() {}
                virtual ~IObject() = 0;
        };

        inline IObject::~IObject() {}
    }
}

#endif // PLUGA_TEST_IPLUGIN_HPP
