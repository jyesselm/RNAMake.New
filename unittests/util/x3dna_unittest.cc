//
// Created by Joseph Yesselman on 12/7/17.
//

#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <util/x3dna.h>

TEST_CASE( "Test X3dna parser ", "[X3dnaParser]" ) {
    auto x = util::X3dna();
    auto path = base::unittest_resources_path() + "/all_atom/p4p6.pdb";
    auto x3dna_bps = x.get_basepairs(path);


}