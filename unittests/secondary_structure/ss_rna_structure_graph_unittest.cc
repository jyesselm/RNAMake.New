//
// Created by Joseph Yesselman on 10/31/17.
//

//
// Created by Joseph Yesselman on 10/27/17.
//

#include <iostream>
#include "../common.hpp"

#include <data_structures/segment_graph.h>
#include <secondary_structure/aligner.h>
#include <secondary_structure/parser.h>


TEST_CASE( "Test SS Graph", "[RNAStructureTypeGraph]" ) {
    auto p = secondary_structure::Parser();
    auto s1 = p.parse_to_segment("GG&CC", "((&))");
    auto s2 = p.parse_to_segment("AA&UU", "((&))");

    /*auto g = data_structures::RNASegmentTypeGraph<secondary_structure::RNASegment,
                                                  secondary_structure::Aligner>();
    g.add_rna_segment(rs1);
    g.add_rna_segment(rs2, 0, g.get_segment_end_name(0, 1));
    */
}
