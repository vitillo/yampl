/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginArbiter.hpp
 * @brief  The file contains structures and procedures required to load and manage Yampl plugins
 */

#ifndef YAMPL_PLUGINARBITER_HPP
#define YAMPL_PLUGINARBITER_HPP

#include "DynamicModule.hpp"
#include "PluginApi.h"
#include "IObject.hpp"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <stack>

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
            /* Plugin Errors */
            PluginGeneralError,
            PluginMainFailure,
        };

        template <typename _Key, typename _Val>
        using hash_map = std::unordered_map<_Key, _Val>;

        class PluginArbiter
        {
            protected:
                static std::shared_ptr<PluginArbiter> _singleton; //!< Singleton instance of the PluginArbiter
                hash_map<std::string, std::shared_ptr<DynamicModule>> _module_map; //!< Module map
                hash_map<std::string, hash_map<object_proto_type, object_register_params>> _object_registration_map; //!< Object registration map
                std::stack<plugin_info_hdr*> _module_init_stack; //!< Stack used by callback functions to retrieve the plugin header
                uint32_t _handle_counter { 0 };
                mutable std::recursive_mutex _map_shared_mtx;

                /**
                 * Initializes the plugin
                 *
                 * @param module the shared pointer to the plugin module
                 * @return the status of the plugin initialization
                 */
                virtual PluginStatus on_module_load(std::shared_ptr<DynamicModule> module);

                /**
                 * Unloads a module given its moniker
                 *
                 * @param moniker the moniker of the plugin module
                 * @return a boolean indicating whether the module was unloaded
                 */
                virtual bool unload_impl(std::string moniker);

                /************************** Hooks **/
                /**
                 * Callback used by plugins to register their objects
                 *
                 * @param params object registration parameters
                 * @return hook_exec_status returned by PluginArbiter
                 */
                hook_exec_status HOOK_register_object(object_register_params* params);

                /**
                 * Function used by handle owners to create objects
                 *
                 * @param params object initialization parameters
                 * @return pointer to the created object
                 */
                template <typename Ty>
                Ty* HOOK_create_object(std::string moniker, object_proto_type type, uint32_t obj_version)
                {
                    object_init_params init_params;
                    object_register_params reg_params;
                    Ty* obj;

                    try
                    {
                        // Retrieve the object_init_params
                        reg_params = _object_registration_map.at(moniker).at(type);

                        // Object initialization params
                        init.obj_version = obj_version;
                        init.type = type;

                        obj = reg_params.hk_create(init_params);
                    }
                    catch (std::out_of_range& ex)
                    {
                        obj = nullptr;
                    }

                    return obj;
                }
            private:
                PluginArbiter(PluginArbiter const&);
            public:
                /**
                 * @brief DynamicModule handle
                 */
                class Handle
                {
                    protected:
                        std::string _moniker;
                        uint32_t    _handle_id;
                        std::shared_ptr<PluginArbiter> _arbiter;
                    public:
                        static constexpr uint32_t _handle_id_invalid = -1;

                        Handle() noexcept;
                        Handle(std::string moniker, uint32_t handle_id, std::shared_ptr<PluginArbiter> arbiter) noexcept;

                        template <typename Ty>
                        Ty* create_object(object_proto_type type) const {
                            return create_object<Ty>(type, OBJECT_VERSION_ANY);
                        }

                        template <typename Ty>
                        Ty* create_object(object_proto_type type, uint32_t obj_version) const {
                            opaque_ptr obj = _arbiter->HOOK_create_object(type, obj_version);

                            return reinterpret_cast<Ty*>(obj);
                        }

                        void destroy_object(IObject* obj) const;

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
