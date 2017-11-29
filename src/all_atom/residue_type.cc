//
//  residue_type.cc
//  RNAMake
//
//  Created by Joseph Yesselman on 1/24/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

//RNAMake Headers
#include <all_atom/residue_type.h>

namespace all_atom {

ResidueType::ResidueType(
        String const & name,
        StringIntMap const & atom_name_map,
        SetType set_type,
        Strings const & extra_alt_names) :
        name_(name),
        atom_name_map_(atom_name_map),
        set_type_(set_type) {

    alt_names_ = Strings();
    if (set_type_ == SetType::RNA) {
        alt_names_.push_back(name_.substr(0, 1));
        alt_names_.push_back("r" + name_.substr(0, 1));
        alt_names_.push_back("D" + name_.substr(0, 1));
    }
}

bool
ResidueType::is_valid_atom_name(
        String const & atom_name)  const {
    if(atom_name_map_.find(atom_name) != atom_name_map_.end()) { return true; }
    else                                                       { return false; }
}

Index
ResidueType::get_atom_index(
        String const & name) const {
    expects<ResidueTypeException>(
            is_valid_atom_name(name),
            "must supply valid atom name for residue type");
    return atom_name_map_.find(name)->second;
}

String
ResidueType::get_atom_name_at_pos(
        Index index) const {
    for(auto const & kv : atom_name_map_) {
        if(kv.second == index) { return kv.first; }
     }
    throw ResidueTypeException("residue type does not have an index: " + std::to_string(index));
}

bool
ResidueType::is_valid_residue_name(
        String const & res_name) const {
    if(res_name == name_) { return true; }
    for(auto const & alt_name : alt_names_) {
        if(res_name == alt_name) { return true; }
    }
    return false;
}

ResidueTypeCOP
get_new_residue_type(
        String const & res_name,
        Strings const & atom_names) {
    auto atom_name_map = StringIntMap();
    auto i = 0;
    for(auto const & name : atom_names) {
        atom_name_map[name] = i;
        i++;
    }
    auto extra_alt_names = Strings();

    return std::make_shared<ResidueType>(res_name, atom_name_map, SetType::UNKNOWN, extra_alt_names);
}

}


/*void
ResidueType::extend_res_specific_altnames() {
    // There has to be a better way to do this
    Strings alt_names;
    if (name_.compare("GUA") == 0) {
        alt_names = split_str_by_delimiter("MIA GDP GTP M2G 1MG 7MG G7M QUO I YG", " ");
    } else if (name_.compare("ADE") == 0) {
        alt_names = split_str_by_delimiter("A23 3DA 1MA 12A AET 2MA", " ");
    } else if (name_.compare("URA") == 0) {
        alt_names = split_str_by_delimiter("PSU H2U 5MU 4SU 5BU 5MC U3H 2MU 70U BRU DT", " ");
    } else if (name_.compare("CYT") == 0) {
        alt_names = split_str_by_delimiter("CBR CCC", " ");
    } else {

    }

    for (auto const & name : alt_names) {
        alt_names_.push_back(name);
    }
}

}*/





