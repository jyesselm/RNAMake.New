//
// Created by Joseph Yesselman on 11/3/17.
//
#ifndef RNAMAKE_NEW_RNA_SEGMENT_H
#define RNAMAKE_NEW_RNA_SEGMENT_H

#include <util/segment_type.h>
#include <primitives/segment.h>
#include <secondary_structure/pose.h>

namespace secondary_structure {

class Segment : public primitives::Segment<Basepair, Structure, Chain, Residue>  {
public:
    typedef primitives::Segment<Basepair, Structure, Chain, Residue> BaseClass;

public:
    Segment(
            Structure const & structure,
            Basepairs const & basepairs,
            Indexes const & end_indexes,
            base::SimpleStringCOPs const & end_ids,
            base::SimpleStringCOP name,
            util::SegmentType segment_type,
            Index aligned_end_index,
            util::Uuid const & uuid):
            BaseClass(structure, basepairs, end_indexes, end_ids, name, segment_type, aligned_end_index, uuid) {}

public:
    void
    set_sequence(
            String const & sequence) {
        structure_.set_sequence(sequence);
    }

    inline
    void
    set_residue_identity(
            Index residue_index,
            char name) {
        structure_.set_residue_identity(residue_index, name);
    }



};

typedef std::shared_ptr<Segment> SegmentOP;

}



#endif //RNAMAKE_NEW_RNA_SEGMENT_H
