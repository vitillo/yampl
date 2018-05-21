/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginApi.h
 * @brief  The file contains the API for developing Yampl plugins
 */

#ifndef YAMPL_PLUGINAPI_H
#define YAMPL_PLUGINAPI_H

#include <stdint.h>

#define PLUGIN_API_VERSION 0UL

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Visibility declaration specifiers
 */
#if defined(__GNUC__) && __GNUC__ >= 4
    #define YAMPL_DECL_EXPORT __attribute__((visibility ("default")))
    #define YAMPL_DECL_IMPORT __attribute__((visibility ("default")))
    #define YAMPL_DECL_LOCAL  __attribute__((visibility ("local")))
#elif defined(_MSC_VER)
    #define YAMPL_DECL_EXPORT __declspec(dllexport)
    #define YAMPL_DECL_IMPORT __declspec(dllimport)
    #define YAMPL_DECL_LOCAL
#else
    #define YAMPL_DECL_EXPORT
    #define YAMPL_DECL_IMPORT
    #define YAMPL_DECL_LOCAL
#endif

/**
 * @brief Helper macro to declare plugins
 */
#define YAMPL_PLUGIN(_Moniker, _Ver_Maj, _Ver_Min) \
    extern "C" { \
        YAMPL_DECL_EXPORT plugin_info_hdr _YAMPL_PLUGIN_HDR = { \
            .moniker = _Moniker, \
            .u.v.major = _Ver_Maj, \
            .u.v.minor = _Ver_Min, \
            .api_compat_version = PLUGIN_API_VERSION \
            .main_callback_fn = PluginMain \
        }; \
    }

/* Maximum length of the plugin moniker */
#define __HDR_MONIKER_MAX_LEN 32

typedef void* opaque_ptr;

typedef enum app_service_type_
{
    SVC_LOGGER_FACILITY,
} app_service_type, *papp_service_type;

typedef struct plugin_init_frame_
{
    /* Plugin API version supported by the application */
    uint32_t api_version;

} plugin_init_frame, *pplugin_init_frame;

/**
 * @brief Enumeration that indicates to the PluginArbiter the underlying interface implemented by a plugin object
 */
typedef enum object_proto_type_
{
    OBJ_PROTO_SK_FACTORY = 0UL, // ISocketFactory
} object_proto_type, *pobject_proto_type;

typedef struct object_init_params_
{
    object_proto_type type; // Requested object
} object_init_params, *pobject_init_params;

typedef enum hook_exec_status_
{
    HOOK_STATUS_SUCCESS,
    HOOK_STATUS_FAILURE,
} hook_exec_status, *phook_exec_status;

/*********************** Hook functions (Plugin side) **/
typedef opaque_ptr (*HOOK_CreateObject)(pobject_init_params /* params */);
typedef hook_exec_status (*HOOK_DestroyObject)(opaque_ptr /* handle */);
typedef hook_exec_status (*HOOK_PluginMain)(pplugin_init_frame /* frame */);

/*********************** Hook functions (Application side) **/
typedef hook_exec_status (*HOOK_RegisterObject)(object_proto_type /* type */);
typedef hook_exec_status (*HOOK_InvokeService)(app_service_type_ /* type */, opaque_ptr /* packed_params */);

typedef struct plugin_info_hdr_
{
    /* Moniker */
    char moniker[__HDR_MONIKER_MAX_LEN + 1];

    /* Plugin Version */
    union {
        struct {
            uint8_t major;
            uint8_t minor;
        } v;
        uint16_t version;
    } u;

    /* Plugin API version */
    uint32_t api_compat_version;

    /* PluginMain callback */
    HOOK_PluginMain main_callback_fn;
} plugin_info_hdr, *pplugin_info_hdr_;

typedef struct object_register_params_
{
    /* Object versiom */
    uint32_t obj_version;

    /* Lifecycle hook functions */
    HOOK_CreateObject   hk_create;
    HOOK_DestroyObject  hk_destroy;
    HOOK_RegisterObject hk_register;
};

#ifdef __cplusplus
}
#endif

#endif // YAMPL_PLUGINAPI_H
