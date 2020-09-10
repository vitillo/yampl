/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   Globals.h
 * @brief  YAMPL globals
 */


#ifndef YAMPL_GLOBALS_H
#define YAMPL_GLOBALS_H

#include <string>

#define DEFAULT_ID get_default_id()

namespace yampl
{
    std::string& get_default_id();
}

#endif // YAMPL_GLOBALS_H
