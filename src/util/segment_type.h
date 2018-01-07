//
// Created by Joseph Yesselman on 11/3/17.
//

#ifndef RNAMAKE_NEW_RNA_SEGMENT_TYPE_H
#define RNAMAKE_NEW_RNA_SEGMENT_TYPE_H

namespace util {

enum class SegmentType {
    SEGMENT                = 0,
    MOTIF                  = 1,
    HELIX                  = 2,
    TWOWAY_JUNCTION        = 3,
    NWAY_JUNCTION          = 4,
    HAIRPIN                = 5,
    TERTIARY_CONTACT       = 6
};

}


#endif //RNAMAKE_NEW_RNA_SEGMENT_TYPE_H
