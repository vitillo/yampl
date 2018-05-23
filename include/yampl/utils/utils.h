#ifndef YAMPL_UTILS_H
#define YAMPL_UTILS_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>

namespace yampl
{
    template <typename T>
    std::string to_string(T value) {
      std::stringstream s;
      s << value;
      return s.str();
    }

    /**
     * Trims a directory path string by removing the ending slash if present
     *
     * @param path the directory path
     * @return trimmed directory path string
     */
    std::string dir_path_trim(std::string path);

    /**
     * Normalizes a directory path string by placing an ending slash if needed
     *
     * @param path the directory path
     * @return normalized directory path string
     */
    std::string dir_path_normalize(std::string path);

    /**
     * Converts an elided module name (ex. yampl-shm) to the corresponding full name (ex. libyampl-shm.so)
     * @param name the elided module name
     * @return the full module name
     */
    std::string to_full_module_name(std::string name);

    /**
     * Returns the value of the YAMPL_INSTALL_PREFIX environment variable
     *
     * @return the plugin base directory
     */
    std::string get_plugin_base_dir();
}

#endif
