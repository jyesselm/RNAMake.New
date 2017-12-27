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

public:
    inline
    String
    dot_bracket() { return dot_bracket_->get_str(); }

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
