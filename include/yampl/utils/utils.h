#ifndef YAMPL_UTILS_H
#define YAMPL_UTILS_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

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
     * Converts a full module name (ex. libyampl-shm.so) to the corresponding short name (ex. yampl-shm)
     *
     * @param name the full module name
     * @return the short module name
     */
    std::string to_short_module_name(std::string name);

    /**
     * Converts a short module name (ex. yampl-shm) to the corresponding full name (ex. libyampl-shm.so)
     *
     * @param name the short module name
     * @return the full module name
     */
    std::string to_full_module_name(std::string name);

    /**
     * Returns the value of the YAMPL_INSTALL_PREFIX environment variable
     *
     * @return the plugin base directory
     */
    std::string get_plugin_base_dir();

    /**
     * Returns a list of files contained in a given directory
     *
     * @param directory the directory where to look for files
     * @param  ext_filter the file extension filter
     * @return a list of filenames
     */
    std::vector<std::string> get_files(std::string directory, std::string ext_filter = "");
}

#endif
