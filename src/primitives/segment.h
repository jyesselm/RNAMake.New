//
// Created by Joseph Yesselman on 12/17/17.
//

#ifndef RNAMAKE_NEW_SEGMENT_H
#define RNAMAKE_NEW_SEGMENT_H

#include <util/segment_type.h>
#include <primitives/pose.h>

namespace primitives {

template<typename BPtype, typename Structuretype, typename Chaintype, typename Restype>
class Segment : public Pose<BPtype, Structuretype, Chaintype, Restype> {
private:
    typedef Pose<BPtype, Structuretype, Chaintype, Restype> BaseClass;

public:
    inline
    Segment(
            Structuretype const & structure,
            std::vector<BPtype> const & basepairs,
            std::vector<BPtype> const & ends,
            base::SimpleStringCOPs const & end_ids,
            base::SimpleStringCOP name,
            util::SegmentType segment_type,
            Index aligned_end):
            BaseClass(structure, basepairs, ends, end_ids, name),
            segment_type_(segment_type),
            aligned_end_(aligned_end) {}

protected:
    util::SegmentType segment_type_;
    Index aligned_end_;
};

}


#endif //RNAMAKE_NEW_SEGMENT_H