//
// Created by Joseph Yesselman on 1/22/18.
//


#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <base/log.h>
#include <resource_management/resource_manager.h>
#include <resource_management/segment_sqlite_library.h>
#include <segment_data_structures/segment_graph.h>


TEST_CASE( "Test Graph Data Structure ", "[Graph]" ) {
    init_unittest_safe_logging();

    auto rts = all_atom::ResidueTypeSet();
    auto db_path = base::resources_path() + "/motif_libraries/ideal_helices.db";
    auto seg_lib = resource_management::SegmentSqliteLibrary(db_path, "data_table", rts);

    auto seg1 = seg_lib.get_segment(StringStringMap{{"name","HELIX.IDEAL.2"}});

    auto sg = all_atom::SegmentGraph();
    sg.add_segment(*seg1);

    for(int i = 0; i < 10; i++) {
        auto seg = seg_lib.get_segment(StringStringMap{{"name","HELIX.IDEAL.2"}});
        sg.add_segment(*seg, i, sg.get_segment_end_name(i, 1));
    }

    REQUIRE(sg.get_num_segments() == 11);

    sg.remove_segment(5);
    auto path = Indexes();
    auto target = Indexes{0, 1, 2, 3, 4, 6, 7, 8, 9, 10};
    for(auto const & n : sg) {
        path.push_back(n->index());
    }
    REQUIRE(path == target);

    SECTION("test replace idealized helices") {
        auto sg = all_atom::SegmentGraph();
        resource_management::ResourceManager rm;
        auto seg1 = rm.get_segment(StringStringMap{{"name","HELIX.IDEAL.10"}});
        sg.add_segment(*seg1);

        auto new_g = all_atom::convert_ideal_helices_to_basepair_steps(sg, rm);
        REQUIRE(new_g->get_num_segments() == 11);
    }

    SECTION("test replace idealized helices with motifs") {
        auto sg = all_atom::SegmentGraph();
        resource_management::ResourceManager rm;

        auto seg1 = rm.get_segment(StringStringMap{{"name","HELIX.IDEAL.5"}});
        auto seg2 = rm.get_segment(StringStringMap{{"name","TWOWAY.1A34.0"}});
        auto seg3 = rm.get_segment(StringStringMap{{"name","HELIX.IDEAL.5"}});
        sg.add_segment(*seg1);
        sg.add_segment(*seg2, 0, sg.get_segment_end_name(0, 1));
        sg.add_segment(*seg3, 1, sg.get_segment_end_name(1, 1));

        auto new_g = all_atom::convert_ideal_helices_to_basepair_steps(sg, rm);
        new_g->write_nodes_to_pdbs("nodes");

    }

    //sg.write_nodes_to_pdbs("test");


}