//
// Created by Joseph Yesselman on 1/3/18.
//

#include <iostream>
#include "../common.hpp"

#include <resource_management/resource_manager.h>

TEST_CASE( "Test resource manager", "[ResourceManager]" ) {
    init_unittest_safe_logging(base::LogLevel::WARN);

    resource_management::ResourceManager rm;
    //okay !
    auto & rm_ref = rm;
    // not okay
    //auto rm1 = rm;

    // get segment
    auto seg = rm.get_segment(StringStringMap{{"name", "HELIX.IDEAL.2"}});

}