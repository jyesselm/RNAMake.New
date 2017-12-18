//
// Created by Joseph Yesselman on 12/15/17.
//

#ifndef RNAMAKE_NEW_ALL_ATOM_BASEPAIR_H
#define RNAMAKE_NEW_ALL_ATOM_BASEPAIR_H

#include <base/vector_container.h>
#include <math/xyz_matrix.h>
#include <math/xyz_vector.h>
#include <util/x3dna.h>
#include <primitives/basepair.h>
#include <all_atom/residue.h>

namespace all_atom {

class Basepair : public primitives::Basepair {
public:
    inline
    Basepair(
            util::Uuid const & res1_uuid,
            util::Uuid const & res2_uuid,
            util::Uuid const & uuid,
            primitives::BasepairType bp_type,
            base::SimpleStringCOP name,
            util::X3dnaBPType x3dna_type,
            math::Matrix const & ref_frame,
            math::Point const & center,
            math::Points const & c1_prime_coords):
            primitives::Basepair(res1_uuid, res2_uuid, uuid, bp_type, name),
            ref_frame_(ref_frame),
            center_(center),
            c1_prime_coords_(c1_prime_coords),
            x3dna_type_(x3dna_type) {}

    /*inline
    Basepair(
            Basepair const & bp):
            primitives::Basepair(bp) {}*/

public:

    inline
    void
    move(
            math::Point const & p) {
        center_ = center_ + p;
        c1_prime_coords_[0] = c1_prime_coords_[0] + p;
        c1_prime_coords_[1] = c1_prime_coords_[1] + p;
    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t,
            math::Point & dummy) {
        math::dot_vector(r, center_, dummy);
        center_ = dummy + t;

        math::dot_vector(r, c1_prime_coords_[0], dummy);
        c1_prime_coords_[0] = dummy + t;

        math::dot_vector(r, c1_prime_coords_[1], dummy);
        c1_prime_coords_[1] = dummy + t;

        ref_frame_ = math::dot(ref_frame_, r);
        ref_frame_.unitarize();
    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t) {
        auto dummy = math::Point();
        transform(r, t, dummy);
    }


public: // getters
    inline
    math::Matrix const &
    get_ref_frame() const { return ref_frame_; }

    inline
    math::Point const &
    get_center() const { return center_; }

    inline
    math::Points const &
    get_c1_prime_coords() const { return c1_prime_coords_; }

    inline
    math::Point const &
    get_res1_c1_prime_coord() const { return c1_prime_coords_[0]; }

    inline
    math::Point const &
    get_res2_c1_prime_coord() const { return c1_prime_coords_[1]; }


private:
    math::Point center_;
    math::Points c1_prime_coords_;
    math::Matrix ref_frame_;
    util::X3dnaBPType x3dna_type_;

};

typedef std::vector<Basepair> Basepairs;

inline
String
generate_bp_name(
        Residue const & res1,
        Residue const & res2) {
    return primitives::generate_bp_name<Residue>(res1, res2);
}

primitives::BasepairType
generate_bp_type(
        Residue const &,
        Residue const &,
        util::X3dnaBPType);


}

#endif //RNAMAKE_NEW_ALL_ATOM_BASEPAIR_H
