/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   DynamicModule.hpp
 * @brief  The file contains the declaration of the DynamicModule type used by the PluginArbiter to load dynamic modules
 */

#ifndef YAMPL_DYNAMICMODULE_HPP
#define YAMPL_DYNAMICMODULE_HPP

#include <string>
#include <dlfcn.h>

namespace yampl
{
    namespace plugin
    {
        /** Forward declaration for DynamicModule**Exception classes */
        class DynamicModuleLoadException;
        class DynamicModuleSymbolException;

        /**
         * @brief Dynamic module minimal wrapper class for PluginArbiter. The class contains the
         * essential methods and parameters needed to load a dynamic module from the filesystem.
         *
         */
        class DynamicModule
        {
            using dl_handle = void*;

            private:
                std::string _module_name;
                std::string _module_prefix;
                dl_handle   _module_handle; //!< Opaque handle returned by dlopen
                bool _free; //!< Prevents the destructor from performing a spurious dlclose

                /**
                 * Internal non-throwing constructor
                 * @param dyn_mod_pfx the directory prefix where the module is stored
                 * @param dyn_mod_name the module name
                 * @param handle the dl_handle to the module
                 */
                DynamicModule(std::string dyn_mod_pfx, std::string dyn_mod_name, dl_handle handle) noexcept;

                /**
                 * Non-visible copy constructor
                 */
                DynamicModule(DynamicModule const&);
        public:
                /**
                 * @brief Binding policy for dlopen
                 */
                enum class BindingPolicy
                {
                    Lazy = RTLD_LAZY,
                    Now  = RTLD_NOW
                };

                enum class SymbolError
                {
                    NotFound, // dlsym() == NULL
                    Internal  // dlerror() != NULL
                };

                /**
                 * Default constructor
                 *
                 */
                DynamicModule() noexcept;

                /**
                 * Move constructor
                 *
                 * @param rhs The dynamic module whose handle and data are to be moved
                 */
                DynamicModule(DynamicModule&& rhs) noexcept;

                /**
                 *
                 * @param dyn_mod_pfx  the directory prefix where the module is stored
                 * @param dyn_mod_name the elided module name (ex. libyampl-shm => yampl-shm)
                 * @throws DynamicModuleLoadException
                 */
                static DynamicModule open(std::string dyn_mod_pfx, std::string dyn_mod_name, BindingPolicy policy = BindingPolicy::Lazy) /* throw(DynamicModuleLoadException) */;

                /**
                 * Performs cleanup of dynamically allocated memory (dlopen)
                 *
                 * @note The destructor assumes there can be no data race
                 */
                virtual ~DynamicModule();

                /**
                 * Attempts to resolve a symbol, throws an exception if dlerror() reports an error
                 *
                 * @tparam _Sym the type of the symbol
                 * @param sym the symbol identifier
                 * @return pointer to the resolved symbol
                 * @throws DynamicModuleSymbolException
                 */
                template<typename _Sym>
                _Sym* resolve_sym(std::string sym) /* throw(DynamicModuleSymbolException) */
                {
                    _Sym* opaque = reinterpret_cast<_Sym*>(dlsym(_module_handle, sym.c_str()));

                    // Assume the module doesn't have symbols pointing to NULL, so treat it as a "symbol not found" condition
                    if (opaque == nullptr)
                        throw DynamicModuleSymbolException("Symbol was not found", SymbolError::NotFound);

                    // Did dlsym fail?
                    char* dl_error = dlerror();

                    if (dl_error != nullptr)
                        throw DynamicModuleSymbolException(dl_error, SymbolError::Internal);

                    return opaque;
                }

                /**
                 * @brief Releases the resources allocated by dlopen and sets the flags accordingly
                 *
                 */
                void release();

                /************************* Properties **/
                std::string name() const;
                std::string prefix() const;
                bool free() const;
        };
    }
}

#endif // YAMPL_DYNAMICMODULE_HPP
