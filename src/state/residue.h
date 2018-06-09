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


    ~Residue() {}

public: //iterator stuff
    typedef util::Beads::const_iterator const_iterator;

    const_iterator begin() const noexcept { return beads_.begin(); }
    const_iterator end() const noexcept   { return beads_.end(); }

public:
    inline
    util::Bead const &
    get_bead(
            util::BeadType bead_type) const {
        for(auto const & b : beads_) {
            if(b.get_type() == bead_type) { return b; }
        }
        throw ResidueException(
                "bead type: " + std::to_string((int)bead_type) + " does not exist in this residue state");
    }

    json::JSON
    get_json() const;

public:

    inline
    void
    move(
            math::Point const & p) {
        for(auto & b : beads_) { b.move(p); }
    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t,
            math::Point & dummy) {
        for(auto & b : beads_) { b.transform(r, t, dummy); }
    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t) {
        auto dummy = math::Point();
        transform(r, t, dummy);
    }



private:
    /**
     * vector of bead objects for sterics
     */
    util::Beads beads_;

};

}


#endif //RNAMAKE_NEW_state_RESIDUE_H
