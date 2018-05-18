/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginArbiter.hpp
 * @brief  The file contains structures and procedures required to load and manage Yampl plugins
 */

#ifndef YAMPL_PLUGINARBITER_HPP
#define YAMPL_PLUGINARBITER_HPP

#include "DynamicModule.hpp"

#include <cstdint>
#include <memory>
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
                std::unordered_map<std::string, std::shared_ptr<DynamicModule>> _module_map; //!< Module map
                mutable std::mutex _module_map_mtx;

        };
    }
}

#endif // YAMPL_PLUGINARBITER_HPP
