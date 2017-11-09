//
// Created by Joseph Yesselman on 11/3/17.
//
#ifndef RNAMAKE_NEW_RNA_SEGMENT_H
#define RNAMAKE_NEW_RNA_SEGMENT_H

#include <util/rna_segment_type.h>
#include <secondary_structure/rna_structure.h>

namespace secondary_structure {

class RNASegment : public RNAStructure {
public:
    RNASegment(
            Structure const & s,
            Basepairs const & basepairs,
            Basepairs const & ends,
            base::SimpleStringCOPs const & end_ids,
            base::SimpleStringCOP & name,
            util::RNASegmentType const & type,
            Flag is_motif,
            Index block_end):
            secondary_structure::RNAStructure(s, basepairs, ends, end_ids, name),
            type_(type),
            is_motif_(is_motif),
            block_end_(block_end){}

    RNASegment(
            RNAStructure const & rs,
            util::RNASegmentType const & type,
            Flag is_motif,
            Index block_end):
            secondary_structure::RNAStructure(rs),
            type_(type),
            is_motif_(is_motif),
            block_end_(block_end){}

public: //unique getters
    inline
    util::RNASegmentType
    get_type() const { return type_; }

    inline
    Flag
    is_motif() const { return is_motif_; }

    inline
    Index
    block_end() const { return block_end_; }

private:
    util::RNASegmentType type_;
    Flag is_motif_;
    Index block_end_;
};

typedef std::shared_ptr<RNASegment> RNASegmentOP;

}



#endif //RNAMAKE_NEW_RNA_SEGMENT_H
