//
// Created by Joseph Yesselman on 11/29/17.
//

#include <all_atom/chain.h>

namespace all_atom {

String
Chain::get_pdb_str(
        int acount,
        int rnum,
        char chain_id) const {

    auto s = String("");
    for(auto const & r : residues_) {
        s += r.get_pdb_str(acount, rnum, chain_id);
        acount += r.get_num_atoms();
        rnum += 1;
    }
    return s;
}


}
