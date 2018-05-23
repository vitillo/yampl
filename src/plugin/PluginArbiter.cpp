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

        PluginArbiter::PluginArbiter()
        {
            // @todo: Implement
        }

        PluginArbiter::PluginArbiter(PluginArbiter const& rhs)
        {
            // @todo: Implement
        }

        PluginArbiter::~PluginArbiter()
        {
            unload_all();
        }

        PluginArbiter::Handle PluginArbiter::load(std::string base_path, std::string plugin_module_name, DiscoveryMode mode)
        {
            std::lock_guard<std::mutex> _guard0(_module_map_mtx);

            // Open the dynamic module
            std::shared_ptr<DynamicModule> module = nullptr;

            Handle handle;

            try
            {
                if (mode == DiscoveryMode::Standard)
                {
                    module = std::make_shared<DynamicModule>(DynamicModule::open(dir_path_normalize(base_path), to_full_module_name(plugin_module_name)));
                }
                else if (mode == DiscoveryMode::Recurse)
                {
                    std::string full_base_path = dir_path_normalize(base_path) + plugin_module_name;
                    std::cout << dir_path_normalize(full_base_path) + to_full_module_name(plugin_module_name) << std::endl;
                    module = std::make_shared<DynamicModule>(DynamicModule::open(dir_path_normalize(full_base_path), to_full_module_name(plugin_module_name)));
                }

                PluginStatus status = OnModuleLoad(module);

                if (status != PluginStatus::Ok)
                    throw DynamicModuleLoadException();

                // Insert the module in the map
                handle = Handle(module);
                _module_map.insert({ handle.moniker(), module });
            }
            catch (DynamicModuleLoadException& ex)
            {
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
            std::vector<std::string> moniker_list;

            for (auto entry : _module_map)
                moniker_list.push_back(entry.first);
            for (auto const& moniker : moniker_list)
                unload_impl(moniker);
        }

        bool PluginArbiter::unload_impl(std::string moniker)
        {
            auto entry = _module_map.find(moniker);
            bool ok = true;

            if (entry == _module_map.end())
                ok = false;
            else
            {
                entry->second->release();
                _module_map.erase(entry);
            }

            return ok;
        }

        bool PluginArbiter::unload(Handle const& handle)
        {
            return unload_impl(handle.moniker());
        }

        PluginStatus PluginArbiter::OnModuleLoad(std::shared_ptr<DynamicModule> module)
        {
//            plugin_init_frame init_frame;
            plugin_info_hdr*  info_hdr;

            // Read the plugin header
            info_hdr = module->resolve_sym<plugin_info_hdr>(PLUGIN_HDR_EXPORT_SYM);
            std::cout << "[+] Loading " << info_hdr->moniker << std::endl;

            return PluginStatus::Ok;
        }

        /**
         * PluginArbiter::Handle
         */
        PluginArbiter::Handle::Handle() noexcept
            : _module(nullptr)
        {

        }

        PluginArbiter::Handle::Handle(std::shared_ptr<DynamicModule> module) noexcept
            : _module(std::move(module))
        {

        }

        std::string PluginArbiter::Handle::moniker() const {
            return _module->resolve_sym<plugin_info_hdr>(PLUGIN_HDR_EXPORT_SYM)->moniker;
        }
    }
}