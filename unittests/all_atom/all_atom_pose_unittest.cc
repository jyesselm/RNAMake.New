//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include "../common.hpp"

#include <math/numerical.h>
#include <util/random_number_generator.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/basepair.h>
#include <all_atom/pose.h>

TEST_CASE( "Test all atom pose", "[Basepair]" ) {
    auto rts = all_atom::ResidueTypeSet();
    auto parser = all_atom::PDBParser(rts);

    auto path = base::unittest_resources_path() + "/all_atom/p4p6.pdb";
    auto p = all_atom::get_pose_from_pdb(path, parser);


}
