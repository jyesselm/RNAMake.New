//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

//rnamake headers
#include <base/types.h>
#include <base/file_io.h>
#include <base/simple_string.h>

namespace py = pybind11;

PYBIND11_PLUGIN(base) {
    py::module m("base", "rnamake's most basic classes and functions");

    // SimpleString Class
    py::class_<base::SimpleString>(m, "SimpleString")
            .def(py::init<const String &>())
            .def("get_str", &base::SimpleString::get_str)
            .def(py::self == py::self)
            .def(py::self != py::self);

    return m.ptr();

}
