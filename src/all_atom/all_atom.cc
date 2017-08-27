#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <all_atom/atom.h>

namespace py = pybind11;

PYBIND11_PLUGIN(all_atom) {
    py::module m("all_atom", "rnamake's 3D structure lib");

    py::class_<Atom>(m, "Atom")
            .def(py::init<const String &, const Point &>())
            .def("get_name", &Atom::get_name);

    return m.ptr();
}
