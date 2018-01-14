//
// Created by Joseph Yesselman on 1/3/18.
//

#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <all_atom/residue_type_set.h>
#include <resource_management/sqlite_library.h>
#include <resource_management/segment_sqlite_library.h>

TEST_CASE( "Test basic sqlite library", "[Sqlitelibrary]" ) {
    auto db_path = base::unittest_resources_path() + "/resource_management/test.db";
    auto sqlib = resource_management::SqliteLibrary(db_path, "data_table");

    SECTION("database file must exist") {
        REQUIRE_THROWS_AS(resource_management::SqliteLibrary("test_fake.db", "data_table"),
                          resource_management::SqliteLibraryException);

    }

    SECTION("test segment databases") {
        auto rts = all_atom::ResidueTypeSet();
        auto db_path = base::resources_path() + "/motif_libraries/ideal_helices.db";
        auto seg_lib = resource_management::SegmentSqliteLibrary(db_path, "data_table", rts);

        auto seg = seg_lib.get_segment(StringStringMap{{"name","HELIX.IDEAL.2"}});
        REQUIRE(seg->get_name_str() == "HELIX.IDEAL.2");
    }

}