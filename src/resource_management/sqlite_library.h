//
// Created by Joseph Yesselman on 1/3/18.
//

#ifndef RNAMAKE_NEW_SQLITE_LIBRARY_H
#define RNAMAKE_NEW_SQLITE_LIBRARY_H

#include <base/assertions.h>
#include <util/sqlite/connection.h>

namespace resource_management {


class SqliteLibraryException : public std::runtime_error {
public:
    /**
     * Standard constructor for SqliteLibraryException
     * @param   message   Error message for SqliteLibrary
     */
    SqliteLibraryException(String const & message) :
            std::runtime_error(message) {}
};


class SqliteLibrary {
public:
    SqliteLibrary(
            String const &,
            String const &);

private:
    void
    _generate_query(
            Strings const &,
            StringStringMap const &);

    bool
    _is_valid_key(
            String const &);

protected:
    util::sqlite::Database db_;
    util::sqlite::Connection conn_;
    util::sqlite::TableDetails table_details_;
    String query_string_;
};

}


#endif //RNAMAKE_NEW_SQLITE_LIBRARY_H
