/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   Globals.cpp
 * @brief  YAMPL globals
 */

#include "yampl/utils/Globals.h"

namespace yampl
{
    std::string& get_default_id() {
        static std::string default_id;
        return default_id;
    }
}