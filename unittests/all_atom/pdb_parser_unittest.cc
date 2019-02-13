//
// Created by Joseph Yesselman on 11/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <base/log.h>
#include <base/paths.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/pdb_parser.h>


TEST_CASE( "Test Parsing PDB formatted files ", "[PDBParser]" ) {

    auto rts = all_atom::ResidueTypeSet();

    SECTION("test rebuilding missing phosphates") {
        auto parser = all_atom::PDBParser(rts);
        // load ideal residue
        auto path = base::resources_path() + "/ideal_residues/GUA.pdb";
        auto residues_1 = parser.parse(path);

        // load in incomplete resiudue
        path = base::unittest_resources_path() + "/all_atom/GUA_incomplete.pdb";
        auto residues_2 = parser.parse(path);

        // rebuilt residue is indentical to ideal residue
        REQUIRE(residues_1->RNA_residues[0]->is_equal(*residues_2->RNA_residues[0], CheckUUID::NO));
    }

    SECTION("test parsing p4-p6") {
        auto parser = all_atom::PDBParser(rts);
        auto path = base::unittest_resources_path() + "/all_atom/p4p6.pdb";
        auto residues = parser.parse(path);
        REQUIRE(residues->RNA_residues.size() == 157);
    }

    SECTION("test parsing proteins") {
        auto parser = all_atom::PDBParser(rts);
        auto path = base::unittest_resources_path() + "/all_atom/2lxe.pdb";
        auto residues = parser.parse(path);

        REQUIRE(residues->protein_residues.size() == 109);
    }

    SECTION("test parsing small molecules") {
        auto parser = all_atom::PDBParser(rts);
        auto path = base::unittest_resources_path() + "/all_atom/1am0.pdb";
        auto residues = parser.parse(path);
        REQUIRE(residues->small_molecule_residues.size() == 1);
        auto amp = residues->small_molecule_residues[0];
        REQUIRE(amp->get_res_name() == "AMP");

        /*std::for_each(amp->bead_begin(),
                      amp->bead_end(),
                      [](util::Bead const & b) {
            std::cout << b.get_center().get_str() << std::endl;
        });*/

    }


}