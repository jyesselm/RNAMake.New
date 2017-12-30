//
// Created by Joseph Yesselman on 12/7/17.
//

#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <util/sqlite_wrappers.h>

TEST_CASE( "Test Basic Sqlite3 connections ", "[Sqlite3Connection]" ) {

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


    /*auto db = sqlitepp::db("test.db");
    sqlitepp::query q(db, "CREATE TABLE data_table (word TEXT, id INT, PRIMARY KEY(id));");

    auto rc = q.exec();
    sqlitepp::transaction tr(db);

    sqlitepp::query q2(db);
    q2.exec("INSERT INTO data_table( word, id) VALUES ('the_word','0');");
    q2.exec("INSERT INTO data_table( word, id) VALUES ('the','1');");
    q2.exec("INSERT INTO data_table( word, id) VALUES ('word','2');");

    SECTION("test counting rows in table") {

        sqlitepp::query q1(db, "SELECT count(*) FROM data_table");
        sqlitepp::result res = q1.store();

        int count = res[0]["count(*)"];
        REQUIRE(count == 3);

    }*/

    /*for(auto const & _row : res) {
        for (sqlitepp::row::size_type c = 0; c < _row.num_fields(); ++c) {
            if (!_row[c].is_null()) {
                std::cout << _row[c].name() << std::endl;
            }
        }
    }*/

    exit(0);
