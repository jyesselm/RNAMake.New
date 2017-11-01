//
// Created by Joseph Yesselman on 8/27/17.
//

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

//rnamake headers
#include <secondary_structure/residue.h>
#include <secondary_structure/chain.h>
#include <secondary_structure/structure.h>
#include <secondary_structure/basepair.h>
#include <secondary_structure/rna_structure.h>

namespace py = pybind11;
namespace secondary_structure {

PYBIND11_PLUGIN(secondary_structure) {
    py::module m("secondary_structure", "rnamake's most basic classes and functions");

    // Residue Class
    py::class_<Residue, std::shared_ptr<Residue>>(m, "Residue")
            .def("get_chain_id", &Residue::get_chain_id)
            .def("get_name", &Residue::get_name)
            .def("get_num", &Residue::get_num)
            .def("get_i_code", &Residue::get_i_code)
            .def("get_uuid", &Residue::get_uuid)
            .def("get_str", &Residue::get_str)
            .def("get_dot_bracket", &Residue::get_dot_bracket)
            .def("get_res_type", &Residue::get_res_type)
            .def("set_name", &Residue::set_name)
            .def("is_equal", &Residue::is_equal)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def(py::init<String>())
            .def(py::init<Residue>())
            .def(py::init<char, char, int, char, char, util::Uuid>());

    py::class_<Chain, std::shared_ptr<Chain>>(m, "Chain")
            .def("__iter__", [](const Chain & c) { return py::make_iterator(c.begin(), c.end()); },
                 py::keep_alive<0, 1>())
            .def("__len__", &Chain::get_length)
            .def("get_first", &Chain::get_first)
            .def("get_last",  &Chain::get_last)
            .def("get_residue", &Chain::get_residue)
            .def("get_length", &Chain::get_length)
            .def("get_dot_bracket", &Chain::get_dot_bracket)
            .def("get_str", &Chain::get_str)
            .def("get_sequence", &Chain::get_sequence)
            .def("set_residue_name", &Chain::set_residue_name)
            .def("is_equal", &Chain::is_equal)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def(py::init<Residues const &>())
            .def(py::init<Chain const &>())
            .def(py::init<String const &>());

    py::class_<Structure, std::shared_ptr<Structure>>(m, "Structure")
            .def(py::init<Residues const &, Cutpoints const &>())
            .def(py::init<Structure const &>())
            .def(py::init<String const &>())
            .def("__iter__", [](const Structure & s) {
                return py::make_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
            .def("get_residue",
                 (Residue const & (Structure::*)(Index) const) &Structure::get_residue)
            .def("get_residue",
                 (Residue const & (Structure::*)(int, char, char) const) &Structure::get_residue)
            .def("get_residue",
                 (Residue const & (Structure::*)(util::Uuid const &) const) &Structure::get_residue)
            .def("get_res_index", &Structure::get_res_index)
            .def("get_num_residues", &Structure::get_num_residues)
            .def("get_num_chains", &Structure::get_num_chains)
            .def("get_sequence", &Structure::get_sequence)
            .def("get_dot_bracket", &Structure::get_dot_bracket)
            .def("get_chains", &Structure::get_chains, py::return_value_policy::reference)
            .def("is_equal", &Structure::is_equal)
            .def("set_residue_name", &Structure::set_residue_name)
            .def("set_sequence", &Structure::set_sequence)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def("get_str", &Structure::get_str);

    py::class_<Basepair,  std::shared_ptr<Basepair>>(m, "Basepair")
            .def("get_bp_type", &Basepair::get_bp_type)
            .def("get_partner", &Basepair::get_partner)
            .def("get_name", &Basepair::get_name)
            .def("get_res1_uuid", &Basepair::get_res1_uuid)
            .def("get_res2_uuid", &Basepair::get_res2_uuid)
            .def("get_uuid", &Basepair::get_uuid)
            .def("get_str", &Basepair::get_str)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def("is_equal", &Basepair::is_equal)
            .def(py::init<Basepair const &>())
            .def(py::init<const util::Uuid &, const util::Uuid &, const util::Uuid &,
                    primitives::BasepairType const &, base::SimpleStringCOP const &>());

    typedef base::VectorContainer<Residue> ResiduesVC;
    py::class_<ResiduesVC, std::shared_ptr<ResiduesVC>>(m, "Residues")
            .def(py::init<Residues const &>())
    .def("__iter__", []( ResiduesVC const & c) {
        return py::make_iterator(c.begin(), c.end()); }, py::keep_alive<0, 1>())
            .def("__len__", &ResiduesVC::size)
            .def("size", &ResiduesVC::size)
            .def("__getitem__", &ResiduesVC::operator[]);

    typedef base::VectorContainer<Chain> ChainsVC;
    py::class_<ChainsVC, std::shared_ptr<ChainsVC>>(m, "Chains")
            .def(py::init<std::vector<Chain> const &>())
    .def("__iter__", []( ChainsVC const & c) {
        return py::make_iterator(c.begin(), c.end()); }, py::keep_alive<0, 1>())
            .def("__len__", &ChainsVC::size)
            .def("size", &ChainsVC::size)
            .def("__getitem__", &ChainsVC::operator[]);

    typedef base::VectorContainer<Basepair> BasepairsVC;
    typedef std::shared_ptr<BasepairsVC> BasepairsOP;
    py::class_<BasepairsVC, std::shared_ptr<BasepairsVC>>(m, "Basepairs")
            .def(py::init<std::vector<Basepair> const &>())
            .def("__iter__", []( BasepairsVC const & c) {
        return py::make_iterator(c.begin(), c.end()); }, py::keep_alive<0, 1>())
            .def("__len__", &BasepairsVC::size)
            .def("size", &BasepairsVC::size)
            .def("__getitem__", &BasepairsVC::operator[]);

    py::class_<RNAStructure, std::shared_ptr<RNAStructure>>(m, "RNAStructure")
            .def(py::init<Structure const &, std::vector<Basepair> const &, std::vector<Basepair> const &,
            base::SimpleStringCOPs const &, base::SimpleStringCOP const &>())
            .def("__iter__", [](const RNAStructure & s) {
                return py::make_iterator(s.begin(), s.end()); }, py::keep_alive<0, 1>())
            .def("iter_basepairs", [](const RNAStructure & s) {
                return py::make_iterator(s.bps_begin(), s.bps_end()); }, py::keep_alive<0, 1>())
            .def("iter_ends", [](const RNAStructure & s) {
                return py::make_iterator(s.ends_begin(), s.ends_end()); }, py::keep_alive<0, 1>())
            .def("get_residue",
                 (Residue const & (RNAStructure::*)(int) const) &RNAStructure::get_residue)
            .def("get_residue",
                 (Residue const & (RNAStructure::*)(int, char, char) const) &RNAStructure::get_residue)
            .def("get_residue",
                 (Residue const & (RNAStructure::*)(util::Uuid const &) const) &RNAStructure::get_residue)
            .def("get_num_residues", &RNAStructure::get_num_residues)
            .def("get_num_chains", &RNAStructure::get_num_chains)
            .def("get_basepairs",
                 (BasepairsOP (RNAStructure::*)(util::Uuid const &) const) &RNAStructure::get_basepairs)
            .def("get_basepairs",
                 (BasepairsOP (RNAStructure::*)(util::Uuid const &, util::Uuid const &) const) &RNAStructure::get_basepairs)
            .def("get_basepairs",
                 (BasepairsOP (RNAStructure::*)(String const &) const) &RNAStructure::get_basepairs)
            .def("get_basepair",
                 (Basepair const & (RNAStructure::*)(util::Uuid const &) const) &RNAStructure::get_basepair)
            .def("get_basepair",
                 (Basepair const & (RNAStructure::*)(util::Uuid const &, util::Uuid const &) const) &RNAStructure::get_basepair)
            .def("get_basepair",
                 (Basepair const & (RNAStructure::*)(String const &) const) &RNAStructure::get_basepair)
            .def("get_basepair",
                 (Basepair const & (RNAStructure::*)(int) const) &RNAStructure::get_basepair)
            .def("get_end",
                 (Basepair const & (RNAStructure::*)(util::Uuid const &) const) &RNAStructure::get_end)
            .def("get_end",
                 (Basepair const & (RNAStructure::*)(util::Uuid const &, util::Uuid const &) const) &RNAStructure::get_end)
            .def("get_end",
                 (Basepair const & (RNAStructure::*)(String const &) const) &RNAStructure::get_end)
            .def("get_end",
                 (Basepair const & (RNAStructure::*)(base::SimpleStringCOP) const) &RNAStructure::get_end)
            .def("get_end",
                 (Basepair const & (RNAStructure::*)(int) const) &RNAStructure::get_end)
            .def("get_end_by_id",
                 (Basepair const & (RNAStructure::*)(String const &) const) &RNAStructure::get_end_by_id)
            .def("get_end_by_id",
                 (Basepair const & (RNAStructure::*)(base::SimpleStringCOP) const) &RNAStructure::get_end_by_id)
            .def("get_end_id", &RNAStructure::get_end_id)
            .def("get_end_index",
                 (int (RNAStructure::*)(base::SimpleStringCOP) const) &RNAStructure::get_end_index)
            .def("get_end_index",
                 (int (RNAStructure::*)(String const &) const) &RNAStructure::get_end_index)
            .def("get_bp_res", &RNAStructure::get_bp_res)
            .def("get_num_basepairs", &RNAStructure::get_num_basepairs)
            .def("get_num_ends", &RNAStructure::get_num_ends)
            .def("get_name", &RNAStructure::get_name)
            .def("get_str", &RNAStructure::get_str)
            .def("set_residue_name", &RNAStructure::set_residue_name)
            .def("set_sequence", &RNAStructure::set_sequence)
            .def(py::self == py::self)
            .def(py::self != py::self);


    m.def("get_structure_from_secondary_structure", &get_structure_from_secondary_structure);
    m.def("generate_bp_name", &primitives::generate_bp_name<Residue>);
    m.def("generate_end_id", &primitives::generate_end_id<Structure, Chain, Basepair, Residue>);


    py::register_exception<ResidueException>(m, "ResidueException");
    py::register_exception<ChainException>(m, "ChainException");
    py::register_exception<StructureException>(m, "StructureException");
    py::register_exception<RNAStructureException>(m, "RNAStructureException");

    return m.ptr();

}
}
