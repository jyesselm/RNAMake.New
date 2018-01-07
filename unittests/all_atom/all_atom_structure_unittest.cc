//
// Created by Joseph Yesselman on 12/2/17.
//


#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/structure.h>

TEST_CASE( "Test all_atom structure ", "[AllAtomStructure]" ) {
    auto rts = all_atom::ResidueTypeSet();
    auto parser = all_atom::PDBParser(rts);
    auto path = base::unittest_resources_path() + "/all_atom/p4p6.pdb";
    auto residues = parser.parse(path);

    auto s = all_atom::get_structure_from_residues(residues->RNA_residues);
    REQUIRE(s->get_num_chains() == 1);
    REQUIRE(s->get_num_residues() == 157);

    SECTION("test protein structure") {
        auto path = base::unittest_resources_path() + "/all_atom/2lxe.pdb";
        auto s = all_atom::get_structure_from_pdb(path, rts, all_atom::SetType::PROTEIN);
        REQUIRE(s->get_num_chains() == 1);
    }
}