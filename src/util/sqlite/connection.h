//
// Created by Joseph Yesselman on 12/30/17.
//

#ifndef RNAMAKE_NEW_SQLITE3_CONNECTION_H
#define RNAMAKE_NEW_SQLITE3_CONNECTION_H

#include <util/sqlite/field.h>
#include <util/sqlite/database.h>
#include <util/sqlite/table_details.h>

namespace util {
namespace sqlite {

class Connection {
public:

    Connection(
            Database const & db) :
            db_(db) {}

    ~Connection() {}

public: // interface to get rows
    int
    start_iterate_rows(
            String const & command) {
        int err = _prepare(command);
    }

    RowOP
    next() {
        if (sqlite3_step(stmt_) != SQLITE_ROW) { return RowOP(nullptr); }
        return _generate_row_from_statement();
    }


    RowOP
    get_first_row(
            String const & command) {
        int err = _prepare(command);
        if (sqlite3_step(stmt_) != SQLITE_ROW) { return RowOP(nullptr); }

        auto row = _generate_row_from_statement();
        sqlite3_finalize(stmt_);
        return row;
    }

    void
    abort_iterate_rows() { sqlite3_finalize(stmt_); }

public: // get other table infos
    TableDetailsOP
    get_table_details(
            String const & table_name) {

        auto q  = "SELECT sql FROM sqlite_master WHERE tbl_name = " + _quoted_string(table_name);
        _prepare(q);
        rc_ = sqlite3_step(stmt_);

        auto table_str = String(reinterpret_cast<const char *>(sqlite3_column_text(stmt_, 0)));
        auto spl = base::split_str_by_delimiter(table_str, "(");
        if(spl.size() != 3) {
            throw SqliteException("not a valid table declaration: " + table_str);
        }
        auto columns = base::split_str_by_delimiter(spl[1], ",");
        auto names = Strings();
        auto types = Strings();
        auto primary_keys = std::map<String, bool>();
        for(auto const & col : columns) {
            auto trimed_col = col;
            base::trim(trimed_col);
            auto col_spl = base::split_str_by_delimiter(trimed_col, " ");
            if(col_spl.size() < 2) {
                throw SqliteException("cannot parse table declaration section: " + col);
            }

            if(col_spl[0] == "PRIMARY") {
                for(auto i = 1; i < col_spl.size(); i++) {
                    primary_keys[col_spl[i]] = true;
                }
            }
            else {
                names.push_back(col_spl[0]);
                types.push_back(col_spl[1]);
            }
        }

        auto td = std::make_shared<TableDetails>(table_name);
        for(int i = 0; i < names.size(); i++) {
            auto is_primary = false;
            if(primary_keys.find(names[i]) == primary_keys.end()) { is_primary = true; }
            td->add_column(names[i], types[i], is_primary);
        }

        return td;
    }

    String const &
    get_database_name() {
        return db_.get_name();
    }


public:
    int
    exec(
            String const & command) {
        int err = _prepare(command);
        err = sqlite3_step(stmt_);
        if (err != SQLITE_DONE) {
            sqlite3_finalize(stmt_);
            return err;
        }
        return sqlite3_finalize(stmt_);

    }

    void
    start_transaction() { _execute("BEGIN TRANSACTION;"); }

    void
    commit_transaction() { _execute("COMMIT;"); }

    void
    rollback_transaction() { _execute("ROLLBACK;"); }

private:
    int
    _execute(
            String const & command) {
        rc_ = sqlite3_exec(db_(), command.c_str(), NULL, 0, &zErrMsg_);
        if (rc_ != SQLITE_OK) {
            throw SqliteException(String(zErrMsg_));
        }
        return rc_;
    }

    int
    _prepare(
            String const & command) {
        rc_ = sqlite3_prepare_v2(db_(),
                                 command.c_str(),
                                 (int) strlen(command.c_str()) + 1,
                                 &stmt_,
                                 NULL);
        if (rc_ != SQLITE_OK) {
            throw SqliteException("error returned with query: " + command);
        }
        return rc_;
    }

    RowOP
    _generate_row_from_statement() {
        auto fields = std::vector<Field>();
        auto col_type = -1;
        for (int i = 0; i < sqlite3_column_count(stmt_); ++i) {
            col_type = sqlite3_column_type(stmt_, i);
            if (col_type == SQLITE_INTEGER) {
                fields.push_back(Field(sqlite3_column_name(stmt_, i), sqlite3_column_int(stmt_, i)));
            } else if (col_type == SQLITE_FLOAT) {
                fields.push_back(Field(sqlite3_column_name(stmt_, i), sqlite3_column_double(stmt_, i)));
            } else if (col_type == SQLITE_BLOB) {
                std::uint8_t const *blob = reinterpret_cast<const std::uint8_t *>(::sqlite3_column_blob(stmt_, i));
                std::vector<std::uint8_t> v(&blob[0], &blob[::sqlite3_column_bytes(stmt_, i)]);
                fields.push_back(Field(sqlite3_column_name(stmt_, i), v));
            } else if (col_type == SQLITE3_TEXT) {
                fields.push_back(Field(sqlite3_column_name(stmt_, i),
                                       reinterpret_cast<const char *>(sqlite3_column_text(stmt_, i))));
            } else {
                throw SqliteException("not supported sqlite3_column type: " + std::to_string(col_type));
            }

        }
        return std::make_shared<Row>(fields);

    }


    String
    _quoted_string( String const & s ) {
        return String("'") + s + String("'");
    }


private:
    Database const & db_;
    sqlite3_stmt *stmt_;  // statement
    char *zErrMsg_;
    int rc_;


};

void
create_table(
        Connection &,
        TableDetails const &);

void
insert_many(
        Connection &,
        String const &,
        std::vector<Strings> const &);

}
}

#endif //RNAMAKE_NEW_SQLITE3_CONNECTION_H
