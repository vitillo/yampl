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
        DynamicModule::DynamicModule(std::string dyn_mod_pfx, std::string dyn_mod_name, BindingPolicy policy)
            : _module_name(dyn_mod_name)
            , _module_prefix(dyn_mod_pfx)
            , _module_handle(nullptr)
            , _free(false)
        {
            std::string module_full_path =_module_prefix;
            module_full_path.append(_module_name);

            // Try open the library
            _module_handle = dlopen(module_full_path.c_str(), static_cast<int>(policy));

            // Did dlopen fail?
            if (_module_handle == nullptr)
                throw DynamicModuleLoadException(dlerror());
            // Clear errors
            dlerror();
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
    }
}