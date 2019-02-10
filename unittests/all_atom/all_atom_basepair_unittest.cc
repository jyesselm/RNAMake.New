//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include "../common.hpp"

#include <math/numerical.h>
#include <util/random_number_generator.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/basepair.h>
#include <all_atom/structure.h>
#include <state/basepair.h>

TEST_CASE( "Test all atom base pair ", "[Basepair]" ) {
    init_unittest_safe_logging();

    auto rts = all_atom::ResidueTypeSet();
    auto parser = all_atom::PDBParser(rts);

    auto path = base::unittest_resources_path() + "/all_atom/p4p6.pdb";
    auto residues = parser.parse(path);
    auto s = all_atom::get_structure_from_residues(residues->RNA_residues);

    auto x = util::X3dna();
    auto x3dna_basepairs = x.get_basepairs(path);

    auto bps = all_atom::get_basepairs_from_x3dna(x3dna_basepairs, *s);

    SECTION("test sync between basepair and residues") {

        auto bp = bps->at(0);
        auto bp_res = all_atom::Residues();
        bp_res.push_back(s->get_residue(bp.get_res1_uuid()));
        bp_res.push_back(s->get_residue(bp.get_res2_uuid()));

        auto rng = util::RandomNumberGenerator();
        for (int i = 0; i < 100; i++) {
            auto rot = math::get_random_rotation_matrix();
            auto dist = util::get_random_point(rng, 10);

            bp.transform(rot, dist);
            bp_res[0].transform(rot, dist);
            bp_res[1].transform(rot, dist);
        }

        // calculate center of base pair
        auto center = math::Point();
        auto count = 0;
        for(auto const & a : bp_res[0]) {
            center += a.get_coords();
            count += 1;
        }
        for(auto const & a : bp_res[1]) {
            center += a.get_coords();
            count += 1;
        }
        center /= count;

        REQUIRE(math::are_points_equal(bp.get_center(), center));

        REQUIRE(math::are_points_equal(bp.get_res1_c1_prime_coord(),
                                       bp_res[0].get_coords("C1'")));

        REQUIRE(math::are_points_equal(bp.get_res2_c1_prime_coord(),
                                       bp_res[1].get_coords("C1'")));

    }

    SECTION("test copy") {
        auto bp = bps->at(0);
        auto bp2 = all_atom::Basepair(bp);
        REQUIRE(bp == bp2);
    }

    SECTION("test json") {
        auto bp = bps->at(0);
        auto j = bp.get_json();
        auto bp2 = all_atom::Basepair(j, bp.get_res1_uuid(), bp.get_res2_uuid(), bp.get_uuid());

        REQUIRE(bp.is_equal(bp2, false));
    }

    SECTION("test swap residues") {
        auto bp = bps->at(0);
        auto bp2 = all_atom::Basepair(bp);
        bp2.swap_residue_positions();

        // should not be equal now
        REQUIRE(bp != bp2);

        // should be swapped
        REQUIRE(bp.get_res1_uuid() == bp2.get_res2_uuid());
        REQUIRE(bp.get_res2_uuid() == bp2.get_res1_uuid());

    }


    SECTION("test state creation") {
        auto bp = bps->at(0);
        auto bp_state = state::Basepair(bp.get_res1_uuid(), bp.get_res2_uuid(), bp.get_uuid(), bp.get_bp_type(),
                                        bp.get_name(), bp.get_ref_frame(), bp.get_center(), bp.get_c1_prime_coords());

        REQUIRE(bp.get_res1_uuid() == bp_state.get_res1_uuid());
        REQUIRE(bp.get_res2_uuid() == bp_state.get_res2_uuid());
        REQUIRE(bp.get_uuid() == bp_state.get_uuid());
        REQUIRE(math::are_points_equal(bp.get_center(), bp_state.get_center()));
        REQUIRE(math::are_points_equal(bp.get_res1_c1_prime_coord(), bp_state.get_res1_c1_prime_coord()));
        REQUIRE(math::are_points_equal(bp.get_res2_c1_prime_coord(), bp_state.get_res2_c1_prime_coord()));

        SECTION("test copying") {
            auto bp_state2 = state::Basepair(bp_state);

            REQUIRE(bp_state2.get_res1_uuid() == bp_state.get_res1_uuid());
            REQUIRE(bp_state2.get_res2_uuid() == bp_state.get_res2_uuid());
            REQUIRE(bp_state2.get_uuid() == bp_state.get_uuid());
            REQUIRE(math::are_points_equal(bp_state2.get_center(), bp_state.get_center()));
            REQUIRE(math::are_points_equal(bp_state2.get_res1_c1_prime_coord(), bp_state.get_res1_c1_prime_coord()));
            REQUIRE(math::are_points_equal(bp_state2.get_res2_c1_prime_coord(), bp_state.get_res2_c1_prime_coord()));

            bp_state.is_equal(bp_state2, CheckUUID::YES);
        }

        SECTION("test json") {

        }

    }

}





























