/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginArbiter.cpp
 * @brief  The file contains structures and procedures required to load and manage Yampl plugins
 */

#include "yampl/plugin/PluginArbiter.hpp"
#include "yampl/utils/utils.h"
#include "yampl/Exceptions.h"

#include <vector>
#include <iostream>

namespace yampl
{
    namespace plugin
    {
        hook_exec_status HOOK_register_object_thunk(object_register_params* params) {
            return PluginArbiter::get_instance()->HOOK_register_object(params);
        }

        /******************************* PluginArbiter **/
        std::shared_ptr<PluginArbiter> PluginArbiter::_singleton = nullptr;

        std::shared_ptr<PluginArbiter> PluginArbiter::get_instance()
        {
            // First time invoking get_instance?
            if (_singleton == nullptr)
            {
                _singleton = std::make_shared<PluginArbiter>();
            }

            return _singleton;
        }

        PluginArbiter::PluginArbiter() = default;

        PluginArbiter::PluginArbiter(PluginArbiter const& rhs)
        {

        }

        PluginArbiter::~PluginArbiter()
        {
            unload_all();
        }

        hook_exec_status PluginArbiter::HOOK_register_object(object_register_params* params)
        {
            std::lock_guard<std::recursive_mutex> _guard0(_map_shared_mtx);

            hook_exec_status status = HOOK_STATUS_SUCCESS;
            plugin_info_hdr* info_hdr = _module_init_stack.top();
            auto obj_type_map = _object_registration_map.at(info_hdr->moniker);

            switch (params->obj_type)
            {
                case OBJ_PROTO_UNKNOWN:
                    // IObject proto is invalid
                    status = HOOK_STATUS_FAILURE;
                    break;
                case OBJ_PROTO_SK_FACTORY:
                    // Add the object to the registration map
                    obj_type_map.insert({ params->obj_type, *params });
                    break;
            }

            return status;
        }

        hook_exec_status PluginArbiter::HOOK_destroy_object(std::string moniker, object_proto_type type, IObject *obj) const {
            auto obj_type_map = _object_registration_map.at(moniker);
            return obj_type_map.at(type).hk_destroy(obj);
        }

        PluginArbiter::Handle PluginArbiter::load(std::string base_path, std::string plugin_module_name, DiscoveryMode mode)
        {
            std::lock_guard<std::recursive_mutex> _guard0(_map_shared_mtx);

            // Open the dynamic module
            std::shared_ptr<DynamicModule> module = nullptr;
            Handle handle;

            try {
                if (mode == DiscoveryMode::Standard) {
                    module = std::make_shared<DynamicModule>(DynamicModule::open(dir_path_normalize(base_path),
                                                                                 to_full_module_name(
                                                                                         plugin_module_name)));
                } else if (mode == DiscoveryMode::Recurse) {
                    std::string full_base_path = dir_path_normalize(base_path) + plugin_module_name;
                    std::cout << dir_path_normalize(full_base_path) + to_full_module_name(plugin_module_name)
                              << std::endl;
                    module = std::make_shared<DynamicModule>(DynamicModule::open(dir_path_normalize(full_base_path),
                                                                                 to_full_module_name(
                                                                                         plugin_module_name)));
                }

                PluginStatus status = on_module_load(module);

                if (status != PluginStatus::Ok)
                    throw DynamicModuleLoadException();

                // Parse the module's header and push it to the stack
                auto *hdr = module->resolve_sym<plugin_info_hdr>(PLUGIN_HDR_EXPORT_SYM);
                _module_init_stack.push(hdr);

                // Initialize the object registration map
                _object_registration_map.insert({ hdr->moniker, hash_map<object_proto_type, object_register_params>() });

                // Insert the module in the map
                handle = Handle(hdr->moniker, _handle_counter, _singleton);
                _module_map.insert({ handle.moniker(), module });

                // Pop the info header off the stack
                _module_init_stack.pop();
            }
            catch (std::out_of_range& ex)
            {
                throw DynamicModuleLoadException("Synchronization error");
            }
            catch (DynamicModuleLoadException& ex)
            {
                if (!_module_init_stack.empty())
                    _module_init_stack.pop();

                // @todo: Decode the hook_exec_status to the corresponding PluginStatus
                std::string what = "PluginArbiter could not load module " + plugin_module_name;
                PluginArbiterException paex(what.c_str(), PluginStatus::Unknown);
                throw paex;
            }

            return handle;
        }

