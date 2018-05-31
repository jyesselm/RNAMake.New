//
// Created by Joseph Yesselman on 10/27/17.
//

#include <iostream>
#include "../common.hpp"

#include <secondary_structure/parser.h>

TEST_CASE( "Test Secondary Structure Parser ", "[SecondaryStructureParser]" ) {
    SECTION("test parse") {
        auto parser = secondary_structure::Parser();
        auto p = parser.parse_to_pose("GAC&GUC", "(((&)))");
        REQUIRE(p->get_num_residues() == 6);
        REQUIRE(p->get_num_ends() == 2);
        REQUIRE(p->get_num_basepairs() == 3);
    }

    SECTION("test no structure") {
        auto parser = secondary_structure::Parser();
        auto p = parser.parse_to_pose("AAAAU", ".....");
        REQUIRE(p->get_num_residues() == 5);
        REQUIRE(p->get_num_ends() == 0);
        REQUIRE(p->get_num_basepairs() == 0);
        REQUIRE(p->get_dot_bracket() == ".....");

        REQUIRE_THROWS_AS(parser.parse_to_segment("AAAAU", "....."),
                          secondary_structure::ParserException);
    }

    SECTION("test parse to segment") {
        auto parser = secondary_structure::Parser();
        auto seg = parser.parse_to_segment("GAC&GUC", "(((&)))");
        REQUIRE(seg->get_num_residues() == 6);
        REQUIRE(seg->get_segment_type() == util::SegmentType::SEGMENT);

    }

    SECTION("test sequence replacement") {


    }
 }
