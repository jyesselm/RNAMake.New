//
// Created by Joseph Yesselman on 11/4/17.
//

#ifndef RNAMAKE_NEW_SS_ALIGNER_H
#define RNAMAKE_NEW_SS_ALIGNER_H

#include <secondary_structure/segment.h>

namespace secondary_structure {

class Aligner {
public:
    Aligner() {}

    ~Aligner() {}

public:
    void
    align(
            Basepair const & ref_bp,
            SegmentOP & rs) const { }

    SegmentOP
    get_aligned(
            Basepair const & ref_bp,
            SegmentOP const & rs) const {
        return std::make_shared<Segment>(*rs);
    }
};


}

#endif //RNAMAKE_NEW_ALIGNER_H
