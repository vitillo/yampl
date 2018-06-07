/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginArbiter.h
 * @brief  The file contains structures and procedures required to load and manage Yampl plugins
 */

#ifndef YAMPL_PLUGINARBITER_HPP
#define YAMPL_PLUGINARBITER_HPP

#include "DynamicModule.h"
#include "PluginApi.h"
#include "IObject.h"

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
        /**
         * PluginArbiter::HOOK_register_object function thunk
         *
         * @param params object registration parameters
         * @return hook_exec_status returned by PluginArbiter
         */
        hook_exec_status HOOK_register_object_thunk(object_register_params* params);

        /******************************* PluginArbiter **/
        enum class PluginStatus : uint8_t
        {
            Unknown = 0UL,
            Ok,
            /* Module Errors */
            ModuleLoadError,
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
                hash_map<std::string, std::shared_ptr<DynamicModule>> _module_map; //!< Module map
                hash_map<std::string, std::string> _module_moniker_map; //!< Module name to moniker map
                hash_map<std::string, hash_map<object_proto_type, object_register_params>> _object_registration_map; //!< Object registration map
                std::stack<plugin_info_hdr*> _module_init_stack; //!< Stack used by callback functions to retrieve the plugin header
                uint32_t _handle_counter { 0 };
                mutable std::recursive_mutex _map_shared_mtx; //!< Shared mutex for interlocked access to the class members

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
                friend hook_exec_status HOOK_register_object_thunk(object_register_params*);

                /**
                 * Callback invoked by plugins to register objects
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
                Ty* HOOK_create_object(std::string moniker, object_proto_type type, uint32_t obj_version) const
                {
                    object_init_params init_params;
                    object_register_params reg_params;
                    Ty* obj;

                    try
                    {
                        // Retrieve the object_init_params
                        reg_params = _object_registration_map.at(moniker).at(type);

                        // Object initialization params
                        init_params.obj_version = obj_version;
                        init_params.type = type;

                        obj = reinterpret_cast<Ty*>(reg_params.hk_create(&init_params));
                    }
                    catch (std::out_of_range& ex)
                    {
                        obj = nullptr;
                    }

                    return obj;
                }

                /**
                 * Function used by handle owners to destroy an object
                 *
                 * @param moniker plugin moniker
                 * @param type the object type
                 * @param obj pointer to the object to destroy
                 * @return the hook_exec_status returned by DestroyObject
                 */
                hook_exec_status HOOK_destroy_object(std::string moniker, object_proto_type type, IObject* obj) const;
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
                        std::vector<IObject*> _obj_alloc_list; //!< List of created objects
                        hash_map<IObject*, object_proto_type> _obj_type_map; //!< Map of objects to their prototype

                        Handle(Handle const& rhs);
                    public:
                        static constexpr uint32_t _handle_id_invalid = -1;

                        Handle() noexcept;
                        Handle(std::string moniker, uint32_t handle_id, std::shared_ptr<PluginArbiter> arbiter) noexcept;
                        Handle(Handle&& rhs) noexcept;

                        Handle& operator =(Handle&& rhs) noexcept;

                        template <typename Ty>
                        Ty* create_object(object_proto_type type) {
                            Ty* obj = create_object<Ty>(type, OBJECT_VERSION_ANY);
                        }

                        template <typename Ty>
                        Ty* create_object(object_proto_type type, uint32_t obj_version)
                        {
                            opaque_ptr obj = _arbiter->HOOK_create_object<Ty>(_moniker, type, obj_version);

                            // If the object has been created, register it
                            if (obj != nullptr)
                            {
                                _obj_alloc_list.push_back(reinterpret_cast<IObject*>(obj));
                                _obj_type_map.insert({ reinterpret_cast<IObject*>(obj), type });
                            }

                            return reinterpret_cast<Ty*>(obj);
                        }

                        void destroy_object(IObject* obj) const;

                        std::string moniker() const;
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
                 * Loads a plugin stored in the specified base path
                 *
                 * @param base_path the path to the plugin base directory
                 * @param plugin_module_name the name of the module to load
                 * @throws PluginArbiterLoadException
                 */
                virtual Handle load(std::string base_path, std::string plugin_module_name);

                /**
                 * Loads all the plugins stored in the specified base path
                 *
                 * @param base_path the path to the plugin base directory
                 * @throws PluginArbiterLoadException
                 */
                virtual void load_all(std::string base_path);

                /**
                 * Returns a handle to the specified plugin
                 *
                 * @param moniker the plugin moniker
                 * @return a unique handle to the plugin
                 */
                virtual Handle get_handle(std::string moniker);

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
