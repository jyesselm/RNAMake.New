//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

//rnamake headers
#include <secondary_structure/residue.h>
#include <secondary_structure/chain.h>

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

    py::class_<Chain, ChainOP>(m, "Chain")
            .def(py::init<ResidueOPs const &>())
            .def(py::init<Chain const &>())
            .def(py::init<String const &>())
            .def("__iter__", [](const Chain & c) { return py::make_iterator(c.begin(), c.end()); },
                 py::keep_alive<0, 1>())
            .def("__len__", &Chain::get_length)
            .def("get_first", &Chain::get_first)
            .def("get_last",  &Chain::get_last)
            .def("get_residue", &Chain::get_residue)
            .def("get_length", &Chain::get_length)
            .def("get_dot_bracket", &Chain::get_dot_bracket)
            .def("get_str", &Chain::get_str)
            .def("get_sequence", &Chain::get_sequence);

    py::register_exception<ResidueException>(m, "ResidueException");
    py::register_exception<ChainException>(m, "ChainException");

    m.def("are_residues_equal", &are_residues_equal);
    m.def("are_chains_equal", &are_chains_equal);

    return m.ptr();

}
}
