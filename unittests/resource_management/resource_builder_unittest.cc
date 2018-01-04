//
// Created by Joseph Yesselman on 1/3/18.
//

#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <all_atom/segment_factory.h>
#include <resource_management/resource_builder.h>

TEST_CASE( "Test resource builder", "[ResourceBuilder]" ) {
    auto rts = std::make_shared<all_atom::ResidueTypeSet>();
    auto seg_f = std::make_shared<all_atom::SegmentFactory>(rts);
    auto rb = resource_management::ResourceBuilder(*seg_f);

}