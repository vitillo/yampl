/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   dynmodule.cpp
 * @brief  Test case for the DynamicModule class
*/

#include "yampl/plugin/DynamicModule.hpp"
#include "yampl/Exceptions.h"

#include <iostream>

using yampl::plugin::DynamicModule;
using yampl::plugin::DynamicModuleLoadException;
using yampl::plugin::DynamicModuleSymbolException;

int main(int argc, char** argv)
{
    int status = 0;

    /*************** DynamicModule::open **/
    try
    {
        DynamicModule dyn = DynamicModule::open("..", "yampl");
        std::cout << "[1] Module loaded successfully" << std::endl;
    }
    catch (DynamicModuleLoadException ex) {
        status = -1;
    }

    /*************** Move semantics **/
    try
    {
        DynamicModule dyn = DynamicModule::open("..", "yampl");
        DynamicModule dyn_ = std::move(dyn);
        std::cout << "[2] Module loaded successfully" << std::endl;

        if (!dyn.free())
            status = -1;
        else
            std::cout << "[3] Move semantics are correct" << std::endl;
    }
    catch (DynamicModuleLoadException ex) {
        status = -1;
    }

    return status;
}