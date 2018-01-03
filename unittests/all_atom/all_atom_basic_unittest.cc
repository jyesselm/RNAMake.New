//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include <dirent.h>
#include <fstream>
#include "../common.hpp"

#include <base/logger.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/structure.h>



TEST_CASE( "testing basic all atom classes", "[AllAtom]" ) {

    auto rts = std::make_shared<all_atom::ResidueTypeSet>();
    auto parser = all_atom::PDBParser(rts);
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
                                        rts->get_residue_type(r.get_res_name()), atoms, r.get_uuid());
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
            auto r2 = all_atom::Residue(j, *rts);
            REQUIRE(r.is_equal(r2, false));
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
            auto c2 = all_atom::Chain(j, *rts);
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

            //std::cout << struc2.get_residue(0).get_json() << std::endl;
            //std::cout << struc->get_residue(0).get_json() << std::endl;

            REQUIRE(*struc != struc2);
        }

        SECTION("test json") {
            auto j = struc->get_json();
            //std::cout << j << std::endl;
            auto struc2 = all_atom::Structure(j, *rts);
            REQUIRE(struc->is_equal(struc2, false));

        }

    }

}