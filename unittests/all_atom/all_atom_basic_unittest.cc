//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include <dirent.h>
#include <fstream>
#include "../common.hpp"

#include <base/log.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/pose.h>

#include <state/residue.h>

#include <util/sqlite/connection.h>

struct UnitestData : BasicUnitestData {
    UnitestData():
            BasicUnitestData(),
            rts(all_atom::ResidueTypeSet()),
            parser(all_atom::PDBParser(rts)){
    }

    all_atom::ResidueTypeSet rts;
    all_atom::PDBParser parser;
};

typedef UnitestSingleton<UnitestData> Singleton;

TEST_CASE( "testing basic all atom classes", "[AllAtom]" ) {

    // variables I dont want to keep remaking!
    auto & rts    = Singleton::get().rts;
    auto & parser = Singleton::get().parser;
    auto path = base::unittest_resources_path() + "/all_atom/p4p6.pdb";
    auto residues = parser.parse(path);

    SECTION("testing atoms") {
        auto a = residues->RNA_residues[0]->get_atom("P");

        SECTION("test construction") {
            auto a2 = all_atom::Atom(a.get_name(), a.get_coords());
            REQUIRE(a == a2);
        }

        SECTION("test copy") {
            auto a2 = all_atom::Atom(a);
            REQUIRE(a == a2);

            auto p = math::Point(2, 2, 2);
            a2.move(p);

            REQUIRE(a != a2);
        }

        SECTION("test json") {
            auto j = a.get_json();
            auto a2 = all_atom::Atom(j);
            REQUIRE(a == a2);
        }
    }

    SECTION("test residues") {
        auto r = *(residues->RNA_residues[0]);

        SECTION("test construction") {
            auto atoms = all_atom::Atoms();
            for(auto const & a : r) { atoms.push_back(a); }
            auto r2 = all_atom::Residue(r.get_name(), r.get_num(), r.get_chain_id(), r.get_i_code(),
                                        rts.get_residue_type(r.get_res_name()), atoms, r.get_uuid());
            REQUIRE(r == r2);
        }

        SECTION("test copy") {
            auto r2 = all_atom::Residue(r);
            REQUIRE(r == r2);

            auto p = math::Point(2, 2, 2);
            r2.move(p);

            REQUIRE(r != r2);
        }

        SECTION("test json") {
            auto j = r.get_json();
            auto r2 = all_atom::Residue(j, rts);
            REQUIRE(r.is_equal(r2, false));
        }

        SECTION("test state generation") {
            auto beads = util::Beads{r.bead_begin(), r.bead_end()};
            auto r_s = state::Residue(r.get_name(), r.get_num(), r.get_chain_id(), r.get_i_code(),
                                      beads, r.get_uuid());

            REQUIRE(r_s.get_name() == r.get_name());
            REQUIRE(r_s.get_num() == r.get_num());
            REQUIRE(r_s.get_chain_id() == r.get_chain_id());
            REQUIRE(r_s.get_i_code() == r.get_i_code());
            REQUIRE(r_s.get_uuid() == r.get_uuid());

            SECTION("test transforms") {
                auto p = math::Point(10, 0, 0);
                r.move(p);
                r_s.move(p);

                REQUIRE(r.get_bead(util::BeadType::BASE)  == r_s.get_bead(util::BeadType::BASE));
                REQUIRE(r.get_bead(util::BeadType::SUGAR) == r_s.get_bead(util::BeadType::SUGAR));
                REQUIRE(r.get_bead(util::BeadType::PHOS)  == r_s.get_bead(util::BeadType::PHOS));

                auto m = math::get_random_rotation_matrix();

                r.transform(m, p);
                r_s.transform(m, p);

                REQUIRE(r.get_bead(util::BeadType::BASE)  == r_s.get_bead(util::BeadType::BASE));
                REQUIRE(r.get_bead(util::BeadType::SUGAR) == r_s.get_bead(util::BeadType::SUGAR));
                REQUIRE(r.get_bead(util::BeadType::PHOS)  == r_s.get_bead(util::BeadType::PHOS));

            }

        }

    }

    SECTION("test chains") {
        auto res = all_atom::Residues();
        res.push_back(*residues->RNA_residues[0]);
        res.push_back(*residues->RNA_residues[1]);
        res.push_back(*residues->RNA_residues[2]);
        auto c = all_atom::Chain(res);

        SECTION("test copy") {
            auto c2 = all_atom::Chain(c);
            REQUIRE(c == c2);

            auto p = math::Point(2, 2, 2);
            c2.move(p);

            REQUIRE(c != c2);
        }

        SECTION("test json") {
            auto j = c.get_json();
            auto c2 = all_atom::Chain(j, rts);
            REQUIRE(c.is_equal(c2, false));
        }
    }

    SECTION("test structure") {
        auto struc = all_atom::get_structure_from_residues(residues->RNA_residues);
        auto j = struc->get_json();

        SECTION("test copy") {
            auto struc2 = all_atom::Structure(*struc);
            REQUIRE(*struc == struc2);

            auto p = math::Point(2, 2, 2);
            struc2.move(p);

            REQUIRE(*struc != struc2);
        }

        SECTION("test json") {
            auto j = struc->get_json();
            auto struc2 = all_atom::Structure(j, rts);
            REQUIRE(struc->is_equal(struc2, false));
        }
    }

    SECTION("test pose") {
        auto p = all_atom::get_pose_from_pdb(path, parser);
        auto j = p->get_json();
        auto p2 = all_atom::Pose(j, rts);
        REQUIRE(p->is_equal(p2, false));

        auto t = math::Point(2, 2, 2);
        p2.move(t);

        REQUIRE(!p->is_equal(p2, false));



    }

}