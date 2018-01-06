//
// Created by Joseph Yesselman on 1/5/18.
//

#ifndef RNAMAKE_NEW_MOTIF_SQLITE_LIBRARY_H
#define RNAMAKE_NEW_MOTIF_SQLITE_LIBRARY_H

#include <all_atom/segment.h>
#include <resource_management/sqlite_library.h>

namespace resource_management {

class SegmentSqliteLibrary : public SqliteLibrary {
public:
    SegmentSqliteLibrary(
            String const & db_path,
            String const & table_name,
            all_atom::ResidueTypeSet const & rts):
            SqliteLibrary(db_path, table_name),
            rts_(rts),
            retrieved_columns_(Strings{"id", "data"}),
            segments_(std::map<int, all_atom::SegmentOP>()) {}

    ~SegmentSqliteLibrary() {}

public:
    all_atom::SegmentOP
    get_segment(
            StringStringMap const &);

protected:
    Strings retrieved_columns_;
    all_atom::ResidueTypeSet const & rts_;
    std::map<int, all_atom::SegmentOP> segments_;

};

}

#endif //RNAMAKE_NEW_MOTIF_SQLITE_LIBRARY_H
