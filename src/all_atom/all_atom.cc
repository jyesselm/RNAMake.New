#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <all_atom/atom.h>

namespace all_atom {
namespace py = pybind11;

PYBIND11_PLUGIN(all_atom) {
    py::module m("all_atom", "rnamake's 3D structure lib");

    py::class_<Atom>(m, "Atom")
            .def(py::init<base::SimpleStringCOP &, math::Point const &>())
            .def(py::init<String const &>())
            .def(py::init<Atom const &>())
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def("get_str", &Atom::get_str)
            .def("get_name", &Atom::get_name)
            .def("get_coords", &Atom::get_coords)
            .def("get_pdb_str", &Atom::get_pdb_str)
            .def("move", &Atom::move)
            .def("transform",
                 (void (Atom::*)(math::Matrix const &, math::Vector const &)) &Atom::transform)
            .def("transform",
                 (void (Atom::*)(math::Matrix const &, math::Vector const &, math::Point &)) &Atom::transform);

    return m.ptr();
}

}
