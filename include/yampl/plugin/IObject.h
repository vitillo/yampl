/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   IObject.h
 * @brief  Base interface for plugin objects
 *
 */

#ifndef YAMPL_IOBJECT_HPP
#define YAMPL_IOBJECT_HPP

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

#endif // YAMPL_IOBJECT_HPP
