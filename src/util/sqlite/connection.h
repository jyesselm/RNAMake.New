//
// Created by Joseph Yesselman on 12/30/17.
//

#ifndef RNAMAKE_NEW_SQLITE3_CONNECTION_H
#define RNAMAKE_NEW_SQLITE3_CONNECTION_H

#include <util/sqlite/field.h>
#include <util/sqlite/database.h>

namespace util {
namespace sqlite {

struct ColumnDetails {
    String name;
    String type;
    bool is_primary;
};

struct TableDetails {
    String name;
    std::vector<ColumnDetails> columns_;

    //iterator
    typedef std::vector<ColumnDetails>::iterator iterator;

    iterator begin() { return columns_.begin(); }
    iterator end()   { return columns_.end(); }

};


class Connection {
public:

    Connection(
            Database const & db) :
            db_(db) {}

    ~Connection() {}

public:
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


private:
    Database const & db_;
    sqlite3_stmt *stmt_;  // statement
    char *zErrMsg_;
    int rc_;


};

void
create_table(
        Connection const &,
        TableDetails const &);

}
}

#endif //RNAMAKE_NEW_SQLITE3_CONNECTION_H
