//
//  residue.cc
//  RNAMake
//
//  Created by Joseph Yesselman on 7/31/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#include <secondary_structure/residue.h>

namespace secondary_structure {

bool
are_residues_equal(
        ResidueOP const & r1,
        ResidueOP const & r2,
        bool check_uuid) {

    if(r1->get_name() != r2->get_name()) { return false; }
    if(r1->get_dot_bracket() != r2->get_dot_bracket()) { return false; }
    if(r1->get_num() != r2->get_num()) { return false; }
    if(r1->get_chain_id() != r2->get_chain_id()) { return false; }
    if(r1->get_i_code() != r2->get_i_code()) { return false; }
    if(check_uuid && r1->get_uuid() != r2->get_uuid()) { return false; }
    return true;
}

}