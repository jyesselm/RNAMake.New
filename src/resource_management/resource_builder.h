//
// Created by Joseph Yesselman on 1/4/18.
//

#ifndef RNAMAKE_NEW_RESOURCE_BUILDER_H
#define RNAMAKE_NEW_RESOURCE_BUILDER_H

#include <base/paths.h>
#include <util/sqlite/connection.h>
#include <all_atom/segment_factory.h>

namespace resource_management {


class ResourceBuilderException : public std::runtime_error {
public:
    /**
     * Standard constructor for ResourceBuilderException
     * @param   message   Error message for SqliteLibrary
     */
    ResourceBuilderException(String const & message) :
            std::runtime_error(message) {}
};

class ResourceBuilder {
public:
    inline
    ResourceBuilder(
            all_atom::SegmentFactory & seg_f,
            String const & motif_dirs_path):
            seg_f_(seg_f),
            motif_dirs_path_(motif_dirs_path),
            motif_table_(_generate_motif_table_details()) {

    }

public:
    void
    build_ideal_helices();

private:
    util::sqlite::TableDetails
    _generate_motif_table_details() {
        auto motif_table = util::sqlite::TableDetails("data_table");
        motif_table.add_column("data", "BLOB");
        motif_table.add_column("name", "TEXT");
        motif_table.add_column("end_name", "TEXT");
        motif_table.add_column("end_id", "TEXT");
        motif_table.add_column("id", "INT", true);
        return motif_table;
    }

private:
    all_atom::SegmentFactory & seg_f_;
    String motif_dirs_path_;
    util::sqlite::TableDetails motif_table_;
    std::vector<uint8_t> blob_;
    String compressed_str_;

};

}

#endif //RNAMAKE_NEW_RESOURCE_BUILDER_H
