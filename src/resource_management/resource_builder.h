//
// Created by Joseph Yesselman on 1/4/18.
//

#ifndef RNAMAKE_NEW_RESOURCE_BUILDER_H
#define RNAMAKE_NEW_RESOURCE_BUILDER_H

#include <base/paths.h>
#include <all_atom/segment_factory.h>

namespace resource_management {

class ResourceBuilder {
public:
    inline
    ResourceBuilder(
            all_atom::SegmentFactory const & seg_f):
            seg_f_(seg_f) {}


private:
    all_atom::SegmentFactory const & seg_f_;

};

}

#endif //RNAMAKE_NEW_RESOURCE_BUILDER_H
