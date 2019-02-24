//
// Created by Joseph Yesselman on 2/13/19.
//

#ifndef RNAMAKE_NEW_STATE_ALIGNER_H
#define RNAMAKE_NEW_STATE_ALIGNER_H

#include <base/log.h>
#include <primitives/aligner.h>
#include <state/segment.h>

namespace state {

class Aligner : public primitives::Aligner<Segment, Basepair> {
public:
    typedef primitives::Aligner<Segment, Basepair> BaseClass;
public:
    Aligner():
            BaseClass() {
        dummy_ = math::Point();
    }

    ~Aligner() {}

public:


private:
    mutable math::Matrix r_, r_t;
    mutable math::Point t_, dummy_, sugar_diff_1_, sugar_diff_2_, avg_sugar_diff_;
    mutable double sugar_dist_1_, sugar_dist_2_;
};

}


#endif //RNAMAKE_NEW_STATE_ALIGNER_H
