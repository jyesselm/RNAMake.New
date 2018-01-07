//
// Created by Joseph Yesselman on 11/22/17.
//


#include <iostream>
#include "../common.hpp"
#include <base/log.h>
//#include <base/logger.h>

TEST_CASE( "Test logging functions", "[Logger]" ) {
    base::init_logging();

    LOGI << "Hello log!"; // short macro
    LOG_DEBUG << "Hello log!"; // long macro
    LOG(plog::debug) << "Hello log!"; // function-style macro

}