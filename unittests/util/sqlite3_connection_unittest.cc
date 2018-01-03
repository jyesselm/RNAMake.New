//
// Created by Joseph Yesselman on 12/7/17.
//

#include <iostream>
#include "../common.hpp"

#include <base/paths.h>
#include <util/sqlite/connection.h>

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>

TEST_CASE( "Test Basic Sqlite3 connections ", "[Sqlite3Connection]" ) {

    SECTION("test basic features of connection") {

        auto db = util::sqlite::Database(":memory:");
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

        SECTION("test get table details") {
            auto tb = q.get_table_details("data_table");

        }
    }

    /*SECTION("test create table") {

        auto db = util::sqlite::Database(":memory:");
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

    SECTION("test bindings") {
        auto db = util::sqlite::Database(":memory:");
        auto conn = util::sqlite::Connection(db);

        auto td = util::sqlite::TableDetails("data_table");
        td.add_column("word", "TEXT");
        td.add_column("id", "INT", true);
        util::sqlite::create_table(conn, td);

        auto str = String("hello");
        conn.bind(1, str);
        auto err = conn.exec("INSERT INTO data_table( id, word ) VALUES ('0', ?);");

        auto row = conn.get_first_row("SELECT * FROM data_table");
        REQUIRE(row->at(0).get_str() == "hello");
    }*/

    SECTION("test bindings with compressed str") {
        /*auto db = util::sqlite::Database(":memory:");
        auto conn = util::sqlite::Connection(db);

        auto td = util::sqlite::TableDetails("data_table");
        td.add_column("word", "BLOB");
        td.add_column("id", "INT", true);
        util::sqlite::create_table(conn, td);

        int level = Z_BEST_COMPRESSION; // Z_DEFAULT_COMPRESSION is the default if no arg is passed
        int strategy = Z_DEFAULT_STRATEGY; // Z_DEFAULT_STRATEGY is the defaul if no arg is passed

        auto str = String("hello hello hello hello hello");
        int size = str.length();
        auto compressed_data = gzip::compress(str.c_str(), size, level, strategy);
        std::vector<uint8_t> myVector(compressed_data.begin(), compressed_data.end());


        conn.bind(1, myVector);
        auto err = conn.exec("INSERT INTO data_table( id, word ) VALUES ('0', ?);");

        auto row = conn.get_first_row("SELECT * FROM data_table");
        auto sql_str = row->at(0).get_str();
        std::vector<uint8_t> blob = row->at(0);
        auto sql_data = String(blob.begin(), blob.end());

        auto decompressed_data = gzip::decompress(sql_data.data(), sql_data.size());
        REQUIRE(sql_data == compressed_data);
        */
    }

}

























