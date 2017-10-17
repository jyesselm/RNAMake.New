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
#include <primitives/rna_structure.h>

namespace primitives {

namespace py = pybind11;

//PYBIND11_MAKE_OPAQUE(std::vector<Residue>);

PYBIND11_PLUGIN(primitives) {
    py::module m("primitives", "rnamake's primitive classes");

    // Uuid Class
    py::class_<PrimitiveResidue, PrimitiveResidueOP>(m, "Residue")
            .def("get_chain_id", &PrimitiveResidue::get_chain_id)
            .def("get_name", &PrimitiveResidue::get_name)
            .def("get_num", &PrimitiveResidue::get_num)
            .def("get_i_code", &PrimitiveResidue::get_i_code)
            .def("get_uuid", &PrimitiveResidue::get_uuid)
            .def("get_str", &PrimitiveResidue::get_str)
            .def(py::init<const char &, const int &, const char &, const char &, const util::Uuid &>());

    py::class_<PrimitiveChain, PrimitiveChainOP>(m, "Chain")
            .def(py::init<const PrimitiveResidueOPs &>())
            .def("__iter__", [](const PrimitiveChain & c) { return py::make_iterator(c.begin(), c.end()); },
            py::keep_alive<0, 1>())
            .def("__len__", &PrimitiveChain::get_length)
            .def("get_first", &PrimitiveChain::get_first)
            .def("get_last",  &PrimitiveChain::get_last)
            .def("get_residue", &PrimitiveChain::get_residue)
            .def("get_length", &PrimitiveChain::get_length);

    py::class_<PrimitiveStructure, PrimitiveStructureOP>(m, "Structure")
            .def(py::init<const ResidueOPs &, const Ints &>())
            .def("__iter__", [](const PrimitiveStructure & s) {
                     return py::make_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
            .def("get_residue",
                 (ResidueOP const & (PrimitiveStructure::*)(int) const) &PrimitiveStructure::get_residue)
            .def("get_residue",
                 (ResidueOP const & (PrimitiveStructure::*)(int, char, char) const) &PrimitiveStructure::get_residue)
            .def("get_residue",
                 (ResidueOP const & (PrimitiveStructure::*)(util::Uuid const &) const) &PrimitiveStructure::get_residue)
            .def("get_res_index", &PrimitiveStructure::get_res_index)
            .def("get_num_residues", &PrimitiveStructure::get_num_residues)
            .def("get_num_chains", &PrimitiveStructure::get_num_chains)
            .def("get_sequence", &PrimitiveStructure::get_sequence)
            .def("get_chains", &PrimitiveStructure::get_chains);

    py::enum_<BasepairType>(m, "BasepairType")
            .value("WC", BasepairType::WC)
            .value("GU", BasepairType::GU)
            .value("NC", BasepairType::NC)
            .export_values();

    py::class_<Basepair, std::shared_ptr<Basepair>>(m, "Basepair")
            .def("get_bp_type", &Basepair::get_bp_type)
            .def("get_partner", &Basepair::get_partner)
            .def("get_name", &Basepair::get_name)
            .def("get_res1_uuid", &Basepair::get_res1_uuid)
            .def("get_res2_uuid", &Basepair::get_res2_uuid)
            .def("get_uuid", &Basepair::get_uuid)
            .def(py::init<const util::Uuid &, const util::Uuid &, const util::Uuid &,
                    BasepairType const &, base::SimpleStringOP const &>());

    py::class_<PrimitiveRNAStructure, PrimitiveRNAStructureOP>(m, "RNAStructure")
            .def(py::init<const PrimitiveStructureOP &, const BasepairOPs &, const BasepairOPs &,
                          const base::SimpleStringOPs &, const base::SimpleStringOP &>())
            .def("__iter__", [](const PrimitiveRNAStructure & s) {
                return py::make_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
            .def("iter_basepairs", [](const PrimitiveRNAStructure & s) {
                return py::make_iterator(s.bps_begin(), s.bps_end()); }, py::keep_alive<0, 1>())
            .def("iter_ends", [](const PrimitiveRNAStructure & s) {
                return py::make_iterator(s.ends_begin(), s.ends_end()); }, py::keep_alive<0, 1>())
            .def("get_residue",
                 (ResidueOP const & (PrimitiveRNAStructure::*)(int) const) &PrimitiveRNAStructure::get_residue)
            .def("get_residue",
                 (ResidueOP const & (PrimitiveRNAStructure::*)(int, char, char) const) &PrimitiveRNAStructure::get_residue)
            .def("get_residue",
                 (ResidueOP const & (PrimitiveRNAStructure::*)(util::Uuid const &) const) &PrimitiveRNAStructure::get_residue)
            .def("get_num_residues", &PrimitiveRNAStructure::get_num_residues)
            .def("get_num_chains", &PrimitiveRNAStructure::get_num_chains)
            .def("get_basepairs",
                 (BasepairOPs (PrimitiveRNAStructure::*)(util::Uuid const &) const) &PrimitiveRNAStructure::get_basepairs)
            .def("get_basepairs",
                 (BasepairOPs (PrimitiveRNAStructure::*)(util::Uuid const &, util::Uuid const &) const) &PrimitiveRNAStructure::get_basepairs)
            .def("get_basepairs",
                 (BasepairOPs (PrimitiveRNAStructure::*)(String const &) const) &PrimitiveRNAStructure::get_basepairs)
            .def("get_basepair",
                 (BasepairOP (PrimitiveRNAStructure::*)(util::Uuid const &) const) &PrimitiveRNAStructure::get_basepair)
            .def("get_basepair",
                 (BasepairOP (PrimitiveRNAStructure::*)(util::Uuid const &, util::Uuid const &) const) &PrimitiveRNAStructure::get_basepair)
            .def("get_basepair",
                 (BasepairOP (PrimitiveRNAStructure::*)(String const &) const) &PrimitiveRNAStructure::get_basepair)
            .def("get_basepair",
                 (BasepairOP const & (PrimitiveRNAStructure::*)(int) const) &PrimitiveRNAStructure::get_basepair)
            .def("get_end",
                 (BasepairOP (PrimitiveRNAStructure::*)(util::Uuid const &) const) &PrimitiveRNAStructure::get_end)
            .def("get_end",
                 (BasepairOP (PrimitiveRNAStructure::*)(util::Uuid const &, util::Uuid const &) const) &PrimitiveRNAStructure::get_end)
            .def("get_end",
                 (BasepairOP (PrimitiveRNAStructure::*)(String const &) const) &PrimitiveRNAStructure::get_end)
            .def("get_end",
                 (BasepairOP (PrimitiveRNAStructure::*)(base::SimpleStringOP const &) const) &PrimitiveRNAStructure::get_end)
            .def("get_end",
                 (BasepairOP const & (PrimitiveRNAStructure::*)(int) const) &PrimitiveRNAStructure::get_end)
            .def("get_end_id", &PrimitiveRNAStructure::get_end_id)
            .def("get_end_index",
                 (int (PrimitiveRNAStructure::*)(base::SimpleStringOP const &) const) &PrimitiveRNAStructure::get_end_index);


    m.def("generate_bp_name", &generate_bp_name);
    m.def("generate_end_id", &generate_end_id);

    py::register_exception<ResidueException>(m, "ResidueException");
    py::register_exception<ChainException>(m, "ChainException");
    py::register_exception<StructureException>(m, "StructureException");
    py::register_exception<BasepairException>(m, "BasepairException");
    py::register_exception<RNAStructureException>(m, "RNAStructureException");

    return m.ptr();
}

}