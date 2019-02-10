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
            primitives::Residue(r.name_, r.num_, r.chain_id_, r.i_code_, r.uuid_),
            beads_(r.beads_){}


    Residue(
            json::JSON & j):
            primitives::Residue() {
        name_     = (char)j["name"].ToInt();
        num_      = (int)j["num"].ToInt();
        chain_id_ = (char)j["chain_id"].ToInt();
        i_code_   = (char)j["i_code"].ToInt();
        uuid_     = util::Uuid();
        beads_    = util::Beads();
        auto & bead_json = j["beads"];
        for(int i = 0; i < bead_json.size(); i++) {
            beads_.push_back(util::Bead(bead_json[i]));
        }

    }

    ~Residue() {}

public: //iterator stuff
    typedef util::Beads::const_iterator const_iterator;

    const_iterator begin() const noexcept { return beads_.begin(); }
    const_iterator end() const noexcept   { return beads_.end(); }


public:

    inline
    bool
    operator == (
            Residue const & r) const {
        return is_equal(r);
    }

    inline
    bool
    operator != (
            Residue const & r) const {
        return !is_equal(r);
    }

    friend
    std::ostream &
    operator<< (
            std::ostream & stream ,
            Residue const & r) {
        stream << r.num_ << r.chain_id_;
        if(r.i_code_ != ' ') { stream << "(" << r.i_code_ << ")"; }
        return stream;
    }


public:
    inline
    bool
    is_equal(
            Residue const & r,
            CheckUUID check_uuid = CheckUUID::YES) const {
        if(check_uuid == CheckUUID::YES && uuid_ != r.uuid_) { return false; }
        if(name_ != r.name_) { return false; }
        if(num_ != r.num_) { return false; }
        if(chain_id_ != r.chain_id_) { return false; }
        if(i_code_ != r.i_code_) { return false; }
        for(int i = 0; i < beads_.size(); i++) {
            if(beads_[i] != r.beads_[i]) { return false;}
        }
        return true;
    }



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
    get_json() const {
        auto bead_array = json::Array();
        for(auto const & b : beads_) { bead_array.append(b.get_json()); }

        return json::JSON {
                "name", name_,
                "num", num_,
                "chain_id", chain_id_,
                "i_code", i_code_,
                "beads", bead_array };
    }

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

typedef std::vector<Residue>     Residues;
typedef std::shared_ptr<Residue> ResidueOP;
typedef std::vector<ResidueOP>   ResidueOPs;

}


#endif //RNAMAKE_NEW_state_RESIDUE_H
