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

#define PLUGIN_HDR_EXPORT _YAMPL_PLUGIN_HDR
#define PLUGIN_HDR_EXPORT_SYM "_YAMPL_PLUGIN_HDR"

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

/* Maximum length of the plugin moniker */
#define __HDR_MONIKER_MAX_LEN 32

typedef void* opaque_ptr;

/**
 * @brief Enumeration that indicates to the PluginArbiter the underlying interface implemented by a plugin object
 */
typedef enum object_proto_type_
{
    OBJ_PROTO_UNKNOWN = 0UL,
    OBJ_PROTO_SK_FACTORY, // ISocketFactory
} object_proto_type, *pobject_proto_type;

typedef struct object_init_params_
{
    object_proto_type type; // Requested object
} object_init_params, *pobject_init_params;

typedef enum hook_exec_status_
{
    HOOK_STATUS_UNKNOWN = 0UL,
    HOOK_STATUS_SUCCESS,
    HOOK_STATUS_FAILURE,
} hook_exec_status, *phook_exec_status;

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
//typedef hook_exec_status (*HOOK_InvokeService)(app_service_type_ /* type */, opaque_ptr /* packed_params */);

/* Singly-linked list structure */
#define CONTAINING_RECORD(_rcr_ty, _entry) ((_rcr_ty*)((char*) _entry - (char*)&((_rcr_ty*)(NULL)->flink)))

typedef struct list_entry_
{
	struct list_entry_* flink;
	struct list_entry_* blink;
} list_entry, *plist_entry;

typedef struct plugin_init_frame_
{
    /* Plugin API version supported by the application */
    uint32_t api_version;

    /* Callback to register objects */
    HOOK_RegisterObject hk_register;
} plugin_init_frame, *pplugin_init_frame;

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

typedef struct object_register_params_
{
    /* Object type */
    object_proto_type obj_type;

    /* Object version */
    uint32_t obj_version;

    /* Lifecycle hook functions */
    HOOK_CreateObject   hk_create;
    HOOK_DestroyObject  hk_destroy;
} object_register_params, *pobject_register_params;

#ifdef __cplusplus
}
#endif

#endif // YAMPL_PLUGINAPI_H
