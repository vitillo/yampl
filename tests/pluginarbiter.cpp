/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   pluginarbiter.cpp
 * @brief  Test case for the PluginArbiter class
*/

#include "yampl/plugin/PluginArbiter.hpp"
#include "yampl/Exceptions.h"

#include <memory>
#include <iostream>

using namespace yampl::plugin;

int main(int argc, char** argv)
{
    int status = 0;
    std::shared_ptr<PluginArbiter> arbiter = PluginArbiter::get_instance();

    /*************** PluginArbiter::load **/
    try
    {
        PluginArbiter::Handle handle = arbiter->load("..", "yampl");
        std::cout << "[1] Module loaded successfully" << std::endl;
    }
    catch (PluginArbiterException ex) {
        status = -1;
    }

    return status;
}