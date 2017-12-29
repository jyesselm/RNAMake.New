//
// Created by Joseph Yesselman on 12/7/17.
//

#include <iostream>
#include "../common.hpp"

#include <base/paths.h>

TEST_CASE( "Test Basic Sqlite3 connections ", "[Sqlite3Connection]" ) {
    /*try { std::remove("test.db"); }
    catch (String const & e) {}

    auto conn = util::Sqlite3Connection();
    conn.connect("test.db");

    auto table_name = String("data_table");
    auto primary_key = String("id");
    auto keys = Strings{"word", "id"};
    auto types = Strings{"TEXT", "INT"};
    auto data = std::vector<Strings>{ {"the_word", "0"}, {"the", "1"}, {"hello", "2"} };

    conn.create_table(table_name, keys, types, primary_key, data);

    // table does not exist
    REQUIRE_THROWS_AS(conn.get_number_of_rows("fake_table"), util::Sqlite3ConnectionException);


    exit(0);

    SECTION( "test counting number of row in table") {

        auto count = conn.get_number_of_rows("data_table");
        REQUIRE(count == 3);
        // does repeated calls mess it up
        for (int i = 0; i < 100; i++) {
            count = conn.get_number_of_rows("data_table");
        }
        REQUIRE(count == 3);
    }*/

}