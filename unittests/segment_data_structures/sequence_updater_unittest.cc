//
// Created by Joseph Yesselman on 5/31/18.
//

//
// Created by Joseph Yesselman on 1/22/18.
//


#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <base/log.h>
#include <resource_management/resource_manager.h>
#include <segment_data_structures/sequence_updater.h>


TEST_CASE( "Test updating sequence in graphs ", "[Graph]" ) {
    init_unittest_safe_logging();

    resource_management::ResourceManager rm;
    auto seg1 = rm.get_segment(StringStringMap{{"name", "HELIX.IDEAL"}});

    auto sg = all_atom::SegmentGraph();
    sg.add_segment(*seg1);

    for (int i = 0; i < 2; i++) {
        auto seg = rm.get_segment(StringStringMap{{"name", "HELIX.IDEAL"}});
        sg.add_segment(*seg, i, sg.get_segment_end_name(i, 1));
    }

    auto ss_sg = all_atom::get_secondary_structure_graph(sg);

    auto su = secondary_structure::SequenceUpdater(rm);
    auto ss_sg_new = su.get_updated_graph(*ss_sg, "AAAA&UUUU");
    for(auto const & n : *ss_sg_new) {
        REQUIRE(n->data().get_sequence() == "AA&UU");
        REQUIRE(n->data().get_end_id(0)->get_str() == "AA_LL_UU_RR");
    }

}