        void PluginArbiter::load_all(std::string base_path, DiscoveryMode mode)
        {

        }

        void PluginArbiter::unload_all()
        {
            std::lock_guard<std::recursive_mutex> _guard0(_map_shared_mtx);

            std::vector<std::string> moniker_list;

            for (auto entry : _module_map)
                moniker_list.push_back(entry.first);
            for (auto const& moniker : moniker_list)
                unload_impl(moniker);

            _handle_counter = 0;
        }

        bool PluginArbiter::unload_impl(std::string moniker)
        {
            std::lock_guard<std::recursive_mutex> _guard0(_map_shared_mtx);

            auto entry = _module_map.find(moniker);
            bool ok = true;

            if (entry == _module_map.end())
                ok = false;
            else
            {
                // Clear the associated entry in the object registration map
                if (_object_registration_map.find(moniker) != _object_registration_map.end())
                    _object_registration_map.erase(moniker);

                entry->second->release();
                _module_map.erase(entry);
                _handle_counter--;
            }

            return ok;
        }

        bool PluginArbiter::unload(Handle const& handle)
        {
            std::lock_guard<std::recursive_mutex> _guard0(_map_shared_mtx);

            return unload_impl(handle.moniker());
        }

        PluginStatus PluginArbiter::on_module_load(std::shared_ptr<DynamicModule> module)
        {
            std::lock_guard<std::recursive_mutex> _guard0(_map_shared_mtx);

            plugin_init_frame init_frame;
            plugin_info_hdr*  info_hdr;
            PluginStatus status = PluginStatus::Ok;

            // Retrieve the plugin header
            info_hdr =_module_init_stack.top();
            HOOK_PluginMain main_callback_fn = info_hdr->hk_plugin_main;

            // Build the plugin init frame
            init_frame.api_version = PLUGIN_API_VERSION;
            init_frame.hk_register = HOOK_register_object_thunk;

            // Call PluginMain
            hook_exec_status init_status = main_callback_fn(&init_frame);

            switch (init_status)
            {
                case HOOK_STATUS_SUCCESS:
                    status = PluginStatus::Ok;
                    break;
                case HOOK_STATUS_FAILURE:
                    status = PluginStatus::PluginMainFailure;
                    break;
                case HOOK_STATUS_UNKNOWN:
                    status = PluginStatus::Unknown;
                    break;
            }

            return PluginStatus::Ok;
        }

        /**
         * PluginArbiter::Handle
         */
        PluginArbiter::Handle::Handle() noexcept
            : Handle("", _handle_id_invalid, nullptr)
        {

        }

        PluginArbiter::Handle::Handle(std::string moniker, uint32_t handle_id, std::shared_ptr<PluginArbiter> arbiter) noexcept
            : _moniker(std::move(moniker))
            , _handle_id(handle_id)
            , _arbiter(std::move(arbiter))
        {

        }

        void PluginArbiter::Handle::destroy_object(IObject* obj) const
        {
            try
            {
                hook_exec_status status = _arbiter->HOOK_destroy_object(_moniker, _obj_type_map.at(obj), obj);
            }
            catch (std::out_of_range& ex) {
                throw PluginArbiterException("Error destroying object");
            }
            catch (PluginArbiterException& ex) {
                throw ex;
            }
        }

        std::string PluginArbiter::Handle::moniker() const {
            return _moniker;
        }
    }
}