/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   plugin_main.cpp
 * @brief  Dummy plugin example
 */

#include "yampl/plugin/PluginApi.h"
#include "plugin_object.hpp"

#include <cstdint>
#include <iostream>

/*********************************** Plugin Info **/
#define PLUGIN_MONIKER "yampl-dummy" "\0"
#define PLUGIN_VER_MAJOR (uint8_t)1
#define PLUGIN_VER_MINOR (uint8_t)0

hook_exec_status PluginMain(pplugin_init_frame frame);

YAMPL_PLUGIN(PLUGIN_MONIKER, PLUGIN_VER_MAJOR, PLUGIN_VER_MINOR)

/*********************************** Code **/
static plugin_init_frame _gbl_init_frame;

hook_exec_status PluginMain(pplugin_init_frame frame)
{
    hook_exec_status status = HOOK_STATUS_SUCCESS;

    if (frame == nullptr)
    {
        status = HOOK_STATUS_FAILURE;
        std::cout << "[PLUGIN][!] PluginMain failed" << std::endl;
    }
    else
    {
        _gbl_init_frame = *frame;

        std::cout << "[PLUGIN][+] PluginMain executed successfully" << std::endl;

        // Register dummy objects
        object_register_params params;
        params.obj_type = OBJ_PROTO_UNKOWN;
        params.obj_version = DummyObject::__OBJECT_VERSION;
        params.hk_create = nullptr;
        params.hk_destroy = nullptr;

        frame->hk_register(params);
    }

    return status;
}