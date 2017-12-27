//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include "../common.hpp"

#include <all_atom/residue_type_set.h>
#include <all_atom/segment_factory.h>

TEST_CASE( "Test all atom segment", "[AllAtomSegments]" ) {
    auto rts = std::make_shared<all_atom::ResidueTypeSet>();
    auto seg_factory = all_atom::SegmentFactory(rts);
    auto path = base::unittest_resources_path() + "/all_atom/HELIX.IDEAL.2/HELIX.IDEAL.2.pdb";
    auto seg = seg_factory.segment_from_pdb(path, util::SegmentType::HELIX, false);



}
