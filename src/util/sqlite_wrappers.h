//
// Created by Joseph Yesselman on 12/29/17.
//

#ifndef RNAMAKE_NEW_SQLITE_WRAPPERS_H
#define RNAMAKE_NEW_SQLITE_WRAPPERS_H

#include <sqlite3.h>
#include <iostream>

#include <base/assertions.h>
#include <base/types.h>
#include <base/string.h>
#include <base/vector_container.h>

namespace util {
namespace sqlite {

class SqliteException : public std::runtime_error {
public:
    SqliteException(String const & message) :
            std::runtime_error(message) {}
};

class Field {
public:
    inline
    Field(
            String const & name,
            int i):
            name_(name),
            int_(i),
            type_(SQLITE_INTEGER) {}
    inline
    Field(
            String const & name,
            sqlite3_int64 i64):
            name_(name),
            int_(i64),
            type_(SQLITE_INTEGER) {}
    inline
    Field(
            String const & name,
            double d):
            name_(name),
            float_(d),
            type_(SQLITE_FLOAT) {}

    inline
    Field(
            String const & name,
            String const & s):
            name_(name),
            str_(s),
            type_(SQLITE_TEXT) {}

    inline
    Field(
            String const & name,
            std::vector<std::uint8_t> const & v):
            name_(name),
            vec_(v),
            type_(SQLITE_BLOB) {}

public: // getters
    inline
    operator
    int() const {
        expects<SqliteException>(
                type_ == SQLITE_INTEGER,
                "cannot get int value, field is not int type value");
        return static_cast<int>(int_);
    }

    inline
    operator
    sqlite3_int64() const {
        expects<SqliteException>(
                type_ == SQLITE_INTEGER,
                "cannot get int value, field is not int type value");
        return int_;
    }

    inline
    operator
    double() const {
        expects<SqliteException>(
                type_ == SQLITE_FLOAT,
                "cannot get double value, field is not double type value");
        return float_;
    }

    inline
    operator
    String() const {
        return get_str();
    }

public:
    inline
    int
    get_type() const { return type_; }

    inline
    String
    get_name() const { return name_; }

    inline
    String
    get_str() const {
        if     (type_ == SQLITE3_TEXT)   { return str_; }
        else if(type_ == SQLITE_INTEGER) { return std::to_string(int_); }
        else if(type_ == SQLITE_FLOAT)   { return std::to_string(float_); }
        else if(type_ == SQLITE_BLOB)    { return "BLOB"; }
    }


private:
    std::int64_t int_;   // int data
    double float_; // float data
    std::vector<std::uint8_t> vec_;   // vector (blob) data
    String str_;   // string (text) data
    String name_;  // field (col) name
    int type_;  // sqlte type
};

typedef base::VectorContainer<Field> Row;
typedef std::shared_ptr<Row>         RowOP;

class Database {
public:
    Database(
            std::string name):
            name_(name),
            db_(nullptr) { open(); }

    ~Database() {
        // close the db
        (void) close();
    }

public:
    // close the database
    int close() {
        int err = sqlite3_close(db_);
        open_ = false;
        db_ = nullptr;
        return err;
    }

    // returns true if the database is open
    inline bool is_open() const { return open_; }

    // SQLite3 access
    inline sqlite3 *get() const { return db_; }

    inline sqlite3 *operator()() const { return db_; }

private:
    // open (connect) the database
    int open(int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE) {
        int err = sqlite3_open_v2(name_.c_str(), &db_, flags, nullptr);
        open_ = !err;
        return err;
    }

private:
    Database & operator=(const Database &) {}    // no assignment

private:
    sqlite3 *db_;    // associated db
    const String name_;  // db filename
    bool open_;  // db open status
};

class Connection  {
public:

    Connection(
            Database const & db):
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
            if     (col_type == SQLITE_INTEGER) {
                fields.push_back(Field(sqlite3_column_name(stmt_, i), sqlite3_column_int(stmt_, i)));
            }
            else if(col_type == SQLITE_FLOAT) {
                fields.push_back(Field(sqlite3_column_name(stmt_, i), sqlite3_column_double(stmt_, i)));
            }
            else if(col_type == SQLITE_BLOB) {
                std::uint8_t const * blob = reinterpret_cast<const std::uint8_t *>(::sqlite3_column_blob(stmt_, i));
                std::vector<std::uint8_t> v(&blob[0], &blob[::sqlite3_column_bytes(stmt_, i)]);
                fields.push_back(Field(sqlite3_column_name(stmt_, i), v));
            }
            else if(col_type == SQLITE3_TEXT) {
                fields.push_back(Field(sqlite3_column_name(stmt_, i),
                                       reinterpret_cast<const char *>(sqlite3_column_text(stmt_, i))));
            }
            else {
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


}
}


#endif //RNAMAKE_NEW_SQLITE_WRAPPERS_H
