//
// Created by Joseph Yesselman on 12/26/17.
//

#ifndef RNAMAKE_NEW_ALL_ATOM_ALIGNER_H
#define RNAMAKE_NEW_ALL_ATOM_ALIGNER_H

#include <base/log.h>
#include <primitives/aligner.h>
#include <all_atom/segment.h>

namespace all_atom {

using Aligner = primitives::Aligner<Segment, Basepair>;

}


#endif //RNAMAKE_NEW_ALIGNER_H
