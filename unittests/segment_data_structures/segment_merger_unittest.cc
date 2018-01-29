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
    auto rts = all_atom::ResidueTypeSet();
    auto db_path = base::resources_path() + "/motif_libraries/ideal_helices.db";
    auto seg_lib = resource_management::SegmentSqliteLibrary(db_path, "data_table", rts);

    auto seg1 = seg_lib.get_segment(StringStringMap{{"name","HELIX.IDEAL.2"}});

    resource_management::ResourceManager rm;
    auto sg = all_atom::SegmentGraph(rm);
    sg.add_segment(*seg1);

    for(int i = 0; i < 10; i++) {
        auto seg = seg_lib.get_segment(StringStringMap{{"name","HELIX.IDEAL.2"}});
        sg.add_segment(*seg, i, sg.get_segment_end_name(i, 1));
    }

    auto sm = all_atom::SegmentMerger(rm);
    auto merged_sg = sm.merge(sg);



    //sg.write_nodes_to_pdbs("test");


}