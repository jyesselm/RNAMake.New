//
//  atom.cpp
//  RNAMake
//
//  Created by Joseph Yesselman on 12/23/14.
//  Copyright (c) 2014 Joseph Yesselman. All rights reserved.
//

//RNAMake Headers
#include "math/xyz_vector.h"
//#include "math/numerical.h"
#include "all_atom/atom.h"

namespace all_atom {

String
Atom::get_str() const {
    return name_->get_str() + " " + coords_.get_str();
}

json::JSON
Atom::get_json() const {
    return json::JSON {
            "name", name_->get_str(),
            "coords", coords_.get_json() };
}

String
Atom::get_pdb_str(
        int acount) const {

    char buffer[200];
    std::sprintf(buffer, "ATOM %6d  P   C   A   1 %11.3f%8.3f%8.3f  1.00 62.18           P\n",
                 acount, coords_.get_x(), coords_.get_y(), coords_.get_z());
    return String(buffer);

}

}