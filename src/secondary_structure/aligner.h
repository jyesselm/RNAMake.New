//
// Created by Joseph Yesselman on 11/4/17.
//

#ifndef RNAMAKE_NEW_SS_ALIGNER_H
#define RNAMAKE_NEW_SS_ALIGNER_H

#include <primitives/aligner.h>
#include <secondary_structure/segment.h>

namespace secondary_structure {

using Aligner = primitives::NoAligner<Segment, Basepair>;

}

#endif //RNAMAKE_NEW_ALIGNER_H
