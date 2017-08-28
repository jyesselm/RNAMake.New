//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>

//rnamake headers
#include <util/uuid.h>

namespace py = pybind11;

PYBIND11_PLUGIN(util) {
    py::module m("util", "rnamake's utility classes");

    // Uuid Class
    py::class_<Uuid>(m, "Uuid")
            .def(py::init<>())
            .def(py::self == py::self)
            .def(py::self != py::self);

    return m.ptr();
}