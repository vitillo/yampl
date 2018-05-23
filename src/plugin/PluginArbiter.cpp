/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginArbiter.cpp
 * @brief  The file contains structures and procedures required to load and manage Yampl plugins
 */

#include "yampl/plugin/PluginArbiter.hpp"

namespace yampl
{
    namespace plugin
    {
        static std::shared_ptr<PluginArbiter> PluginArbiter::_singleton = nullptr;
    }
}