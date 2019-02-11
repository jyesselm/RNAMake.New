//
// Created by Joseph Yesselman on 12/16/18.
//

#ifndef RNAMAKE_NEW_STATE_STRUCTURE_H
#define RNAMAKE_NEW_STATE_STRUCTURE_H

#include <primitives/structure.h>
#include <state/residue.h>
#include <state/chain.h>

namespace state {

class Structure : public primitives::Structure<Chain, Residue> {
public:
    typedef primitives::Structure<Chain, Residue> ParentClass;

public:
    inline
    Structure(
            Residues const & residues,
            Cutpoints const & cut_points): ParentClass(residues, cut_points) {}

    inline
    Structure(
            Structure const & structure): ParentClass(structure.residues_, structure.cut_points_) {}

    inline
    Structure(
            json::JSON & j) : ParentClass() {
        auto & j_res = j["residues"];
        auto & j_cuts = j["cutpoints"];

        for (int i = 0; i < j_res.size(); i++) {
            residues_.push_back(Residue(j_res[i]));
        }
        for (int i = 0; i < j_cuts.size(); i++) {
            cut_points_.push_back(j_cuts[i].ToInt());
        }

    }

    ~Structure() {}

public: //operators
    inline
    bool
    operator == (
            Structure const & s) const {
        return is_equal(s);
    }

    inline
    bool
    operator !=(
            Structure const & s) const {
        return !(is_equal(s));
    }

public:
    bool
    is_equal(
            Structure const & s,
            CheckUUID check_uuid = CheckUUID::YES) const {

        if(residues_.size() != s.residues_.size() ) { return false; }
        if(cut_points_.size() != s.cut_points_.size() ) { return false; }

        for(int i = 0; i < residues_.size(); i++) {
            if(!(residues_[i].is_equal(s.residues_[i], check_uuid))) { return false; }
        }

        for(int i = 0; i < cut_points_.size(); i++) {
            if(cut_points_[i] != s.cut_points_[i]) { return false; }
        }
        return true;
    }


public: // non const
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

    void
    new_uuids() {
        for(auto & r : residues_) { r.new_uuid(); }
    }

public: //getters

    json::JSON
    get_json() const {
        auto j_res = json::Array();
        auto j_cuts = json::Array();
        for(auto const & r : residues_) { j_res.append(r.get_json()); }
        for(auto const & i : cut_points_) { j_cuts.append(i); }
        auto j = json::Object();
        return json::JSON{
                "residues", j_res,
                "cutpoints", j_cuts};

    }

};

typedef std::shared_ptr<Structure> StructureOP;


}

#endif //RNAMAKE_NEW_STATE_STRUCTURE_H
