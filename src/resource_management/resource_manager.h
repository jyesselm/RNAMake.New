//
// Created by Joseph Yesselman on 1/14/18.
//

#ifndef RNAMAKE_NEW_RESOURCE_MANAGER_H
#define RNAMAKE_NEW_RESOURCE_MANAGER_H

#include <all_atom/segment_factory.h>

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

public:
    all_atom::ResidueTypeSet const &
    get_residue_type_set() { return rts_; }

private:
    all_atom::ResidueTypeSet rts_;
    all_atom::SegmentFactory seg_f_;
};

}


#endif //RNAMAKE_NEW_RESOURCE_MANAGER_H
