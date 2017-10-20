//
//  chain.cc
//  RNAMake
//
//  Created by Joseph Yesselman on 7/31/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#include "secondary_structure/chain.h"

namespace secondary_structure {

bool
are_chains_equal(
        ChainOP const & c1,
        ChainOP const & c2,
        bool check_uuid) {

    if(c1->get_length() != c2->get_length()) { return false;}

    for(int i = 0; i < c1->get_length(); i++) {
        if(! are_residues_equal(c1->get_residue(i), c2->get_residue(i), check_uuid) ) { return false; }
    }
    return true;
}



}
