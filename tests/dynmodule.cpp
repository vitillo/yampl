/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   dynmodule.cpp
 * @brief  Test case for the DynamicModule class
*/

#include "yampl/plugin/DynamicModule.h"
#include "yampl/Exceptions.h"

#include <iostream>

#define TRACE(x) (std::cout << "[+] " + std::string((x)) << std::endl)
#define ERROR(x) (std::cerr << "[!] " + std::string((x)) << std::endl)

using yampl::plugin::DynamicModule;
using yampl::plugin::DynamicModuleLoadException;
using yampl::plugin::DynamicModuleSymbolException;

int main(int argc, char** argv)
{
    int status = 0;

    TRACE("[dynmodule]");

    /*************** DynamicModule::open **/
    try
    {
        DynamicModule dyn = DynamicModule::open("../", "libyampl.so");
        TRACE("Module loaded successfully");
    }
    catch (DynamicModuleLoadException& ex) {
        status = -1;
    }

    /*************** Move semantics **/
    try
    {
        DynamicModule dyn = DynamicModule::open("../", "libyampl.so");
        DynamicModule dyn_ = std::move(dyn);

        TRACE("Module loaded successfully");

        if (!dyn.free())
            status = -1;
        else
            TRACE("Move semantics are correct");
    }
    catch (DynamicModuleLoadException& ex) {
        status = -1;
    }

    return status;
}