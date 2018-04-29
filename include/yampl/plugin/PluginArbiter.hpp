/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PluginArbiter.hpp
 * @brief  The file contains structures and procedures required to create, load and manage Yampl plugins
 */

#ifndef YAMPL_PLUGINARBITER_HPP
#define YAMPL_PLUGINARBITER_HPP

#include <cstdint>
#include <scy/pluga/pluga.h>

#if __GNUC__ >= 4
    #define DECLSPEC_EXPORT __attribute__((visibility ("default")))
    #define DECLSPEC_IMPORT __attribute__((visibility ("default")))
    #define DECLSPEC_LOCAL  __attribute__((visibility ("local")))
#else
    #define DECLSPEC_EXPORT
    #define DECLSPEC_IMPORT
    #define DECLSPEC_LOCAL
#endif

namespace yampl
{
    namespace plugin
    {
        /******************* Constants **/
        constexpr uint32_t YAMPL_PLUGIN_ARBITER_ABI_VER = 1U;

        using GetPluginDelegate = scy::pluga::IPlugin* (*)();
        /**
         * @brief Enumeration for plugin types
         */
        enum class PluginType
        {
            YAMPL_SOCKET_BACKEND
        };

        struct PluginHeader
        {
            public:
                uint32_t abi_version;
                char const* file_name;
                char const* class_name;
                PluginType   plugin_type;
                char const* plugin_name;
                char const* plugin_version;
                /* The delegate function that builds and returns the singleton instance */
                GetPluginDelegate get;
        };

        #define YAMPL_PLUGIN(_ClsName, _Id, _Ver)                                       \
        extern "C" {                                                                   \
            DECLSPEC_EXPORT scy::pluga::IPlugin* getPlugin()                            \
            {                                                                           \
                static _ClsName singleton;                                             \
                return &singleton;                                                     \
            }                                                                           \
            DECLSPEC_EXPORT yampl::plugin::PluginHeader header = {                      \
                YAMPL_PLUGIN_ARBITER_ABI_VER,                                           \
                __FILE__,                                                               \
                #_ClsName,                                                              \
                pluginName,                                                             \
                pluginVersion,                                                          \
                getPlugin,                                                              \
            };                                                                          \
        }
    }
}

#endif // YAMPL_PLUGINARBITER_HPP
