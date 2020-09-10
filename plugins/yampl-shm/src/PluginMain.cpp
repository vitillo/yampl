/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginMain.cpp
 * @brief  The file contains the entry point of yampl-shm plugin
 */

#include "yampl/plugin/PluginApi.h"
#include "SocketFactory.h"

/******************** Plugin Info **/
#define PLUGIN_MONIKER "yampl-shm"
#define PLUGIN_VER_MAJOR (uint8_t)1
#define PLUGIN_VER_MINOR (uint8_t)0

hook_exec_status PluginMain(plugin_init_frame* frame);

// Export
YAMPL_PLUGIN(PLUGIN_MONIKER, PLUGIN_VER_MAJOR, PLUGIN_VER_MINOR)

/*********************** Plugin Main **/
static plugin_init_frame _gbl_init_frame;

hook_exec_status PluginMain(plugin_init_frame* frame)
{
	hook_exec_status status;

	// Sanity check
	if (frame == nullptr) {
            status = HOOK_STATUS_FAILURE;
	}
	// Plugin API version check
	else if (frame->api_version != PLUGIN_API_VERSION) {
            status = HOOK_STATUS_FAILURE;
	}
	else
	{
	    // Store the frame for later retrieval
            _gbl_init_frame = *frame;

            // Register SocketFactory
            object_register_params reg_params;
            reg_params.obj_type   = OBJ_PROTO_SK_FACTORY;
            reg_params.hk_create  = yampl::shm::HOOK_CreateObject;
            reg_params.hk_destroy = yampl::shm::HOOK_DestroyObject;
            
	    hook_exec_status reg_status = frame->hk_register(&reg_params);

            if (reg_status == HOOK_STATUS_SUCCESS)
                status = HOOK_STATUS_SUCCESS;
            else
                status = HOOK_STATUS_FAILURE;
	}

	return status;
}
