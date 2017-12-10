//
// Created by Joseph Yesselman on 11/29/17.
//

#ifndef RNAMAKE_ALL_ATOM_CHAIN_H
#define RNAMAKE_ALL_ATOM_CHAIN_H

#include <primitives/chain.h>
#include <all_atom/residue.h>

namespace all_atom {

class Chain : public primitives::Chain<Residue> {
public:
    typedef primitives::Chain<Residue> ParentClass;

public:
    inline
    Chain(
            Residues const & residues): ParentClass(residues) {}

    inline
    Chain(
            Chain const & c): ParentClass(c) {}

    Chain(
            String const & s,
            ResidueTypeSet const & rts) {

        residues_ = Residues();
        Strings spl = base::split_str_by_delimiter(s, ";");
        for(auto const & r_str : spl) {
            if (r_str.length() < 3) { continue; }
            residues_.push_back(Residue(r_str, rts));
        }


    }

    virtual
    ~Chain() {}

public:
    inline
    bool
    operator == (
            Chain const & c) const {
        if(residues_.size() != c.residues_.size()) { return false; }

        for(int i = 0; i < c.get_length(); i++) {
            if(residues_[i] != c.residues_[i]) { return false; }
        }
        return true;
    }

    inline
    bool
    operator != (
            Chain const & c) const {
        return !(*this == c);
    }

public:
    bool
    is_equal(
            Chain const & c,
            bool check_uuid = true) const {

        if(residues_.size() != c.residues_.size()) { return false; }

        for(int i = 0; i < c.get_length(); i++) {
            if(residues_[i].is_equal(c.residues_[i]), check_uuid) { return false; }
        }
        return true;
    }

public:

    inline
    String
    get_str() const {
        auto s = String("");
        for (auto const & r : residues_) { s += r.get_str() + ";"; }
        return s;
    }

};

typedef std::shared_ptr<Chain> ChainOP;
typedef std::vector<ChainOP>   ChainOPs;


}


#endif //RNAMAKE_ALL_ATOM_CHAIN_H