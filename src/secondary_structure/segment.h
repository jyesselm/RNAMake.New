//
// Created by Joseph Yesselman on 11/3/17.
//
#ifndef RNAMAKE_NEW_RNA_SEGMENT_H
#define RNAMAKE_NEW_RNA_SEGMENT_H

#include <util/segment_type.h>
#include <secondary_structure/pose.h>

namespace secondary_structure {

class Segment : public Pose {

public:
    Segment(
            Structure const & s,
            Basepairs const & basepairs,
            Basepairs const & ends,
            base::SimpleStringCOPs const & end_ids,
            base::SimpleStringCOP name,
            util::SegmentType type,
            Index block_end):
            secondary_structure::Pose(s, basepairs, ends, end_ids, name) {}

    Segment(
            Pose const & p,
            util::SegmentType type,
            Index block_end):
            secondary_structure::Pose(p) {}


};

typedef std::shared_ptr<Segment> SegmentOP;

}



#endif //RNAMAKE_NEW_RNA_SEGMENT_H
