//
// Created by Joseph Yesselman on 1/3/18.
//

#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <resource_management/sqlite_library.h>

TEST_CASE( "Test basic sqlite library", "[Sqlitelibrary]" ) {
    auto db_path = base::unittest_resources_path() + "/resource_management/test.db";
    auto sqlib = resource_management::SqliteLibrary(db_path, "data_table");

    SECTION("database file must exist") {
        REQUIRE_THROWS_AS(resource_management::SqliteLibrary("test_fake.db", "data_table"),
                          resource_management::SqliteLibraryException);

    }

}