//
// Created by Joseph Yesselman on 2/13/19.
//

#ifndef RNAMAKE_NEW_STATE_ALIGNER_H
#define RNAMAKE_NEW_STATE_ALIGNER_H

#include <base/log.h>
#include <primitives/aligner.h>
#include <state/segment.h>

namespace state {

using Aligner = primitives::Aligner<Segment, Basepair>;

}


#endif //RNAMAKE_NEW_STATE_ALIGNER_H
