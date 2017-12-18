//
// Created by Joseph Yesselman on 12/15/17.
//

#ifndef RNAMAKE_NEW_ALL_ATOM_RNA_STRUCTURE_H
#define RNAMAKE_NEW_ALL_ATOM_RNA_STRUCTURE_H


#include <primitives/pose.h>
#include <all_atom/structure.h>
#include <all_atom/basepair.h>

namespace all_atom {

class Pose : primitives::Pose<Basepair, Structure, Chain, Residue> {
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

private:
    Structure proteins_;
    Structure small_molecules_;
    base::SimpleStringCOP dot_bracket_;

};

typedef std::shared_ptr<Pose> PoseOP;

PoseOP
pose_from_pdb(
        String const &,
        ResidueTypeSetCOP);

}
#endif //RNAMAKE_NEW_ALL_ATOM_RNA_STRUCTURE_H
