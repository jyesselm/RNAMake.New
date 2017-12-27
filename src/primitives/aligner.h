//
// Created by Joseph Yesselman on 12/26/17.
//

#ifndef RNAMAKE_NEW_ALIGNER_H
#define RNAMAKE_NEW_ALIGNER_H


namespace primitives {

class Aligner {
public:
    Aligner() {}

    ~Aligner() {}

public:
    void
    align(
            Basepair const & ref_bp,
            SegmentOP & rs) {}

    SegmentOP
    get_aligned(
            Basepair const & ref_bp,
            SegmentOP const & rs) {
        return std::make_shared<Segment>(*rs);
    }
};

}


#endif //RNAMAKE_NEW_ALIGNER_H
