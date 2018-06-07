/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginArbiter.cpp
 * @brief  The file contains structures and procedures required to load and manage Yampl plugins
 */

#include "yampl/plugin/PluginArbiter.h"
#include "yampl/utils/utils.h"
#include "yampl/Exceptions.h"

#include <vector>

namespace yampl
{
    namespace plugin
    {
        hook_exec_status HOOK_register_object_thunk(object_register_params* params) {
            return PluginArbiter::get_instance()->HOOK_register_object(params);
        }

        /******************************* PluginArbiter **/
        std::shared_ptr<PluginArbiter> PluginArbiter::get_instance()
        {
            static std::shared_ptr<PluginArbiter> _singleton = nullptr;

            // First time invoking get_instance?
            if (_singleton == nullptr)
                _singleton = std::make_shared<PluginArbiter>();

            return _singleton;
        }

        PluginArbiter::PluginArbiter() = default;

        PluginArbiter::PluginArbiter(PluginArbiter const& rhs) { }

        PluginArbiter::~PluginArbiter()
        {
            unload_all();
        }

        hook_exec_status PluginArbiter::HOOK_register_object(object_register_params* params)
        {
            hook_exec_status status;

            if (params == nullptr)
            {
                status = HOOK_STATUS_SUCCESS;
            }
            else
            {
                plugin_info_hdr* info_hdr = _module_init_stack.top();

                switch (params->obj_type)
                {
                    case OBJ_PROTO_UNKNOWN:
                        // IObject proto is invalid
                        status = HOOK_STATUS_FAILURE;
                        break;
                    case OBJ_PROTO_SK_FACTORY:
                        // Add the object to the registration map
                        _object_registration_map.at(info_hdr->moniker).insert({ params->obj_type, *params });
                        status = HOOK_STATUS_SUCCESS;
                        break;
                    default:
                        status = HOOK_STATUS_FAILURE;
                        break;
                }
            }

            return status;
        }

        hook_exec_status PluginArbiter::HOOK_destroy_object(std::string moniker, object_proto_type type, IObject *obj) const
        {
            auto obj_type_map = _object_registration_map.at(moniker);
            return obj_type_map.at(type).hk_destroy(obj);
        }

        PluginArbiter::Handle PluginArbiter::load(std::string base_path, std::string plugin_module_name)
        {
            std::lock_guard<std::recursive_mutex> __lock(_map_shared_mtx);

            Handle handle;
            auto moniker_itr = _module_moniker_map.find(plugin_module_name);

            // Ensure the module isn't loaded already
            if (moniker_itr == _module_moniker_map.end())
            {
                try
                {
                    std::shared_ptr<DynamicModule> module = std::make_shared<DynamicModule>(DynamicModule::open(dir_path_normalize(base_path),
                                                                                 to_full_module_name(plugin_module_name)));

                    // Parse the module header and push it to the stack
                    auto *hdr = module->resolve_sym<plugin_info_hdr>(PLUGIN_HDR_EXPORT_SYM);
                    _module_init_stack.push(hdr);

                    // Initialize the object registration map
                    _object_registration_map.insert({ hdr->moniker, hash_map<object_proto_type, object_register_params>() });

                    // Call PluginMain
                    PluginStatus status = on_module_load(module);

                    if (status != PluginStatus::Ok)
                        throw DynamicModuleLoadException();

                    // Insert the module in the map
                    _module_map.insert({ hdr->moniker, module });
                    _module_moniker_map.insert({ plugin_module_name, hdr->moniker });

                    // Pop the info header off the stack
                    _module_init_stack.pop();

                    // Get a handle to the plugin
                    handle = get_handle(hdr->moniker);
                }
                catch (DynamicModuleLoadException& ex)
                {
                    if (!_module_init_stack.empty())
                        _module_init_stack.pop();

                    std::string what = "PluginArbiter could not load module " + plugin_module_name + ". " + ex.what();

                    throw PluginArbiterException(what.c_str(), PluginStatus::ModuleLoadError);
                }
            }
            else
                handle = get_handle(moniker_itr->second);

            return handle;
        }

        void PluginArbiter::load_all(std::string base_path)
        {
            auto plugin_list = get_files(base_path, "so");

            for (auto& plugin : plugin_list)
                load(base_path, to_short_module_name(plugin));
        }

        PluginArbiter::Handle PluginArbiter::get_handle(std::string moniker)
        {
            std::lock_guard<std::recursive_mutex> __lock(_map_shared_mtx);

            Handle handle;

            // Check if the plugin is loaded
            if (_module_map.find(moniker) != _module_map.end())
            {
                handle = Handle(moniker, _handle_counter, get_instance());
                _handle_counter++;
            }
            else
                throw PluginArbiterException("Cannot create a handle to a non-existing plugin");

            return handle;
        }

        void PluginArbiter::unload_all()
        {
            std::lock_guard<std::recursive_mutex> __lock(_map_shared_mtx);

            std::vector<std::string> moniker_list;

            for (auto entry : _module_map)
                moniker_list.push_back(entry.first);
            for (auto const& moniker : moniker_list)
                unload_impl(moniker);

            _handle_counter = 0;
        }

        bool PluginArbiter::unload_impl(std::string moniker)
        {
            std::lock_guard<std::recursive_mutex> __lock(_map_shared_mtx);

            auto entry = _module_map.find(moniker);
            bool ok = true;

            if (entry == _module_map.end())
                ok = false;
            else
            {
                // Clear the associated entry in the object registration map
                if (_object_registration_map.find(moniker) != _object_registration_map.end())
                    _object_registration_map.erase(moniker);

                // Clear the associated entries
                _module_map.erase(entry);
                _module_moniker_map.erase(to_short_module_name(entry->second->name()));
            }

            return ok;
        }

        bool PluginArbiter::unload(Handle const& handle)
        {
            return unload_impl(handle.moniker());
        }

        PluginStatus PluginArbiter::on_module_load(std::shared_ptr<DynamicModule> module)
        {
            std::lock_guard<std::recursive_mutex> __lock(_map_shared_mtx);

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
        { }

        PluginArbiter::Handle::Handle(std::string moniker, uint32_t handle_id, std::shared_ptr<PluginArbiter> arbiter) noexcept
            : _moniker(std::move(moniker))
            , _handle_id(handle_id)
            , _arbiter(std::move(arbiter))
        { }

        PluginArbiter::Handle::Handle(Handle const& rhs) { }

        PluginArbiter::Handle::Handle(Handle&& rhs) noexcept
            : _moniker(std::move(rhs._moniker))
            , _handle_id(rhs._handle_id)
            , _arbiter(std::move(rhs._arbiter))
            , _obj_alloc_list(std::move(rhs._obj_alloc_list))
            , _obj_type_map(std::move(rhs._obj_type_map))
        {
            // Invalidate the handle id of the moved handle
            rhs._handle_id = _handle_id_invalid;
        }

        PluginArbiter::Handle& PluginArbiter::Handle::operator=(PluginArbiter::Handle &&rhs) noexcept
        {
            _moniker = std::move(rhs._moniker);
            _handle_id = rhs._handle_id;
            _arbiter = std::move(rhs._arbiter);
            _obj_alloc_list = std::move(rhs._obj_alloc_list);
            _obj_type_map = std::move(rhs._obj_type_map);

            // Invalidate the handle if of the moved handle
            rhs._handle_id = _handle_id_invalid;
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
