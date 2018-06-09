//
// Created by Joseph Yesselman on 6/9/18.
//

#ifndef RNAMAKE_NEW_state_RESIDUE_H
#define RNAMAKE_NEW_state_RESIDUE_H

//RNAMake Headers
#include <primitives/residue.h>
#include <util/bead.h>

namespace state {

class Residue : public primitives::Residue {
public:
    inline
    Residue(
           char name,
           int num,
           char chain_id,
           char i_code,
           util::Beads const & beads,
           util::Uuid const & uuid):
           primitives::Residue(name, num, chain_id, i_code, uuid),
           beads_(std::move(beads)) {}

    inline
    Residue(
            Residue const & r):
            primitives::Residue(r.name_, r.num_, r.chain_id_, r.i_code_, r.uuid_) {}

private:
    /**
     * vector of bead objects for sterics
     */
    util::Beads beads_;

};

}


#endif //RNAMAKE_NEW_state_RESIDUE_H
