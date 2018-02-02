//
// Created by Joseph Yesselman on 1/14/18.
//

#ifndef RNAMAKE_NEW_RESOURCE_MANAGER_H
#define RNAMAKE_NEW_RESOURCE_MANAGER_H

#include <all_atom/segment_factory.h>
#include <resource_management/segment_sqlite_library.h>

namespace resource_management {

class ResourceManager {
public:
    ResourceManager():
            rts_(all_atom::ResidueTypeSet()),
            seg_f_(all_atom::SegmentFactory(rts_)) {}

    ~ResourceManager() {}

public:
    // only want one resource manager active at a time
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager & operator = (const ResourceManager & ) { return *this; }

public:    // load new segments from pdbs and components
    inline
    all_atom::SegmentOP
    segment_from_pdb(
            String const & pdb_path,
            util::SegmentType segment_type = util::SegmentType::SEGMENT,
            bool rebuild_x3dna_files = true) {
        return seg_f_.segment_from_pdb(pdb_path, segment_type, rebuild_x3dna_files);
    }

    inline
    all_atom::SegmentOPs
    all_segments_from_pdb(
            String const & pdb_path,
            util::SegmentType segment_type = util::SegmentType::SEGMENT,
            bool rebuild_x3dna_files = true) {
        return seg_f_.all_segments_from_pdb(pdb_path, segment_type, rebuild_x3dna_files);
    }

    inline
    all_atom::SegmentOP
    segment_from_components(
            String const & name,
            all_atom::Structure const & rna_struc,
            all_atom::Basepairs const & basepairs,
            all_atom::Structure const & proteins,
            all_atom::Structure const & small_molecules,
            util::SegmentType segment_type = util::SegmentType::SEGMENT) {
        return seg_f_.segment_from_components(name, rna_struc, basepairs, proteins, small_molecules, segment_type);
    }

public: // get segments




public:
    all_atom::ResidueTypeSet const &
    get_residue_type_set() { return rts_; }

private:
    all_atom::ResidueTypeSet rts_;
    all_atom::SegmentFactory seg_f_;
    std::vector<SegmentSqliteLibrary> sqlite_libraries_;
};

}


#endif //RNAMAKE_NEW_RESOURCE_MANAGER_H
