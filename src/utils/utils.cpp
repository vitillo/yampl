/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   utils.cpp
 * @brief  Utilities for Yampl
 */

#include "yampl/utils/utils.h"

#include <fstream>

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

    std::string get_plugin_base_dir()
    {
        char const* env = std::getenv("YAMPL_INSTALL_PREFIX");
        std::string env_s = "";

        // If the environment variable isn't present, fall back to .yamplrc
        if (env == nullptr)
        {
            char const* home_dir = std::getenv("HOME");

            if (home_dir != nullptr)
            {
                std::ifstream yamplrc_in(dir_path_normalize(std::string(home_dir)) + ".yamplrc", std::ios_base::in);

                if (yamplrc_in.is_open()) {
                    yamplrc_in >> env_s;
                    env_s = dir_path_normalize(env_s) + "plugins";
                }
            }
        }
        else
            env_s = dir_path_normalize(std::string(env)) + "plugins";

        return env_s;
    }
}