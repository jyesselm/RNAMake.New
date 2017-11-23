//
// Created by Joseph Yesselman on 11/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <base/logger.h>

TEST_CASE( "Test logging functions", "[Logger]" ) {
    SET_LOG_LEVEL(Loglevel::INFO);
    LOG_INFO("PDBParserUnittest", "test");
    LOG_WARNING("PDBParserUnittest", "test");
    LOG_ERROR("PDBParserUnittest", "test");
    SET_LOG_LEVEL(Loglevel::WARNING);
    LOG_INFO("PDBParserUnittest", "test");

}