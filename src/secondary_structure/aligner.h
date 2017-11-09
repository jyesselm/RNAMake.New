//
// Created by Joseph Yesselman on 11/4/17.
//

#ifndef RNAMAKE_NEW_ALIGNER_H
#define RNAMAKE_NEW_ALIGNER_H

#include <secondary_structure/rna_segment.h>

namespace secondary_structure {

class Aligner {
public:
    Aligner() {}

    ~Aligner() {}

public:
    void
    align(
            Basepair const & ref_bp,
            RNASegmentOP & rs) { }

    RNASegmentOP
    get_aligned(
            Basepair const & ref_bp,
            RNASegment const & rs) {
        return std::make_shared<RNASegment>(rs);
    }
};


}

#endif //RNAMAKE_NEW_ALIGNER_H
