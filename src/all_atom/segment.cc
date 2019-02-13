//
// Created by Joseph Yesselman on 12/20/17.
//

#include <all_atom/segment.h>

namespace all_atom {

String
Segment::get_pdb_str(
        int & acount,
        int & rnum,
        char & chain_id) const {
    //TODO might need to alter the chain id for both proteins and small molecules
    auto pdb_str = structure_.get_pdb_str(acount, rnum, chain_id);
    if(proteins_.get_num_residues() > 0) {
        pdb_str += proteins_.get_pdb_str(acount, rnum, chain_id);
    }
    if(small_molecules_.get_num_residues() > 0) {
        pdb_str += small_molecules_.get_pdb_str(acount, rnum, chain_id);
    }

    return pdb_str;
}

void
Segment::write_pdb(
        String const & fname) const {
    std::ofstream out;
    out.open(fname.c_str());
    out << get_pdb_str(1) << std::endl;
    out.close();
}

void
Segment::write_steric_beads_to_pdb(
        String const & fname) {
    structure_.write_steric_beads_to_pdb(fname);

}

}