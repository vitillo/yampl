/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   pluginarbiter.cpp
 * @brief  Test case for the PluginArbiter class
*/

#include "yampl/plugin/PluginArbiter.h"
#include "yampl/Exceptions.h"
#include "yampl/utils/utils.h"

#include <memory>
#include <iostream>

#define ARBITER_LOAD_ERROR (-1)
#define RESOLVE_SYM_ERROR  (-2)

#define TRACE(x) (std::cout << "[+] " + std::string((x)) << std::endl)
#define ERROR(x) (std::cerr << "[!] " + std::string((x)) << std::endl)

using namespace yampl::plugin;

int main(int argc, char** argv)
{
    int status = 0;
    std::shared_ptr<PluginArbiter> arbiter = PluginArbiter::get_instance();

    TRACE("[pluginarbiter]");

    try
    {
        /*************** PluginArbiter::load **/
        PluginArbiter::Handle handle = arbiter->load(yampl::get_plugin_base_dir(), "yampl-shm");
        TRACE("PluginArbiter loaded module " + handle.moniker() + " successfully");

        /*************** PluginArbiter::unload **/
        arbiter->unload(handle);
        TRACE("PluginArbiter unloaded " + handle.moniker() + " successfully");

        /*************** PluginArbiter::load_all **/
        arbiter->load_all(yampl::get_plugin_base_dir());
        TRACE("PluginArbiter unloaded all modules in " + yampl::get_plugin_base_dir());

        /*************** PluginArbiter::get_handle **/
        handle = arbiter->get_handle("yampl-pipe"); // Get a handle to yampl-pipe
        TRACE("Successfully obtained a handle to " + handle.moniker());

        /*************** PluginArbiter::unload_all **/
        arbiter->unload_all();
        TRACE("PluginArbiter unloaded all modules successfully");
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
