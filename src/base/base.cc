//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>

//rnamake headers
#include <base/file_io.h>

namespace py = pybind11;

PYBIND11_PLUGIN(base_util) {
    py::module m("base_util", "rnamake's most basic classes and functions");

    m.def("get_lines_from_file", &get_lines_from_file);

    return m.ptr();

}
