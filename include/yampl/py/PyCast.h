/**
 * @author Ayoub Chouak (a.chouak@protonmail.com)
 * @file   PyCast.h
 * @brief  Type casters for Python bindings
 */


#ifndef YAMPL_PYCAST_H
#define YAMPL_PYCAST_H

#include "yampl/ISocketFactory.h"
#include "yampl/Exceptions.h"

#include <pybind11/pybind11.h>
#include <string>

#if PY_MAJOR_VERSION < 3
    #define PyVariantString_FromString PyString_FromString
#else
    #define PyVariantString_FromString PyUnicode_FromString
#endif

namespace py_ = pybind11;

namespace yampl
{
    namespace py
    {
       /**
        * @brief Raw byte buffer wrapper class for c++/python interop
        */
        class byte_buffer
        {
            public:
                using data_type = uint8_t;

                byte_buffer() = default;

                byte_buffer(uint8_t*, ssize_t);
                
               /**
                * Getters and setters
                */
                uint8_t* setBuffer(uint8_t*);
                size_t setSize(size_t);

                uint8_t* getBuffer() const;
                size_t getSize() const;

               /**
                * @brief Releases the buffer
                */
                void release();
            protected:
                data_type* buffer;
                ssize_t    size; //!< Actual size of the buffer
        };

       /**
        * @brief Python pickling helper class
        */
        class pickler
        {
            private:
                static py_::object get_pickle_module();
            public:
               /**
                * @param obj object to serialize as a string
                * @return the pickled representation of the object as a string
                */
                static py_::object dumps(py_::object obj);

               /**
                * @param obj string to deserialize to its underlying object
                * @return the pickled object hierarchy from `obj`
                */
                static py_::object loads(py_::object obj);

               /**
                * @param buffer string to deserialize to its underlying object
                * @return the pickled object hierarchy from `buffer`
                */
                static py_::object loads(byte_buffer const& buffer);
        };
    }
}

namespace pybind11
{
    namespace detail
    {
       /**
        * @brief Template specialization for byte_buffer <=> py::bytes type_caster
        */
        template <> struct type_caster<yampl::py::byte_buffer>
        {
            public:
                PYBIND11_TYPE_CASTER(yampl::py::byte_buffer, _("byte_buffer"));

                /**
                 * Conversion part 1 (Python->C++): convert a PyObject into a byte_buffer
                 * instance or return false upon failure. The second argument
                 * indicates whether implicit conversions should be applied.
                 *
                 * @arg h PyObject handle
                 */
                bool load(handle h, bool)
                {
                    auto py_str = PyBytes_AsString(h.ptr());
                    value = yampl::py::byte_buffer(reinterpret_cast<uint8_t*>(py_str), py_::len(h));

                    /* Ensure return code was OK (to avoid out-of-range errors etc) */
                    return !(value.getBuffer() == nullptr && !PyErr_Occurred());
                }

                /**
                 * Conversion part 2 (C++ -> Python): convert an byte_buffer instance into
                 * a Python object. The second and third arguments are used to
                 * indicate the return value policy and parent object (for
                 * ``return_value_policy::reference_internal``) and are generally
                 * ignored by implicit casters.
                 *
                 * @src src the byte_buffer to cast
                 */
                static handle cast(yampl::py::byte_buffer src, return_value_policy /* policy */, handle /* parent */)
                {
                    PyObject* bytes = PyBytes_FromStringAndSize(reinterpret_cast<const char*>(src.getBuffer()), src.getSize());
                    auto bytes_obj = reinterpret_steal<object>(py_::handle(bytes));
                    auto tuple = py_::make_tuple(src.getSize(), bytes_obj);

                    tuple.inc_ref();
                    return tuple;
                }
        };
    }
}

#endif //YAMPL_PYCAST_H
