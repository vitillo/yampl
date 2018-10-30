/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginApi.h
 * @brief  The file contains the API for a YAMPL plugin
 */

#ifndef YAMPL_PLUGINAPI_H
#define YAMPL_PLUGINAPI_H

#include <stdint.h>

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

#define PLUGIN_HDR_EXPORT _YAMPL_PLUGIN_HDR
#define PLUGIN_HDR_EXPORT_SYM "_YAMPL_PLUGIN_HDR"

#define PLUGIN_API_VERSION 0UL

// Maximum length of the plugin moniker
#define __HDR_MONIKER_MAX_LEN 32

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Helper macro to declare plugins
 */
#define YAMPL_PLUGIN(_Moniker, _Ver_Maj, _Ver_Min) \
    extern "C" { \
        YAMPL_DECL_EXPORT plugin_info_hdr PLUGIN_HDR_EXPORT = { \
            _Moniker, \
            _Ver_Maj, \
            _Ver_Min, \
            PLUGIN_API_VERSION, \
            PluginMain \
        }; \
    }

typedef void* opaque_ptr;

/**
 * @brief Type of interface implemented by a plugin object
 */
typedef enum object_proto_type_
{
    OBJ_PROTO_UNKNOWN = 0UL, // IObject
    OBJ_PROTO_SK_FACTORY,    // ISocketFactory
} object_proto_type, *pobject_proto_type;

#define OBJECT_VERSION_ANY (uint32_t) -1

/**
 * @brief Struct used for the initialization of a plugin object
 */
typedef struct object_init_params_
{
    object_proto_type type; // Requested object
    uint32_t obj_version;   // Requested object version
} object_init_params, *pobject_init_params;

/**
 * @brief Status of the execution of a lifecycle hook procedure
 */
typedef enum hook_exec_status_
{
    HOOK_STATUS_UNKNOWN = 0UL,
    HOOK_STATUS_SUCCESS,
    HOOK_STATUS_FAILURE,
} hook_exec_status, *phook_exec_status;

/**
 * @brief Extended status
 */
typedef struct hook_exec_status_ex_
{
    hook_exec_status status;
    uint32_t status_code;
} hook_exec_status_ex, *phook_exec_status_ex;

struct plugin_init_frame_;
struct object_register_params_;
struct object_init_params_;

/*********************** Hook functions (Plugin side) **/
typedef opaque_ptr (*HOOK_CreateObject)(struct object_init_params_* /* params */);
typedef hook_exec_status (*HOOK_DestroyObject)(opaque_ptr /* handle */);
typedef hook_exec_status (*HOOK_PluginMain)(struct plugin_init_frame_* /* frame */);

/*********************** Hook functions (Application side) **/
typedef hook_exec_status (*HOOK_RegisterObject)(object_register_params_* /* params */);

/**
 * @brief Struct used for the initialization of a plugin
 */
typedef struct plugin_init_frame_
{
    /* Plugin API version supported by the application */
    uint32_t api_version;

    /* Callback to register objects */
    HOOK_RegisterObject hk_register;
} plugin_init_frame, *pplugin_init_frame;

/**
 * @brief Struct representing the information header
 *        of a plugin
 */
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
    HOOK_PluginMain hk_plugin_main;
} plugin_info_hdr, *pplugin_info_hdr;

/**
 * @brief Struct to register a plugin object
 */
typedef struct object_register_params_
{
    /* Object type */
    object_proto_type obj_type;

    /* Lifecycle hook functions */
    HOOK_CreateObject   hk_create;
    HOOK_DestroyObject  hk_destroy;
} object_register_params, *pobject_register_params;

#ifdef __cplusplus
}
#endif

#endif // YAMPL_PLUGINAPI_H
