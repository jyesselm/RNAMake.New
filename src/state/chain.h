//
// Created by Joseph Yesselman on 6/9/18.
//

#ifndef RNAMAKE_NEW_state_CHAIN_H
#define RNAMAKE_NEW_state_CHAIN_H

#include <primitives/chain.h>
#include <state/residue.h>

namespace state {

class Chain : public primitives::Chain<Residue> {
public:
    typedef primitives::Chain<Residue> ParentClass;

public:
    inline
    Chain(
            Residues const & residues) : ParentClass(residues) {}

    inline
    Chain(
            Chain const & c) {
        residues_ = Residues();
        for(auto const & r : c) {
            residues_.push_back(Residue(r));
        }
    }

    Chain(
            json::JSON & j) {
        residues_ = Residues();
        for(int i = 0; i < j.size(); i++) {
            residues_.push_back(Residue(j[i]));
        }
    }

    virtual
    ~Chain() {}

public:
    bool
    operator == (
            Chain const & c) const {
        return is_equal(c);
    }

    bool
    operator != (
            Chain const & c) const {
        return !(is_equal(c));
    }

public:
    bool
    is_equal(
            Chain const & c,
            bool check_uuid = true) const {

        if(residues_.size() != c.residues_.size()) { return false; }

        for(int i = 0; i < c.get_length(); i++) {
            if(!(residues_[i].is_equal(c.residues_[i], check_uuid))) { return false; }
        }
        return true;
    }

public: // non const methods

    void
    move(
            math::Point const & p) {
        for(auto & r : residues_) { r.move(p); }
    }

    void
    transform(
            math::Matrix const & r,
            math::Vector const & t,
            math::Point & dummy) {

        for(auto & res : residues_) { res.transform(r, t, dummy); }
    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t) {
        auto dummy = math::Point();
        transform(r, t, dummy);
    }

public:

    inline
    json::JSON
    get_json() const {
        auto j = json::Array();
        for(auto const & r : residues_) { j.append(r.get_json()); }
        return j;
    }


};

}

#endif //RNAMAKE_NEW_state_CHAIN_H
