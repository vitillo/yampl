/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   plugin_object.hpp
 * @brief  Dummy plugin example
 */


#ifndef YAMPL_PLUGIN_OBJECT_HPP
#define YAMPL_PLUGIN_OBJECT_HPP

#include "yampl/plugin/IObject.hpp"

#include <cstdint>
#include <iostream>

class DummyObject : public IObject
{
    public:
        static const uint32_t __OBJECT_VERSION = 0UL;

        DummyObject() {
            std::cout << "[PLUGIN][+] DummyObject::DummyObject()" << std::endl;
        }

        ~DummyObject() {
            std::cout << "[PLUGIN][+] DummyObject::~DummyObject()" << std::endl;
        }
};

#endif // YAMPL_PLUGIN_OBJECT_HPP
