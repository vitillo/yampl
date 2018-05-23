/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   utils.cpp
 * @brief  Utilities for Yampl
 */

#include "yampl/utils/utils.h"

namespace yampl
{
    std::string dir_path_trim(std::string path)
    {
        if (path.length() > 0)
            if (*path.end() == '/')
                path.substr(0, path.length() - 1);

        return path;
    }

    std::string dir_path_normalize(std::string path)
    {
        if (path.length() > 0)
            if (*path.end() != '/')
                path.append("/");
            else
                path = "/";

        return path;
    }

    std::string to_full_module_name(std::string name)
    {
        return "lib" + name + ".so";
    }

    std::string get_plugin_base_dir() {
        char const* env = std::getenv("YAMPL_INSTALL_PREFIX");
        return env != nullptr ? dir_path_normalize(env) + "plugins" : "";
    }
}