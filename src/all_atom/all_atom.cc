#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <all_atom/atom.h>
#include <all_atom/residue_type.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/residue.h>
#include <all_atom/pdb_parser.h>

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

    py::enum_<SetType>(m, "SetType")
            .value("RNA", SetType::RNA)
            .value("PROTEIN", SetType::PROTEIN)
            .value("UNKNOWN", SetType::UNKNOWN)
            .export_values();

    py::class_<ResidueTypeSet, std::shared_ptr<ResidueTypeSet>>(m, "ResidueTypeSet")
            .def(py::init<>())
            .def("get_residue_type", &ResidueTypeSet::get_residue_type)
            .def("contains_residue_type", &ResidueTypeSet::contains_residue_type);

    py::class_<Residue, std::shared_ptr<Residue>>(m, "Residue")
            .def("get_chain_id", &Residue::get_chain_id)
            .def("get_name", &Residue::get_name)
            .def("get_num", &Residue::get_num)
            .def("get_i_code", &Residue::get_i_code)
            .def("get_uuid", &Residue::get_uuid)
            .def("get_str", &Residue::get_str)
            .def("get_atom", (Atom const & (Residue::*)(String const &) const) &Residue::get_atom)
            .def("get_atom", (Atom const & (Residue::*)(Index) const) &Residue::get_atom)
            .def("get_coords", &Residue::get_coords)
            .def("get_bead", &Residue::get_bead)
            .def("get_center", &Residue::get_center)
            .def("get_res_name", &Residue::get_res_name)
            .def("get_res_set_type", &Residue::get_res_set_type)
            .def("get_str", &Residue::get_str)
            .def("get_pdb_str", (String (Residue::*)(int) const) &Residue::get_pdb_str)
            .def("get_pdb_str", (String (Residue::*)(int, int, char) const) &Residue::get_pdb_str)
            .def("write_pdb", &Residue::write_pdb)
            .def("is_equal", &Residue::is_equal)
            .def("move", &Residue::move)
            .def("transform",
                 (void (Residue::*)(math::Matrix const &, math::Vector const &)) &Residue::transform)
            .def("transform",
                 (void (Residue::*)(math::Matrix const &, math::Vector const &, math::Point &)) &Residue::transform)
            .def(py::self == py::self)
            .def(py::self != py::self)
            .def(py::init<char, int, char, char, ResidueTypeCOP, Atoms const &, util::Uuid const &>());

    py::class_<PDBParser, std::shared_ptr<PDBParser>>(m, "PDBParser")
            .def(py::init<ResidueTypeSetCOP>())
            .def("parse", &PDBParser::parse);

    py::class_<PDBParserResidues, std::shared_ptr<PDBParserResidues>>(m, "PDBParserResidues")
            .def(py::init<ResidueOPs const &, ResidueOPs const &, ResidueOPs const &>())
            .def("has_RNA", &PDBParserResidues::has_RNA)
            .def("has_protein", &PDBParserResidues::has_protein)
            .def("has_small_molecules", &PDBParserResidues::has_small_molecules)
            .def_readonly("RNA_residues", &PDBParserResidues::RNA_residues)
            .def_readonly("protein_residues", &PDBParserResidues::protein_residues)
            .def_readonly("small_molecule_residues", &PDBParserResidues::small_molecule_residues);



    return m.ptr();
}

}
