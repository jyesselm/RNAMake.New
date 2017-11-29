//
//  pdb_parser.h
//  RNAMake
//
//  Created by Joseph Yesselman on 5/12/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#ifndef __RNAMake__all_atom_pdb_parser__
#define __RNAMake__all_atom_pdb_parser__

#include <stdio.h>
#include <map>

//RNAMake Headers
#include <base/paths.h>
#include <base/option.h>
#include <math/xyz_matrix.h>
#include <all_atom/residue.h>
#include <all_atom/residue_type_set.h>

namespace all_atom {

struct PDBParserResidues {
    ResidueOPs RNA_residues, protein_residues, small_molecule_residues;
};

typedef std::shared_ptr<PDBParserResidues> PDBParserResiduesOP;

class PDBParser {
public:
    PDBParser(
            ResidueTypeSetOP rts) :
            rts_(rts) {

        auto opt_infos = base::OptInfos{
                base::OPT_INFO_BOOL("parse_rna", "true"),
                base::OPT_INFO_BOOL("parse_proteins", "false"),
                base::OPT_INFO_BOOL("parse_small_molecules", "false")};
        options_ = base::Options(opt_infos);

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

    PDBParserResiduesOP
    parse(
            String const &);

public: // option wrappers
    inline
    void
    set_option_values(
            StringStringMap const & key_values) { options_.set_values(key_values); }

    inline
    void
    set_bool_option_value(
            String const & name,
            bool value) { options_.set_bool_value(name, value); }

    inline
    bool
    get_bool_option_value(
            String const & name) { return options_.get_bool_value(name); }

private:
    void
    _parse_atoms_from_pdb_file(
            String const &);

    ResidueOP
    _setup_ref_residue(
            String const &);

    ResidueOP
    _setup_residue(
            String const &,
            Atoms const &,
            ResidueTypeCOP);

    math::Matrix
    _get_res_ref_frame(
            ResidueCOP);

    math::Matrix
    _get_res_ref_frame_from_atoms(
            std::vector<Atom const *> const &,
            ResidueTypeCOP);

    void
    _replace_missing_phosphate_backbone(
            std::vector<Atom const *> &,
            ResidueTypeCOP);

private:
    ResidueTypeSetOP rts_;
    std::map<String, ResidueOP> ref_residues_;
    // parse variables
    std::map<String, Atoms> atoms_;
    String startswith_;
    String atom_name_, res_name_, res_num_, chain_id_, i_code_;
    String sx_, sy_, sz_;
    math::Point atom_coords_;
    std::map<String, String> atom_name_corrections_;
    base::Options options_;





};

}
#endif /* defined(__RNAMake__all_atom_pdb_parser__) */
