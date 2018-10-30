/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   utils.cpp
 * @brief  Utilities for Yampl
 */

#include "yampl/utils/utils.h"

#include <fstream>
#include <iostream>
#include <dirent.h>

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

    std::string to_short_module_name(std::string name)
    {
        std::size_t idx0 = name.find("lib");
        std::size_t idx1 = name.find(".so");

        if (idx0 == 0 && idx1 == name.length() - 3) {
            name = name.substr(3, name.length() - 6);
        }

        return name;
    }

    std::string to_full_module_name(std::string name)
    {
        return "lib" + name + ".so";
    }

    std::string get_plugin_base_dir()
    {
        char const* pfx = std::getenv("YAMPL_INSTALL_PREFIX");
        char const* home_dir = std::getenv("HOME");
        std::string pfx_s = "";
        std::string home_dir_s = "";

        if (home_dir != nullptr) {
            home_dir_s = dir_path_trim(std::string(home_dir));
        }

        // If the environment variable isn't present, fall back to .yamplrc
        if (pfx == nullptr)
        {
            if (home_dir != nullptr)
            {
                std::ifstream yamplrc_in(dir_path_normalize(home_dir_s) + ".yamplrc", std::ios_base::in);

                if (yamplrc_in.is_open()) {
                    yamplrc_in >> pfx_s;

                    if (pfx_s.length() > 0)
                    {
                        // Expand ~ to $HOME if present
                        if (pfx_s[0] == '~') {
                            std::string pfx_s_ = pfx_s.substr(1);
                            pfx_s = home_dir_s + pfx_s_;

                        }

                        pfx_s = dir_path_normalize(pfx_s) + "lib/plugins";
                    }
                }
            }
        }
        else
        {
            // Expand ~ to $HOME if present
            if (pfx_s[0] == '~') {
                std::string pfx_s_ = pfx_s.substr(1);
                pfx_s = home_dir_s + pfx_s_;
            }

            pfx_s = dir_path_normalize(std::string(pfx)) + "lib/plugins";
        }

        return pfx_s;
    }

    std::vector<std::string> get_files(std::string directory, std::string ext_filter)
    {
        std::vector<std::string> file_list;
        DIR* dir = opendir(directory.c_str());
        dirent* dir_entry = readdir(dir);

        while (dir_entry != nullptr)
        {
            if (dir_entry->d_type == DT_REG)
            {
                std::string file_name = dir_entry->d_name;

                if (ext_filter.length() > 0) {
                    if (file_name.find(ext_filter, (file_name.length() - ext_filter.length())) != std::string::npos)
                        file_list.push_back(file_name);
                }
                else
                    file_list.push_back(file_name);
            }

            dir_entry = readdir(dir);
        }

        closedir(dir);

        return file_list;
    }
}