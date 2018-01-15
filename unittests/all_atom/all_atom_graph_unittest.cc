//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include "../common.hpp"

#include <base/log.h>
#include <data_structures/segment_graph.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/segment_factory.h>

TEST_CASE( "Test all atom segment", "[AllAtomSegments]" ) {
    init_unittest_safe_logging();

    auto rts = all_atom::ResidueTypeSet();
    auto seg_factory = all_atom::SegmentFactory(rts);
    auto path = base::unittest_resources_path() + "/all_atom/HELIX.IDEAL.2/HELIX.IDEAL.2.pdb";
    auto seg = seg_factory.segment_from_pdb(path, util::SegmentType::HELIX, false);

    auto g = data_structures::SegmentGraph<all_atom::Segment, all_atom::Aligner>();
    g.add_segment(seg);
    g.add_segment(seg, 0, g.get_segment_end_name(0, 1));

    for(auto const & n : g) {
        n.data->write_pdb("test."+std::to_string(n.index)+".pdb");
    }
}
