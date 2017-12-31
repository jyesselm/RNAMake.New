//
// Created by Joseph Yesselman on 12/7/17.
//

#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <util/sqlite/connection.h>

TEST_CASE( "Test Basic Sqlite3 connections ", "[Sqlite3Connection]" ) {

    SECTION("test basic features of connection") {

        try { std::remove("test.db"); }
        catch (String const & e) {}

        auto db = util::sqlite::Database("test.db");
        auto q = util::sqlite::Connection(db);

        q.exec("CREATE TABLE data_table (word TEXT, id INT, PRIMARY KEY(id));");
        q.start_transaction();

        q.exec("INSERT INTO data_table( word, id) VALUES ('the_word','0');");
        q.exec("INSERT INTO data_table( word, id) VALUES ('the','1');");
        q.exec("INSERT INTO data_table( word, id) VALUES ('word','2');");

        q.commit_transaction();

        SECTION("test getting first row") {
            auto row = q.get_first_row("SELECT * FROM data_table");
            REQUIRE(row->at(0).get_name() == "word");
            REQUIRE(row->at(0).get_str() == "the_word");

            // cannot use auto needs "int" to figure out what the type is
            int id = row->at(1);
            REQUIRE(id == 0);

            //stop from weird casting ...
            REQUIRE_THROWS_AS(int id = row->at(0), util::sqlite::SqliteException);
        }
    }

    SECTION("test create table") {

        try { std::remove("test2.db"); }
        catch (String const & e) {}

        auto db = util::sqlite::Database("test2.db");
        auto conn = util::sqlite::Connection(db);

        auto td = util::sqlite::TableDetails("data_table");
        td.add_column("word", "TEXT");
        td.add_column("id", "INT", true);
        util::sqlite::create_table(conn, td);

        SECTION("test reading table details from sqlite database") {

            auto new_td = *conn.get_table_details("data_table");
            REQUIRE(new_td.size() == 2);
            REQUIRE(new_td[0].name == "word");
            REQUIRE(new_td[0].type == "TEXT");
            REQUIRE(new_td[1].is_primary == true);
        }

        SECTION("test inserting multiple rows") {
            auto data = std::vector<Strings>{ {"the_word", "0"}, {"the", "1"}, {"hello", "2"} };
            util::sqlite::insert_many(conn, "data_table", data);

            auto row = conn.get_first_row("SELECT * FROM data_table");
            REQUIRE(row->at(0).get_name() == "word");
            REQUIRE(row->at(0).get_str() == "the_word");
        }



    }

}
