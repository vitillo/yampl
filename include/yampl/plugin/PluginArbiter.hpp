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
            protected:
                static std::shared_ptr<PluginArbiter> _singleton; //!< Singleton instance of the PluginArbiter
                std::unordered_map<std::string, std::shared_ptr<DynamicModule>> _module_map; //!< Module map
                mutable std::mutex _module_map_mtx;

                virtual ~PluginArbiter();
            private:
                PluginArbiter();
                PluginArbiter(PluginArbiter const&);
            public:
                enum class PluginDiscoveryMode
                {
                    Standard,       // The base directory contains all the plugins
                    RecurseShallow, // The base directory contains the plugins wrapped in their respective folders
                    RecurseDeep     // The base directory contains the plugins, but the depth is unknown
                };

                /**
                 * Builds a singleton instance of the class, if it hasn't been built already
                 *
                 * @return a shared pointer to the created instance
                 */
                static std::shared_ptr<PluginArbiter> get_instance();

                /**
                 * Loads all the plugins stored in the specified base path
                 *
                 * @param base_path the path to the plugin base directory
                 * @param mode specifies how the PluginArbiter should discover the plugins in the directory
                 * @throws PluginArbiterLoadException
                 */
                void load_all(std::string base_path, PluginDiscoveryMode mode = PluginDiscoveryMode::Standard);

                /**
                 * Loads a plugin stored in the specified base path
                 *
                 * @param base_path the path to the plugin base directory
                 * @param plugin_module_name the name of the module to load
                 * @param mode specifies how the PluginArbiter should discover the plugin in the given directory
                 * @throws PluginArbiterLoadException
                 */
                void load(std::string base_path, std::string plugin_module_name, PluginDiscoveryMode mode = PluginDiscoveryMode::Standard);

                /**
                * @brief unloads all the plugins
                */
                void unload_all();

                /**
                * @brief unloads the specified plugin
                * @return a boolean indicating whether the module was unloaded
                */
                bool unload(std::string plugin_module_name);
        };
    }
}

#endif // YAMPL_PLUGINARBITER_HPP
