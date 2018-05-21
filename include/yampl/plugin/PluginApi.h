/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginApi.h
 * @brief  The file contains the API for developing Yampl plugins
 */

#ifndef YAMPL_PLUGINAPI_H
#define YAMPL_PLUGINAPI_H

#include <stdint.h>

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
 * @brief Declares the specified class as a Yampl plugin and registers it with a unique moniker and a version
 */
#define YAMPL_PLUGIN(_ClsName, _Id, _Ver)                                               \
        extern "C" {                                                                   \
            YAMPL_DECL_EXPORT yampl::plugin::IPluginProto* getPlugin()                  \
            {                                                                           \
                static _ClsName singleton;                                             \
                return &singleton;                                                     \
            }                                                                           \
            YAMPL_DECL_EXPORT yampl::plugin::PluginHeader header = {                    \
                YAMPL_PLUGIN_ARBITER_ABI_VER,                                           \
                __FILE__,                                                               \
                #_ClsName,                                                              \
                pluginName,                                                             \
                pluginVersion,                                                          \
                getPlugin,                                                              \
            };                                                                          \
        }

#define __HDR_MONIKER_MAX_LEN 32

typedef void* opaque_ptr;

/*********************** Hook functions **/
typedef opaque_ptr (*HOOK_CreateObject)(pobject_init_params /* params */);
typedef hook_exec_status (*HOOK_DestroyObject)(opaque_ptr /* handle */);
typedef hook_exec_status (*HOOK_RegisterObject)(object_proto_type /* type */, )

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
    uint32_t api_version;
} plugin_info_hdr, *pplugin_info_hdr_;

/**
 * @brief Enumeration that indicates to the PluginArbiter the underlying implementation language used for a plugin object
 */
typedef enum object_impl_lang_
{
   OBJ_IMPL_C,
   OBJ_IMPL_CPP
} object_impl_lang, *pobject_impl_lang;

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

typedef struct object_register_params_
{
    /* Plugin API version */
    uint32_t api_version;

    /* Lifecycle hook functions */
    HOOK_CreateObject   hk_create;
    HOOK_DestroyObject  hk_destroy;
    HOOK_RegisterObject hk_register;

    /* Implementation language */
    object_impl_lang impl_lang;
};

typedef enum hook_exec_status_
{
    HOOK_STATUS_SUCCESS,
    HOOK_STATUS_FAILURE,
} hook_exec_status, *phook_exec_status;

#ifdef __cplusplus
}
#endif

#endif // YAMPL_PLUGINAPI_H
