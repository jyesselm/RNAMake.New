//
// Created by Joseph Yesselman on 12/26/17.
//

#ifndef RNAMAKE_NEW_ALL_ATOM_ALIGNER_H
#define RNAMAKE_NEW_ALL_ATOM_ALIGNER_H

#include <base/logger.h>
#include <primitives/aligner.h>
#include <all_atom/segment.h>

namespace all_atom {

class Aligner : public primitives::Aligner<Segment, Basepair>  {
public:
    typedef primitives::Aligner<Segment, Basepair> BaseClass;
public:
    Aligner():
            BaseClass() {
        dummy_ = math::Point();
    }

    ~Aligner() {}

public:
    virtual
    void
    align(
            Basepair const & ref_bp,
            Segment & seg) {

        r_ = dot(ref_bp.get_ref_frame().get_transposed(), seg.get_aligned_end().get_ref_frame());
        r_.unitarize();
        transpose(r_, r_t);
        t_ = -ref_bp.get_center();
        seg.transform(r_t, t_, dummy_);
        seg.move(ref_bp.get_center() - seg.get_aligned_end().get_center());

        sugar_dist_ = ref_bp.get_res1_c1_prime_coord().distance(seg.get_aligned_end().get_res2_c1_prime_coord());
        if(sugar_dist_ > 5) {
            LOG_WARNING("SegmentAligener",
                        "difference in sugar c1' coords between reference and aligned is greater than 5. "
                        "This could lead to alignment issues!");
            return;
        }

        sugar_diff_1_ =  ref_bp.get_res1_c1_prime_coord() - seg.get_aligned_end().get_res2_c1_prime_coord();
        sugar_diff_2_ =  ref_bp.get_res2_c1_prime_coord() - seg.get_aligned_end().get_res1_c1_prime_coord();
        avg_sugar_diff_ = (sugar_diff_1_ + sugar_diff_2_) / 2;
        seg.move(avg_sugar_diff_);

    }

    virtual
    SegmentTypeOP
    get_aligned(
            Basepair const & ref_bp,
            Segment const & seg) {
        auto seg_copy = std::make_shared<Segment>(seg);
        align(ref_bp, *seg_copy);
        return seg_copy;
    }

private:
    math::Matrix r_, r_t;
    math::Point t_, dummy_, sugar_diff_1_, sugar_diff_2_, avg_sugar_diff_;
    double sugar_dist_;

};

}


#endif //RNAMAKE_NEW_ALIGNER_H
