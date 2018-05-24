/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   pluginarbiter.cpp
 * @brief  Test case for the PluginArbiter class
*/

#include "yampl/plugin/PluginArbiter.hpp"
#include "yampl/Exceptions.h"
#include "yampl/utils/utils.h"

#include <memory>
#include <iostream>

#define ARBITER_LOAD_ERROR -1
#define RESOLVE_SYM_ERROR  -2

#define TRACE(x) std::cout << "[+] " + std::string((x)) << std::endl
#define ERROR(x) std::cerr << "[!] " + std::string((x)) << std::endl

using namespace yampl::plugin;

int main(int argc, char** argv)
{
    int status = 0;
    std::shared_ptr<PluginArbiter> arbiter = PluginArbiter::get_instance();

    try
    {
        /*************** PluginArbiter::load **/
        PluginArbiter::Handle handle = arbiter->load(yampl::get_plugin_base_dir(), "yampl-shm", PluginArbiter::DiscoveryMode::Recurse);
        TRACE("PluginArbiter loaded the module successfully");

        /*************** PluginArbiter::Handle::resolve_sym **/
        handle.resolve_sym<void>(PLUGIN_HDR_EXPORT_SYM);
    }
    catch (PluginArbiterException const& ex) {
        status = ARBITER_LOAD_ERROR;
        ERROR(ex.what());
    }
    catch (DynamicModuleSymbolException const& ex){
        status = RESOLVE_SYM_ERROR;
        ERROR("Symbol resolution failed");
    }

    return status;
}