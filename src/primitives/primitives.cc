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

    // Residue Class
    py::class_<PrimitiveResidue>(m, "Residue")
            .def("get_chain_id", &PrimitiveResidue::get_chain_id)
            .def("get_name", &PrimitiveResidue::get_name)
            .def("get_num", &PrimitiveResidue::get_num)
            .def("get_i_code", &PrimitiveResidue::get_i_code)
            .def("get_uuid", &PrimitiveResidue::get_uuid)
            .def("get_str", &PrimitiveResidue::get_str)
            .def(py::self == py::self)
            .def(py::init<char, int, char, char, util::Uuid const &>());

    py::class_<PrimitiveChain>(m, "Chain")
            .def(py::init<PrimitiveResidues const & >())
            .def(py::init<PrimitiveChain const &>())
            .def("__iter__", [](PrimitiveChain const & c) { return py::make_iterator(c.begin(), c.end()); },
            py::keep_alive<0, 1>())
            .def("__len__", &PrimitiveChain::get_length)
            .def("get_first", &PrimitiveChain::get_first)
            .def("get_last",  &PrimitiveChain::get_last)
            .def("get_residue", &PrimitiveChain::get_residue)
            .def("get_length", &PrimitiveChain::get_length);

    typedef base::VectorContainer<PrimitiveResidue> Residues;
    py::class_<Residues, std::shared_ptr<Residues>>(m, "Residues")
            .def(py::init<PrimitiveResidues const &>())
            .def("__iter__", []( Residues const & c) {
                return py::make_iterator(c.begin(), c.end()); }, py::keep_alive<0, 1>())
            .def("__len__", &Residues::size)
            .def("size", &Residues::size)
            .def("__getitem__", &Residues::operator[]);

    typedef base::VectorContainer<PrimitiveChain> Chains;
    py::class_<Chains, std::shared_ptr<Chains>>(m, "Chains")
            .def(py::init<std::vector<PrimitiveChain> const &>())
            .def("__iter__", []( Chains const & c) {
                return py::make_iterator(c.begin(), c.end()); }, py::keep_alive<0, 1>())
            .def("__len__", &Chains::size)
            .def("size", &Chains::size)
            .def("__getitem__", &Chains::operator[]);

    typedef base::VectorContainer<PrimitiveBasepair> Basepairs;
    typedef std::shared_ptr<Basepairs> BasepairsOP;
    py::class_<Basepairs, std::shared_ptr<Basepairs>>(m, "Basepairs")
            .def(py::init<std::vector<PrimitiveBasepair> const &>())
    .def("__iter__", []( Basepairs const & c) {
        return py::make_iterator(c.begin(), c.end()); }, py::keep_alive<0, 1>())
            .def("__len__", &Basepairs::size)
            .def("size", &Basepairs::size)
            .def("__getitem__", &Basepairs::operator[]);

    py::class_<PrimitiveStructure>(m, "Structure")
            .def(py::init<PrimitiveResidues const &, Cutpoints const &>())
            .def("__iter__", [](const PrimitiveStructure & s) {
                     return py::make_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
            .def("get_residue",
                 (Residue const & (PrimitiveStructure::*)(Index) const) &PrimitiveStructure::get_residue)
            .def("get_residue",
                 (Residue const & (PrimitiveStructure::*)(int, char, char) const) &PrimitiveStructure::get_residue)
            .def("get_residue",
                 (Residue const & (PrimitiveStructure::*)(util::Uuid const &) const) &PrimitiveStructure::get_residue)
            .def("get_res_index", &PrimitiveStructure::get_res_index)
            .def("get_num_residues", &PrimitiveStructure::get_num_residues)
            .def("get_num_chains", &PrimitiveStructure::get_num_chains)
            .def("get_sequence", &PrimitiveStructure::get_sequence)
            .def("get_chains", &PrimitiveStructure::get_chains, py::return_value_policy::reference);

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
            .def(py::self == py::self)
            .def(py::init<const util::Uuid &, const util::Uuid &, const util::Uuid &,
                    BasepairType const &, base::SimpleStringOP const &>());

    /*py::class_<PrimitiveRNAStructure, PrimitiveRNAStructureOP>(m, "RNAStructure")
            .def(py::init<PrimitiveStructure const &, PrimitiveBasepairs const &, PrimitiveBasepairs const &,
                           base::SimpleStringCOPs const &, base::SimpleStringCOP const &>())
            .def("__iter__", [](const PrimitiveRNAStructure & s) {
                return py::make_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
           .def("iter_basepairs", [](const PrimitiveRNAStructure & s) {
                return py::make_iterator(s.bps_begin(), s.bps_end()); }, py::keep_alive<0, 1>())
            .def("iter_ends", [](const PrimitiveRNAStructure & s) {
                return py::make_iterator(s.ends_begin(), s.ends_end()); }, py::keep_alive<0, 1>())
            .def("get_residue",
                 (Residue const & (PrimitiveRNAStructure::*)(int) const) &PrimitiveRNAStructure::get_residue)
            .def("get_residue",
                 (Residue const & (PrimitiveRNAStructure::*)(int, char, char) const) &PrimitiveRNAStructure::get_residue)
            .def("get_residue",
                 (Residue const & (PrimitiveRNAStructure::*)(util::Uuid const &) const) &PrimitiveRNAStructure::get_residue)
            .def("get_num_residues", &PrimitiveRNAStructure::get_num_residues)
            .def("get_num_chains", &PrimitiveRNAStructure::get_num_chains)
            .def("get_basepairs",
                 (BasepairsOP (PrimitiveRNAStructure::*)(util::Uuid const &) const) &PrimitiveRNAStructure::get_basepairs)
            .def("get_basepairs",
                 (BasepairsOP (PrimitiveRNAStructure::*)(util::Uuid const &, util::Uuid const &) const) &PrimitiveRNAStructure::get_basepairs)
            .def("get_basepairs",
                 (BasepairsOP (PrimitiveRNAStructure::*)(String const &) const) &PrimitiveRNAStructure::get_basepairs)
            .def("get_basepair",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(util::Uuid const &) const) &PrimitiveRNAStructure::get_basepair)
            .def("get_basepair",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(util::Uuid const &, util::Uuid const &) const) &PrimitiveRNAStructure::get_basepair)
            .def("get_basepair",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(String const &) const) &PrimitiveRNAStructure::get_basepair)
            .def("get_basepair",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(int) const) &PrimitiveRNAStructure::get_basepair)
            .def("get_end",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(util::Uuid const &) const) &PrimitiveRNAStructure::get_end)
            .def("get_end",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(util::Uuid const &, util::Uuid const &) const) &PrimitiveRNAStructure::get_end)
            .def("get_end",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(String const &) const) &PrimitiveRNAStructure::get_end)
            .def("get_end",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(base::SimpleStringCOP) const) &PrimitiveRNAStructure::get_end)
            .def("get_end",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(int) const) &PrimitiveRNAStructure::get_end)
            .def("get_end_by_id",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(String const &) const) &PrimitiveRNAStructure::get_end_by_id)
            .def("get_end_by_id",
                 (PrimitiveBasepair const & (PrimitiveRNAStructure::*)(base::SimpleStringCOP) const) &PrimitiveRNAStructure::get_end_by_id)
            .def("get_end_id", &PrimitiveRNAStructure::get_end_id)
            .def("get_end_index",
                 (int (PrimitiveRNAStructure::*)(base::SimpleStringCOP) const) &PrimitiveRNAStructure::get_end_index)
            .def("get_end_index",
                 (int (PrimitiveRNAStructure::*)(String const &) const) &PrimitiveRNAStructure::get_end_index)
            .def("get_bp_res", &PrimitiveRNAStructure::get_bp_res)
            .def("get_num_basepairs", &PrimitiveRNAStructure::get_num_basepairs)
            .def("get_num_ends", &PrimitiveRNAStructure::get_num_ends)
            .def("get_name", &PrimitiveRNAStructure::get_name);*/


    m.def("generate_bp_name", &generate_bp_name<PrimitiveResidue>);
    m.def("generate_end_id", &generate_end_id<PrimitiveStructure, PrimitiveChain, PrimitiveBasepair, PrimitiveResidue>);

    py::register_exception<ResidueException>(m, "ResidueException");
    py::register_exception<ChainException>(m, "ChainException");
    py::register_exception<StructureException>(m, "StructureException");
    py::register_exception<BasepairException>(m, "BasepairException");
    py::register_exception<RNAStructureException>(m, "RNAStructureException");

    return m.ptr();
}

}