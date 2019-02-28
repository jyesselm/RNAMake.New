//
// Created by Joseph Yesselman on 2/13/19.
//

#include <iostream>
#include "../common.hpp"

#include <math/numerical.h>
#include <util/random_number_generator.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/segment_factory.h>
#include <state/aligner.h>

TEST_CASE( "Test states derivived from all atom segments", "[StateSegments]" ) {
    init_unittest_safe_logging();

    auto rts = all_atom::ResidueTypeSet();
    auto seg_factory = all_atom::SegmentFactory(rts);
    auto path = base::unittest_resources_path() + "/all_atom/HELIX.IDEAL.2/HELIX.IDEAL.2.pdb";
    auto seg = seg_factory.segment_from_pdb(path, util::SegmentType::HELIX, false);

    auto seg_state = seg->get_state();

    REQUIRE(seg_state->get_name() == seg->get_name());
    REQUIRE(seg_state->get_num_chains() == seg->get_num_chains());
    REQUIRE(seg_state->get_num_residues() == seg->get_num_residues());
    REQUIRE(seg_state->get_num_basepairs() == seg->get_num_basepairs());
    REQUIRE(seg_state->get_num_ends() == seg->get_num_ends());

    for(auto i = 0; i < seg->get_num_basepairs(); i++) {
        auto & bp = seg->get_basepair(i);
        auto & bp_s = seg_state->get_basepair(i);

        REQUIRE(bp.get_res1_uuid() == bp_s.get_res1_uuid());
        REQUIRE(bp.get_res2_uuid() == bp_s.get_res2_uuid());
        REQUIRE(bp.get_uuid() == bp_s.get_uuid());
        REQUIRE(math::are_points_equal(bp.get_center(), bp_s.get_center()));
        REQUIRE(math::are_points_equal(bp.get_res1_c1_prime_coord(), bp_s.get_res1_c1_prime_coord()));
        REQUIRE(math::are_points_equal(bp.get_res2_c1_prime_coord(), bp_s.get_res2_c1_prime_coord()));
    }

    SECTION("test copy") {
        auto seg_state_2 = state::Segment(*seg_state);

        REQUIRE(*seg_state == seg_state_2);

        auto p = math::Point(2, 2, 2);
        seg_state_2.move(p);

        REQUIRE(*seg_state != seg_state_2);
    }

    SECTION("test json") {
        auto j = seg_state->get_json();
        auto seg_state_2 = state::Segment(j);

        REQUIRE(seg_state->is_equal(seg_state_2, CheckUUID::NO));

        auto p = math::Point(2, 2, 2);
        seg_state_2.move(p);

        REQUIRE(!seg_state->is_equal(seg_state_2, CheckUUID::NO));

    }

    SECTION("test sterics") {
        auto seg_state_2 = state::Segment(*seg_state);

        auto steric_clash_count = seg_state_2.get_num_steric_clashes(*seg_state);

        REQUIRE(steric_clash_count > 1);
        REQUIRE(seg_state_2.steric_clash(*seg_state) == true);

        auto p = math::Point(100, 100, 100);
        seg_state_2.move(p);
        steric_clash_count = seg_state_2.get_num_steric_clashes(*seg_state);

        REQUIRE(steric_clash_count == 0);
        REQUIRE(seg_state_2.steric_clash(*seg_state) == false);
    }

    SECTION("test get secondary structure") {
        auto seg_ss = seg_state->get_secondary_structure();

        REQUIRE(seg_ss->get_name() == seg_state->get_name());
        REQUIRE(seg_ss->get_num_chains() == seg_state->get_num_chains());
        REQUIRE(seg_ss->get_num_residues() == seg_state->get_num_residues());
        REQUIRE(seg_ss->get_num_basepairs() == seg_state->get_num_basepairs());
        REQUIRE(seg_ss->get_num_ends() == seg_state->get_num_ends());

        //secondary structure specific
        REQUIRE(seg_ss->get_dot_bracket() == seg_state->get_dot_bracket());
        REQUIRE(seg_ss->get_sequence() == seg_state->get_sequence());

        // can change sequence in secondary structure, should be different from all_atom segment
        seg_ss->set_residue_identity(0, 'A');

        REQUIRE(seg_ss->get_sequence() != seg_state->get_sequence());
    }

    SECTION("test identical behavior between all_atom and state") {
        auto rng = util::RandomNumberGenerator();
        for (int i = 0; i < 100; i++) {
            auto rot = math::get_random_rotation_matrix();
            auto dist = util::get_random_point(rng, 10);

            seg->transform(rot, dist);
            seg_state->transform(rot, dist);

        }

        REQUIRE(math::are_points_equal(seg->get_end(0).get_center(), seg_state->get_end(0).get_center()));
        REQUIRE(math::are_matrices_equal(seg->get_end(0).get_ref_frame(), seg_state->get_end(0).get_ref_frame()));

        REQUIRE(math::are_points_equal(seg->get_end(1).get_center(), seg_state->get_end(1).get_center()));
        REQUIRE(math::are_matrices_equal(seg->get_end(1).get_ref_frame(), seg_state->get_end(1).get_ref_frame()));
    }

    SECTION("test aligner") {
        auto aligner = state::Aligner();
        auto all_atom_aligner = all_atom::Aligner();
        auto hybrid_aligner = primitives::Aligner<all_atom::Segment, state::Basepair>();
        auto seg_state_1 = seg->get_state();
        auto seg_state_2 = seg->get_state();
        auto seg_copy_1 = all_atom::Segment(*seg);
        auto seg_copy_2 = all_atom::Segment(*seg);

        aligner.align(seg_state_1->get_end(1), *seg_state_2);
        all_atom_aligner.align(seg->get_end(1), seg_copy_1);

        // move all_atom motif to where state is oriented
        hybrid_aligner.align(seg_state_2->get_aligned_end(), seg_copy_2);

        REQUIRE(math::are_points_equal(seg_copy_1.get_end(1).get_center(), seg_copy_2.get_end(1).get_center()));
        REQUIRE(math::are_matrices_equal(seg_copy_1.get_end(1).get_ref_frame(), seg_copy_2.get_end(1).get_ref_frame()));


    }

}



































