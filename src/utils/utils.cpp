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
            if (path[path.length() - 1] == '/')
                path.substr(0, path.length() - 1);

        return path;
    }

    std::string dir_path_normalize(std::string path)
    {
        if (path.length() > 0) {
            if (path[path.length() - 1] != '/')
                path.append("/");
        }
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
        char const* home_dir = std::getenv("HOME");
        std::string env_s = "";
        std::string home_dir_s = "";

        if (home_dir != nullptr)
            home_dir_s = std::string(home_dir);

        // If the environment variable isn't present, fall back to .yamplrc
        if (env == nullptr)
        {
            if (home_dir != nullptr)
            {
                std::ifstream yamplrc_in(dir_path_normalize(home_dir_s) + ".yamplrc", std::ios_base::in);

                if (yamplrc_in.is_open()) {
                    yamplrc_in >> env_s;

                    if (env_s.length() > 0)
                    {
                        // Expand ~ to $HOME if present
                        if (env_s[0] == '~')
                            env_s.replace(0, home_dir_s.length(), home_dir_s);
                        env_s = dir_path_normalize(env_s) + "plugins";
                    }
                }
            }
        }
        else
        {
            // Expand ~ to $HOME if present
            if (env_s[0] == '~')
                env_s.replace(0, home_dir_s.length(), home_dir_s);
            env_s = dir_path_normalize(std::string(env)) + "plugins";
        }

        return env_s;
    }
}