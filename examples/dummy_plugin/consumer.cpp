/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   consumer.cpp
 * @brief  Example of a consumer for the yampl-dummy plugin
*/

#include "yampl/plugin/PluginApi.h"
#include "yampl/plugin/DynamicModule.h"
#include "yampl/Exceptions.h"

#include <iostream>

#define STATUS_OK 0
#define STATUS_LOAD_EXCEPTION -1
#define STATUS_SYM_EXCEPTION  -2

using yampl::plugin::DynamicModule;
using yampl::plugin::DynamicModuleLoadException;
using yampl::plugin::DynamicModuleSymbolException;

hook_exec_status
_HOOK_RegisterObject(object_register_params* params)
{
    hook_exec_status status = HOOK_STATUS_SUCCESS;

    std::cout << "[+] Registering object of type ";

    switch (params->obj_type)
    {
        case OBJ_PROTO_UNKNOWN:
            std::cout << "OBJ_PROTO_UNKNOWN" << std::endl;
            break;
        case OBJ_PROTO_SK_FACTORY:
            std::cout << "OBJ_PROTO_SK_FACTORY" << std::endl;
            break;
    }

    return status;
}

int main(int argc, char** argv)
{
    int status = STATUS_OK;

    try
    {
        // Load the module
        DynamicModule dyn = DynamicModule::open("../lib/", "libyampl-dummy.so");
        std::cout << "[1] Module loaded successfully" << std::endl;

        // Parse the header
        pplugin_info_hdr info = dyn.resolve_sym<plugin_info_hdr>(PLUGIN_HDR_EXPORT_SYM);

        // Dump the information
        std::cout << "[2] Header parsed successfully" << std::endl;
        std::cout << "---------------------------" << std::endl;
        std::cout << "info->moniker = " << info->moniker << std::endl;
        std::cout << "info->version = " << info->u.version << std::endl;
        std::cout << "info->api_compat_version = " << info->api_compat_version << std::endl;
        std::cout << "info->hk_plugin_main = " << reinterpret_cast<opaque_ptr>(info->hk_plugin_main) << std::endl;
        std::cout << "---------------------------" << std::endl << std::endl;

        // Load the plugin
        plugin_init_frame frame;

	    // API Version and IObject registration hook
        frame.api_version = PLUGIN_API_VERSION;
        frame.hk_register = _HOOK_RegisterObject;

        // Call PluginMain with the constructed frame
        info->hk_plugin_main(&frame);
    }
    catch (DynamicModuleLoadException ex) {
        status = STATUS_LOAD_EXCEPTION;
    }
    catch (DynamicModuleSymbolException ex) {
        std::cout << ex.what() << std::endl;
        status = STATUS_SYM_EXCEPTION;
    }

    return status;
}
