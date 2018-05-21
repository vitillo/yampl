/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginArbiter.hpp
 * @brief  The file contains structures and procedures required to load and manage Yampl plugins
 */

#ifndef YAMPL_PLUGINARBITER_HPP
#define YAMPL_PLUGINARBITER_HPP

#include <cstdint>
#include <unordered_map>
#include <mutex>
#include <list>

namespace yampl
{
    namespace plugin
    {
        class PluginArbiter
        {
            private:
                std::unordered_map<std::shared_ptr<DynModule>> _module_map; //!< Module map
                std::list<std::shared_ptr<DynModule>> _module_list_lo; //!< Module list (Load order)
                mutable std::mutex _module_map_mtx;
                mutable std::mutex _module_list_lo;

        };
    }
}

#endif // YAMPL_PLUGINARBITER_HPP
