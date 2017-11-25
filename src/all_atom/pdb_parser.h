//
//  pdb_parser.h
//  RNAMake
//
//  Created by Joseph Yesselman on 5/12/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#ifndef __RNAMake__pdb_parser__
#define __RNAMake__pdb_parser__

#include <stdio.h>
#include <map>

//RNAMake Headers
#include <base/paths.h>
#include <all_atom/residue.h>
#include <all_atom/residue_type_set.h>

namespace all_atom {

class PDBParser {
public:
    PDBParser(
            ResidueTypeSetOP rts) :
            residues_(ResidueOPs()),
            rts_(rts) {

        atom_name_corrections_ = std::map<String, String>();
        atom_name_corrections_["O1P"] = "OP1";
        atom_name_corrections_["O2P"] = "OP2";

        auto path = base::resources_path() + "/ideal_residues/";
        auto names = Strings{"ADE", "CYT", "GUA", "URA"};
        for(auto const & name : names) {
            ref_residues_[name] = _setup_ref_residue(path + name + ".pdb");
        }
    }

    ~PDBParser() {}

public:

    ResidueOPs const &
    parse(String const & pdb_file,
            int protein = 0,
            int rna = 1);

private:
    void
    _parse_atoms_from_pdb_file(
            String const &);

    ResidueOP
    _setup_ref_residue(
            String const &);

    ResidueOP
    _set_residue(
            String const &);

private:
    ResidueOPs residues_;
    ResidueTypeSetOP rts_;
    std::map<String, ResidueOP> ref_residues_;
    // parse variables
    std::map<String, Atoms> atoms_;
    String startswith_;
    String atom_name_, res_name_, res_num_, chain_id_, i_code_;
    String sx_, sy_, sz_;
    math::Point atom_coords_;
    std::map<String, String> atom_name_corrections_;





};

}
#endif /* defined(__RNAMake__pdb_parser__) */
