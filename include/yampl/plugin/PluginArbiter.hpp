/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginArbiter.hpp
 * @brief  The file contains structures and procedures required to load and manage Yampl plugins
 */

#ifndef YAMPL_PLUGINARBITER_HPP
#define YAMPL_PLUGINARBITER_HPP

#include "DynamicModule.hpp"
#include "PluginApi.h"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <list>

namespace yampl
{
    namespace plugin
    {
        enum class PluginStatus : uint8_t
        {
            Unknown = 0UL,
            Ok,
            /* Module Errors */
            ModuleNotFound,
            /* API Errors */
            ApiVersionMismatch,
            /* Internal Errors */
            InternalGeneralError,
        };

        class PluginArbiter
        {
            protected:
                static std::shared_ptr<PluginArbiter> _singleton; //!< Singleton instance of the PluginArbiter
                std::unordered_map<std::string, std::shared_ptr<DynamicModule>> _module_map; //!< Module map
                mutable std::mutex _module_map_mtx;

                /**
                 * Initializes the plugin
                 *
                 * @param module the shared pointer to the plugin module
                 * @return the status of the plugin initialization
                 */
                virtual PluginStatus OnModuleLoad(std::shared_ptr<DynamicModule> module);

                /**
                 * Unloads a module given its moniker
                 *
                 * @param moniker the moniker of the plugin module
                 * @return a boolean indicating whether the module was unloaded
                 */
                virtual bool unload_impl(std::string moniker);

                /************************** Hooks **/
//                virtual hook_exec_status hook_register_object;
            private:
                PluginArbiter(PluginArbiter const&);
            public:
                /**
                 * @brief DynamicModule handle
                 */
                class Handle
                {
                    protected:
                        std::shared_ptr<DynamicModule> _module;
                    public:
                        Handle() noexcept;
                        Handle(std::shared_ptr<DynamicModule> module) noexcept;

                        template<typename _Sym>
                        _Sym* resolve_sym(std::string sym) {
                            return _module->resolve_sym<plugin_info_hdr>(sym);
                        }

                        std::string moniker() const;
                };

                enum class DiscoveryMode
                {
                    Standard, // The base directory contains all the plugins
                    Recurse,  // The base directory contains the plugins wrapped in their respective folders
                };

                PluginArbiter();
                virtual ~PluginArbiter();

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
                virtual void load_all(std::string base_path, DiscoveryMode mode);

                /**
                 * Loads a plugin stored in the specified base path
                 *
                 * @param base_path the path to the plugin base directory
                 * @param plugin_module_name the name of the module to load
                 * @param mode specifies how the PluginArbiter should discover the plugin in the given directory
                 * @throws PluginArbiterLoadException
                 */
                virtual Handle load(std::string base_path, std::string plugin_module_name, DiscoveryMode mode);

                /**
                 * @todo get_handle()
                 */

                /**
                * @brief unloads all the plugins
                */
                virtual void unload_all();

                /**
                * Unloads the plugin held by the specified handle
                 *
                * @return a boolean indicating whether the module was unloaded
                */
                virtual bool unload(Handle const& handle);
        };
    }
}

#endif // YAMPL_PLUGINARBITER_HPP
