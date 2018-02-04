//
// Created by Joseph Yesselman on 1/22/18.
//


#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <base/log.h>
#include <resource_management/resource_manager.h>
#include <resource_management/segment_sqlite_library.h>
#include <segment_data_structures/segment_merger.h>


TEST_CASE( "Test Graph Data Structure ", "[Graph]" ) {
    resource_management::ResourceManager rm;
    auto seg1 = rm.get_segment(StringStringMap{{"name","HELIX.IDEAL.2"}});

    auto sg = all_atom::SegmentGraph();
    sg.add_segment(*seg1);

    for(int i = 0; i < 10; i++) {
        auto seg = rm.get_segment(StringStringMap{{"name","HELIX.IDEAL.2"}});
        sg.add_segment(*seg, i, sg.get_segment_end_name(i, 1));
    }

    auto sm = all_atom::SegmentMerger(rm);
    auto merged_sg = sm.merge(sg, "merged_graph");

    REQUIRE(merged_sg->get_num_ends() == 2);
    REQUIRE(merged_sg->get_num_chains() == 2);
    REQUIRE(merged_sg->get_name_str() == "merged_graph");

    // copies normally
    auto merged_copy = all_atom::Segment(*merged_sg);
    REQUIRE(merged_copy.is_equal(*merged_sg));

    auto j = merged_sg->get_json();
    merged_copy = all_atom::Segment(j, rm.get_residue_type_set());
    REQUIRE(merged_copy.is_equal(*merged_sg, false));


    //sg.write_nodes_to_pdbs("test");


}