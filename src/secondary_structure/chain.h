//
//  chain.h
//  RNAMake
//
//  Created by Joseph Yesselman on 7/31/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#ifndef __RNAMake__ss_chain__
#define __RNAMake__ss_chain__

#include <stdio.h>

#include <primitives/chain.h>
#include <secondary_structure/residue.h>

namespace secondary_structure {


class Chain : public primitives::Chain<secondary_structure::Residue> {
public:
    typedef primitives::Chain<secondary_structure::Residue> ParentClass;

public:
    inline
    Chain(
            ResidueOPs const & residues): ParentClass(residues) {}

    inline
    Chain(
            Chain const & c) {
        residues_ = ResidueOPs(c.residues_.size());
        int i = 0;
        for(auto const & r : c.residues_) {
            residues_[i] = std::make_shared<Residue>(*r);
            i++;
        }
    }

    inline
    Chain(
            String const & s) {
        residues_ = ResidueOPs();
        Strings spl = base::split_str_by_delimiter(s, ";");
        for(auto const & r_str : spl) {
            if(r_str.length() < 3) { continue; }
            auto r = std::make_shared<Residue>(r_str);
            residues_.push_back(r);
        }
    }

    virtual
    ~Chain() {}
    
public:
    
    typedef typename ResidueOPs::iterator iterator;
    typedef typename ResidueOPs::const_iterator const_iterator;
    
    iterator begin() { return residues_.begin(); }
    iterator end()   { return residues_.end(); }
    
    const_iterator begin() const { return residues_.begin(); }
    const_iterator end()   const { return residues_.end(); }
    
public:

    inline
    String
    get_dot_bracket() {
        auto db = String("");
        for(auto const & r : residues_) { db += r->get_dot_bracket(); }
        return db;
    }

    inline
    String
    get_sequence() {
        auto seq = String("");
        for(auto const & r : residues_) { seq += r->get_name(); }
        return seq;
    }

    inline
    String
    get_str() {
        auto s = String("");
        for (auto const & r : residues_) { s += r->get_str() + ";"; }
        return s;
    }

};

typedef std::shared_ptr<Chain> ChainOP;
typedef std::vector<ChainOP>   ChainOPs;

bool
are_chains_equal(
        ChainOP const & c1,
        ChainOP const & c2,
        bool check_uuid = true);
    
    
} //secondary structure


#endif /* defined(__RNAMake__chain__) */
