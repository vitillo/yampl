/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   DynamicModule.cpp
 * @brief  The file contains the definition of the DynamicModule structures
 */

#include "yampl/plugin/DynamicModule.hpp"
#include "yampl/Exceptions.h"

namespace yampl
{
    namespace plugin
    {
        DynamicModule::DynamicModule() noexcept
            : _module_prefix("")
            , _module_name("")
            , _module_handle(nullptr)
            , _free(true)
        {
            // EMPTY BODY
        }

        DynamicModule::DynamicModule(std::string dyn_mod_pfx, std::string dyn_mod_name, dl_handle handle) noexcept
            : _module_prefix(dyn_mod_pfx)
            , _module_name(dyn_mod_name)
            , _module_handle(handle)
            , _free(false)
        {
            // EMPTY BODY
        }

        DynamicModule::DynamicModule(DynamicModule const& rhs) {
            // EMPTY BODY
        }

        DynamicModule::DynamicModule(DynamicModule &&rhs) noexcept
            : _module_name(std::move(rhs._module_name))
            , _module_prefix(std::move(rhs._module_prefix))
            , _module_handle(rhs._module_handle)
            , _free(false)
        {
            // Render the moved DynamicModule unusable
            rhs._module_handle = nullptr;
            rhs._free = true;
        }

        DynamicModule DynamicModule::open(std::string dyn_mod_pfx, std::string dyn_mod_name, BindingPolicy policy)
        {
            std::string module_full_path = dyn_mod_pfx + dyn_mod_name;

            // Try open the library
            dl_handle handle = dlopen(module_full_path.c_str(), static_cast<int>(policy));

            // Did dlopen fail?
            if (handle == nullptr)
                throw DynamicModuleLoadException(dlerror());
            // Clear errors
            dlerror();

            return DynamicModule(std::move(dyn_mod_pfx), std::move(dyn_mod_name), handle);
        }

        DynamicModule::~DynamicModule()
        {
            if (!_free)
                release();
        }

        void DynamicModule::release()
        {
            dlclose(_module_handle);
            _free = true;
        }

        std::string DynamicModule::name() const {
            return _module_name;
        }

        std::string DynamicModule::prefix() const {
            return _module_prefix;
        }

        bool DynamicModule::free() const {
            return _free;
        }
    }
}