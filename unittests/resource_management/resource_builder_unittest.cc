//
// Created by Joseph Yesselman on 1/3/18.
//

#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <all_atom/segment_factory.h>
#include <resource_management/resource_builder.h>

TEST_CASE( "Test resource builder", "[ResourceBuilder]" ) {
    auto rts   = all_atom::ResidueTypeSet();
    auto seg_f = all_atom::SegmentFactory(rts);

    auto path  = "/Users/jyesselm/projects/RNAMake/rnamake/resources/motifs";
    auto rb    = resource_management::ResourceBuilder(seg_f, path);
    rb.build_ideal_helices();

}