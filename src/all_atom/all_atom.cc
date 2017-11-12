#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <all_atom/atom.h>
#include <all_atom/residue_type.h>
#include <all_atom/residue_type_set.h>

namespace all_atom {
namespace py = pybind11;

PYBIND11_PLUGIN(all_atom) {
    py::module m("all_atom", "rnamake's 3D structure lib");

    py::class_<Atom, std::shared_ptr<Atom>>(m, "Atom")
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

    py::class_<ResidueType, std::shared_ptr<ResidueType>>(m, "ResidueType")
            .def(py::init<String const &, StringIntMap const &, SetType, Strings const &>())
            .def("is_valid_atom_name", &ResidueType::is_valid_atom_name)
            .def("get_atom_index", &ResidueType::get_atom_index)
            .def("is_valid_residue_name", &ResidueType::is_valid_residue_name)
            .def("get_name", &ResidueType::get_name)
            .def("get_set_type", &ResidueType::get_set_type);

    py::class_<ResidueTypeSet, std::shared_ptr<ResidueTypeSet>>(m, "ResidueTypeSet")
            .def(py::init<>())
            .def("get_residue_type", &ResidueTypeSet::get_residue_type)
            .def("contains_residue_type", &ResidueTypeSet::contains_residue_type);

    return m.ptr();
}

}
