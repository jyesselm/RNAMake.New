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

        auto path = base::resources_path() + "/ideal_residues/";
        auto names = Strings{"GUA"};
        _setup_ref_residue(path+names[0]+".pdb");

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

    void
    _setup_ref_residue(
            String const &);

private:
    ResidueOPs residues_;
    ResidueTypeSetOP rts_;
    // parse variables
    std::map<String, Atoms> atoms_;
    String startswith_;
    String atom_name_, res_name_, res_num_, chain_id_, i_code_;
    String sx_, sy_, sz_;
    math::Point atom_coords_;





};

}
#endif /* defined(__RNAMake__pdb_parser__) */
