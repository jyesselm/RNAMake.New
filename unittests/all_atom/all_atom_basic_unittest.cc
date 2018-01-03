//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include <dirent.h>
#include <fstream>
#include "../common.hpp"

#include <base/logger.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/pose.h>

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>

#include <util/sqlite/connection.h>

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
            auto struc2 = all_atom::Structure(j, *rts);
            REQUIRE(struc->is_equal(struc2, false));
        }
    }

    SECTION("test pose") {

        auto db = util::sqlite::Database(":memory:");
        auto conn = util::sqlite::Connection(db);

        auto tb = util::sqlite::TableDetails("data_table");
        tb.add_column("id", "INT", true);
        tb.add_column("str", "TEXT");
        util::sqlite::create_table(conn, tb);

        int level = Z_BEST_COMPRESSION; // Z_DEFAULT_COMPRESSION is the default if no arg is passed
        int strategy = Z_DEFAULT_STRATEGY; // Z_DEFAULT_STRATEGY is the defaul if no arg is passed

        auto str = String("hello hello hello hello hello");
        int size = str.length();
        auto compressed_data = gzip::compress(str.c_str(), size, level, strategy);

        auto datas = std::vector<Strings>();
        std::cout << compressed_data << std::endl;
        datas.push_back(Strings{"1", compressed_data});
        util::sqlite::insert_many(conn, "data_table", datas);


        exit(0);

        /*auto p = all_atom::get_pose_from_pdb(path, parser);
        auto j = p->get_json();
        //std:cout << j.dump_minimized() << std::endl;
        //auto p2 = all_atom::Pose(j, *rts);

        int level = Z_BEST_COMPRESSION; // Z_DEFAULT_COMPRESSION is the default if no arg is passed
        int strategy = Z_DEFAULT_STRATEGY; // Z_DEFAULT_STRATEGY is the defaul if no arg is passed

        int size = j.dump_minimized().length();
        auto compressed_data = gzip::compress(j.dump_minimized().c_str(), size, level, strategy);


        ofstream out ("test.bin");
        out << compressed_data.size();
        out.write(compressed_data.c_str(), compressed_data.size());
        out.close();

        ifstream in ("test.bin");
        char * buff = new char[5];
        in.read(buff, 5);
        std::cout << buff << std::endl;*/

        //auto decompressed_data = gzip::decompress(compressed_data.data(), compressed_data.size());


    }

}