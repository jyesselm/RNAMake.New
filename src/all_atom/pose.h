//
// Created by Joseph Yesselman on 12/15/17.
//

#ifndef RNAMAKE_NEW_ALL_ATOM_RNA_STRUCTURE_H
#define RNAMAKE_NEW_ALL_ATOM_RNA_STRUCTURE_H


#include <primitives/pose.h>
#include <all_atom/structure.h>
#include <all_atom/basepair.h>
#include <all_atom/pdb_parser.h>

namespace all_atom {

class Pose : public primitives::Pose<Basepair, Structure, Chain, Residue> {
public:
    friend class SegmentFactory;

public:
    typedef primitives::Pose<Basepair, Structure, Chain, Residue> BaseClass;

public:
    inline
    Pose(
            Structure const & structure,
            Basepairs const & basepairs,
            Basepairs const & ends,
            base::SimpleStringCOPs const & end_ids,
            base::SimpleStringCOP name,
            Structure const & proteins,
            Structure const & small_molecules,
            base::SimpleStringCOP dot_bracket):
            BaseClass(structure, basepairs, ends, end_ids, name),
            proteins_(proteins),
            small_molecules_(small_molecules),
            dot_bracket_(dot_bracket) {}
    inline
    Pose(
            Pose const & p):
            BaseClass(p.structure_, p.basepairs_, p.ends_, p.end_ids_, p.name_),
            proteins_(p.proteins_),
            small_molecules_(p.small_molecules_),
            dot_bracket_(p.dot_bracket_) {}

    inline
    Pose(
            json::JSON & j,
            ResidueTypeSet const & rts):
            BaseClass(),
            proteins_(Structure(j["proteins"], rts)),
            small_molecules_(Structure(j["small_molecules"], rts)) {
        structure_ = Structure(j["structure"], rts);
    }

public:
    inline
    String
    get_dot_bracket() { return dot_bracket_->get_str(); }

public:

    json::JSON
    get_json() const {
        auto j_bps = json::Array();
        auto j_ends = json::Array();
        auto j_end_ids = json::Array();

        for(auto const & bp : basepairs_) {
            auto bp_res = get_bp_res(bp);
            j_bps.append(json::Array(bp.get_json(), bp_res->at(0).get_num(), bp_res->at(0).get_chain_id(),
                                     bp_res->at(0).get_i_code(), bp_res->at(1).get_num(),
                                     bp_res->at(1).get_chain_id(), bp_res->at(1).get_i_code()));
        }

        for(auto const & bp : ends_) {
            auto bp_res = get_bp_res(bp);
            j_ends.append(json::Array(bp.get_json(), bp_res->at(0).get_num(), bp_res->at(0).get_chain_id(),
                                     bp_res->at(0).get_i_code(), bp_res->at(1).get_num(),
                                     bp_res->at(1).get_chain_id(), bp_res->at(1).get_i_code()));
        }

        for(auto const & end_id : end_ids_) { j_end_ids.append(end_id->get_str()); }

        return json::JSON{
                "structure", structure_.get_json(),
                "basepairs", j_bps,
                "ends", j_ends,
                "end_ids", j_end_ids,
                "name", name_->get_str(),
                "proteins", proteins_.get_json(),
                "small_molecules", small_molecules_.get_json(),
                "dot_bracket", dot_bracket_->get_str() };

    }



protected:
    Structure proteins_;
    Structure small_molecules_;
    base::SimpleStringCOP dot_bracket_;
};

typedef std::shared_ptr<Pose> PoseOP;

inline
base::VectorContainerOP<Basepair>
get_ends_from_basepairs(
        Structure const & s,
        Basepairs const & bps) {
    return primitives::get_ends_from_basepairs<Basepair, Structure>(s, bps);
}

inline
String
generate_end_id(
        Structure const & s,
        Basepairs const & bps,
        Basepairs const & ends,
        Basepair const & end) {
    return primitives::generate_end_id<Structure, Chain, Basepair, Residue>(s, bps, ends, end);
}

inline
String
generate_secondary_structure(
        Structure const & s,
        Basepairs const & bps,
        Basepairs const & ends) {
    return primitives::generate_secondary_structure<Structure, Chain, Basepair, Residue>(s, bps, ends);
}

PoseOP
get_pose_from_pdb(
        String const &,
        PDBParser &);

}
#endif //RNAMAKE_NEW_ALL_ATOM_RNA_STRUCTURE_H
