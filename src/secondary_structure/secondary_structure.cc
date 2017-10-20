//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

//rnamake headers
#include <secondary_structure/residue.h>

namespace py = pybind11;
namespace secondary_structure {

PYBIND11_PLUGIN(secondary_structure) {
    py::module m("secondary_structure", "rnamake's most basic classes and functions");

    // Residue Class
    py::class_<Residue, ResidueOP>(m, "Residue")
            .def("get_chain_id", &Residue::get_chain_id)
            .def("get_name", &Residue::get_name)
            .def("get_num", &Residue::get_num)
            .def("get_i_code", &Residue::get_i_code)
            .def("get_uuid", &Residue::get_uuid)
            .def("get_str", &Residue::get_str)
            .def("get_dot_bracket", &Residue::get_dot_bracket)
            .def("get_res_type", &Residue::get_res_type)
            .def("set_name", &Residue::set_name)
            .def(py::init<String>())
            .def(py::init<Residue>())
            .def(py::init<char, char, int, char, char, util::Uuid>());

    m.def("are_residues_equal", &are_residues_equal);

    return m.ptr();

}
}
