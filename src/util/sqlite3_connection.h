//
//  sqlite3_connection.h
//  RNAMake
//
//  Created by Joseph Yesselman on 2/2/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#ifndef __RNAMake__sqlite3_connection__
#define __RNAMake__sqlite3_connection__

#include <stdio.h>
#include <iostream>
#include <sqlite3.h>


//RNAMake Libraries
#include <base/types.h>
#include <base/logger.h>
#include <base/file_io.h>
#include <base/string.h>

namespace util {

class Sqlite3ConnectionException : public std::runtime_error {
public:
    Sqlite3ConnectionException(String const & message) :
            std::runtime_error(message) {}
};

class Sqlite3Connection {
public:
    enum class ColumnType {
        TEXT,
        INT
    };

    struct TableDetails {
        String name;
        int num_of_rows;
        std::map<String, ColumnType> column_and_types;

        ColumnType
        get_column_type(
                String const &) {
            return ColumnType::TEXT;
        }

    };

    typedef std::shared_ptr<TableDetails> TableDetailsOP;


public:
    Sqlite3Connection() :
            setup_(0),
            zErrMsg_(0),
            ic_(0),
            rc_(0) {

    }

    ~Sqlite3Connection() {
        if (setup_) {
            delete zErrMsg_;
            sqlite3_close(db_);
        }
    }

public:

    void
    connect(
            String const & path) {
        if(!base::file_exists(path)) {
            LOG_INFO("Sqlite3Connection", "creating new sqlite3 database file at path: " + path);
            std::ofstream out;
            out.open(path);
            out.close();
        }

        rc_ = sqlite3_open(path.c_str(), &db_);
        if (rc_) {
            throw Sqlite3ConnectionException("Can't open sqlite3 database " + path);
        }
        setup_ = 1;
    }

    void
    create_table(
            String const & table_name,
            Strings const & keys,
            Strings const & types,
            String const & primary_key,
            std::vector<Strings> const & data) {

        auto table_str = "CREATE TABLE " + table_name + "(";
        auto insert_str = "INSERT INTO " + table_name + "( ";
        for(int i = 0; i < keys.size(); i++) {
            table_str += keys[i] + " " + types[i] + ", ";
            insert_str += keys[i];
            if(i != keys.size()-1) { insert_str += ", "; }
        }
        table_str += "PRIMARY KEY(" + primary_key + "))";
        insert_str += ") VALUES ";

        _execute(table_str);
        _execute("BEGIN TRANSACTION;");

        int i = -1;
        auto insert_statement = String("");
        for(auto const & row : data) {
            i = -1;
            insert_statement = insert_str + "(";
            for(auto const & element : row) {
                i++;
                insert_statement += _quoted_string(element);
                if(i != row.size()-1) {
                    insert_statement += ",";
                }
            }
            insert_statement += ");";
            _execute(insert_statement);
        }

        _execute("END TRANSACTION;");

        _get_db_info();

    }

    void
    query(String const & query_statement) {
        _prepare(query_statement);
        rc_ = sqlite3_step(stmt_);
    }

    int
    get_number_of_rows(
            String const & table_name) {
        query("SELECT count(*) from " + table_name);
        int count = sqlite3_column_int(stmt_, 0);
        rc_ = sqlite3_step(stmt_);
        return count;
    }

    Strings
    fetch_one(String const &);


private:
    void
    _execute(
            String const & command) {
        rc_ = sqlite3_exec(db_, command.c_str(), NULL, 0, &zErrMsg_);
        if (rc_ != SQLITE_OK) {
            throw Sqlite3ConnectionException(String(zErrMsg_));
        }
    }

    void
    _prepare(
            String const & command) {
        rc_ = sqlite3_prepare_v2(db_,
                                 command.c_str(),
                                 (int) strlen(command.c_str()) + 1,
                                 &stmt_,
                                 NULL);
        if (rc_ != SQLITE_OK) {
            throw Sqlite3ConnectionException("error returned with query: " + command);
        }

    }

    String
    _quoted_string( String const & s ) {
        return String("'") + s + String("'");
    }

    void
    _get_db_info() {
        auto q = String("Select name From sqlite_master where type='table' order by name;");

        query(q);
        auto table_names = Strings();

        int col = sqlite3_column_count(stmt_);
        for (int i = 0; i < col; i++) {
            table_names.push_back(String(reinterpret_cast<const char *>(sqlite3_column_text(stmt_, i))));
        }

        for(auto const & table_name : table_names) {
            std::cout << table_name << std::endl;
            q  = "SELECT sql FROM sqlite_master WHERE tbl_name = " + _quoted_string(table_name);
            query(q);

            auto table_str = String(reinterpret_cast<const char *>(sqlite3_column_text(stmt_, 0)));
            auto spl = base::split_str_by_delimiter(table_str, "(");
            if(spl.size() != 3) {
                throw Sqlite3ConnectionException("not a valid table declaration: " + table_str);
            }

            auto names = Strings();
            auto types = Strings();

            auto columns = base::split_str_by_delimiter(spl[1], ",");
            for(auto const & s : columns) {
                std::cout << s << std::endl;
            }

        }



    }

private:
    int rc_;
    int ic_;
    int setup_;
    String query_statement_;
    String db_name_;
    sqlite3 *db_;
    sqlite3_stmt *stmt_;
    char *zErrMsg_;

    std::map<String, TableDetails> table_details_;


};

}

#endif /* defined(__RNAMake__sqlite3_connection__) */
