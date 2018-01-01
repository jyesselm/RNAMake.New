//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include "../common.hpp"

#include <base/logger.h>
#include <math/numerical.h>
#include <util/random_number_generator.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/basepair.h>
#include <all_atom/structure.h>

TEST_CASE( "Test all atom base pair ", "[Basepair]" ) {
    auto rts = std::make_shared<all_atom::ResidueTypeSet>();
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

        auto j = bp_res[0].get_atom("P").get_json();



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

}
