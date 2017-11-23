//
// Created by Joseph Yesselman on 11/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <base/logger.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/pdb_parser.h>


TEST_CASE( "Test Parsing PDB formatted files ", "[PDBParser]" ) {
    auto rts = std::make_shared<all_atom::ResidueTypeSet>();
    auto parser = all_atom::PDBParser(rts);
}