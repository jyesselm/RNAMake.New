//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

//rnamake headers
#include <util/uuid.h>
#include <util/bead.h>

namespace py = pybind11;
namespace util {

PYBIND11_PLUGIN(util) {
    py::module m("util", "rnamake's utility classes");

    // Uuid Class
    py::class_<Uuid>(m, "Uuid")
            .def(py::init<>())
            .def(py::self == py::self)
            .def(py::self != py::self);

    py::enum_<BeadType>(m, "BeadType")
            .value("PHOS",  BeadType::PHOS)
            .value("SUGAR", BeadType::SUGAR)
            .value("BASE",  BeadType::BASE)
            .export_values();

    return m.ptr();
}

}