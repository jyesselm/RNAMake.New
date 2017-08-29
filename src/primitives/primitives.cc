//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

//rnamake headers
#include <primitives/residue.h>
#include <primitives/chain.h>
#include <primitives/structure.h>
#include <primitives/basepair.h>

namespace primitives {

namespace py = pybind11;

//PYBIND11_MAKE_OPAQUE(std::vector<Residue>);

PYBIND11_PLUGIN(primitives) {
    py::module m("primitives", "rnamake's primitive classes");

    // Uuid Class
    py::class_<Residue, std::shared_ptr<Residue>>(m, "Residue")
            .def("get_chain_id", &Residue::get_chain_id)
            .def("get_name", &Residue::get_name)
            .def("get_num", &Residue::get_num)
            .def("get_i_code", &Residue::get_i_code)
            .def("get_uuid", &Residue::get_uuid)
            .def("get_str", &Residue::get_str)
            .def(py::init<const char &, const int &, const char &, const char &, const Uuid &>());

    py::class_<Chain<Residue>, std::shared_ptr<Chain<Residue>>>(m, "Chain")
            .def(py::init<const ResidueOPs &>())
            .def("__iter__", [](const Chain<Residue> & c) { return py::make_iterator(c.begin(), c.end()); },
            py::keep_alive<0, 1>())
            .def("__len__", &Chain<Residue>::get_length)
            .def("get_first", &Chain<Residue>::get_first)
            .def("get_last",  &Chain<Residue>::get_last)
            .def("get_residue", &Chain<Residue>::get_residue)
            .def("get_length", &Chain<Residue>::get_length);

    typedef Structure<Chain<Residue>, Residue> StructureType;
    py::class_<StructureType, std::shared_ptr<StructureType>>(m, "Structure")
            .def(py::init<const ResidueOPs &, const Ints &>())
            .def("__iter__", [](const StructureType & s) {
                     return py::make_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
            .def("get_residue",
                 (ResidueOP const & (StructureType::*)(int) const) &StructureType::get_residue)
            .def("get_residue",
                 (ResidueOP const (StructureType::*)(int, char, char) const) &StructureType::get_residue)
            .def("get_residue",
                 (ResidueOP const (StructureType::*)(Uuid const &) const) &StructureType::get_residue)
            .def("get_res_index", &StructureType::get_res_index)
            .def("get_num_residues", &StructureType::get_num_residues)
            .def("get_num_chains", &StructureType::get_num_chains)
            .def("get_sequence", &StructureType::get_sequence);

    py::enum_<BasepairType>(m, "BasepairType")
            .value("WC", BasepairType::WC)
            .value("GU", BasepairType::GU)
            .value("NC", BasepairType::NC)
            .export_values();

    py::class_<Basepair, std::shared_ptr<Basepair>>(m, "Basepair")
            .def("get_bp_type", &Basepair::get_bp_type)
            .def("get_partner", &Basepair::get_partner)
            .def("get_res1_uuid", &Basepair::get_res1_uuid)
            .def("get_res2_uuid", &Basepair::get_res2_uuid)
            .def("get_uuid", &Basepair::get_uuid)
            .def(py::init<const Uuid &, const Uuid &, const Uuid &, BasepairType const &>());

    py::register_exception<ResidueException>(m, "ResidueException");
    py::register_exception<ChainException>(m, "ChainException");
    py::register_exception<StructureException>(m, "StructureException");
    py::register_exception<BasepairException>(m, "BasepairException");

    return m.ptr();
}

}