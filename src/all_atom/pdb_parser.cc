//
//  pdb_parser.cc
//  RNAMake
//
//  Created by Joseph Yesselman on 5/12/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#include <map>

//RNAMake Headers
#include <base/string.h>
#include <base/file_io.h>
#include <base/logger.h>
#include <math/xyz_vector.h>
#include <all_atom/residue.h>
#include <all_atom/pdb_parser.h>

namespace all_atom {

void
PDBParser::_parse_atoms_from_pdb_file(
        String const & pdb_file) {

    atoms_ = std::map<String, Atoms>();
    auto lines = base::get_lines_from_file(pdb_file);
    auto key = String("");
    for (auto const & line : lines) {
        if(line.size() < 6) { continue; }
        startswith_ = line.substr(0, 6);
        if (startswith_ == ("ATOM  ") || startswith_ == ("HETATM")) {
            atom_name_ = line.substr(12, 4);
            atom_name_ = base::trim(atom_name_);

            // do not save hydrogen atoms
            if(atom_name_[0] == 'H') { continue; }

            if(atom_name_corrections_.find(atom_name_) != atom_name_corrections_.end()) {
                atom_name_ = atom_name_corrections_[atom_name_];
            }

            res_name_ = line.substr(17, 4);
            res_name_ = base::trim(res_name_);

            // do not save water
            if(res_name_ == "HOH") { continue; }

            chain_id_ = line.substr(21, 1);
            i_code_ = line.substr(16, 1);

            sx_ = line.substr(30, 8);
            sy_ = line.substr(38, 8);
            sz_ = line.substr(46, 8);
            sx_ = base::trim(sx_);
            sy_ = base::trim(sy_);
            sz_ = base::trim(sz_);

            atom_coords_ = math::Point(std::stod(sx_),  std::stod(sy_),  std::stod(sz_));

            res_num_ = line.substr(22, 4);
            res_num_ = base::trim(res_num_);

            key = res_name_ + "|" + res_num_ + "|" + chain_id_ + "|" + i_code_;
            if(atoms_.find(key) == atoms_.end()) { atoms_[key] = Atoms(); }
            atoms_[key].push_back(Atom(std::make_shared<base::SimpleString>(atom_name_),
                                       atom_coords_));

        }

        else if (startswith_ == "ENDMDL" || startswith_.substr(0, 3) == "END") { break; }
    }


}

ResidueOP
PDBParser::_setup_ref_residue(
        String const & pdb_file) {
    _parse_atoms_from_pdb_file(pdb_file);
    auto key = atoms_.begin()->first;
    auto & atoms = atoms_.begin()->second;
    auto spl = base::split_str_by_delimiter(key, "|");
    auto res_name = spl[0][0];
    auto res_type = rts_->get_residue_type(spl[0]);
    auto res_num = std::stoi(spl[1]);
    auto chain_id = spl[2][0];
    auto i_code = spl[3][0];

    return std::make_shared<Residue>(res_name, res_num, chain_id, i_code, res_type, atoms, util::Uuid());
}


math::Matrix
PDBParser::_get_res_ref_frame(
        ResidueCOP r) {
    auto vec1 = math::Point();
    auto vec2 = math::Point();
    if(r->get_name() == 'A' || r->get_name() == 'G') {
        vec1 = (r->get_coords("N9") - r->get_coords("C1'")).normalize();
        vec2 = (r->get_coords("N9") - r->get_bead(util::BeadType::BASE).get_center()).normalize();
    }
    else {
        vec1 = (r->get_coords("N1") - r->get_coords("C1'")).normalize();
        vec2 = (r->get_coords("N1") - r->get_bead(util::BeadType::BASE).get_center()).normalize();
    }
    auto cross = vec1.cross(vec2);
    auto m = math::Matrix(
            vec1.get_x(), vec1.get_y(), vec1.get_z(),
            vec2.get_x(), vec2.get_y(), vec2.get_z(),
            cross.get_x(), cross.get_y(), cross.get_z());
    m.unitarize();
    return m;
}

math::Matrix
PDBParser::_get_res_ref_frame_from_atoms(
        std::vector<Atom const *> const & atoms,
        ResidueTypeCOP res_type) {

    auto vec1 = math::Point();
    auto vec2 = math::Point();
    auto base_center = math::Point();
    int count = 0;
    for (int i = 12; i < res_type->get_num_atoms(); i++) {
        if(atoms[i] == nullptr) { continue; }
        base_center += atoms[i]->get_coords();
        count += 1;
    }
    base_center /= float(count);
    auto c1p_atom = atoms[ res_type->get_atom_index("C1'")];

    if (res_type->get_short_name() == 'A' || res_type->get_short_name() == 'G') {
        auto n9_atom  = atoms[ res_type->get_atom_index("N9") ];
        vec1 = (n9_atom->get_coords() - c1p_atom->get_coords()).normalize();
        vec2 = (n9_atom->get_coords() - base_center).normalize();
    }
    else {
        auto n1_atom = atoms[ res_type->get_atom_index("N1") ];
        vec1 = (n1_atom->get_coords() - c1p_atom->get_coords()).normalize();
        vec2 = (n1_atom->get_coords() - base_center).normalize();
    }

    auto cross = vec1.cross(vec2);
    auto m = math::Matrix(
            vec1.get_x(), vec1.get_y(), vec1.get_z(),
            vec2.get_x(), vec2.get_y(), vec2.get_z(),
            cross.get_x(), cross.get_y(), cross.get_z());
    m.unitarize();
    return m;
}


void
PDBParser::_replace_missing_phosphate_backbone(
        std::vector<Atom const *> & atoms,
        ResidueTypeCOP res_type) {

    auto ref_res = ref_residues_[res_type->get_name()];
    auto ref_frame_1 = _get_res_ref_frame_from_atoms(atoms, res_type);
    auto ref_frame_2 = _get_res_ref_frame(ref_res);
    //std::cout << ref_frame_1.get_str_readable() << std::endl;
    auto rot = dot(ref_frame_1.transpose(), ref_frame_2);
    auto r_t = rot.transpose();
    auto t = -ref_res->get_center();
    auto c4p_atom =  atoms[ res_type->get_atom_index("C4'") ];
    ref_res->transform(r_t, t);
    ref_res->move(c4p_atom->get_coords() - ref_res->get_coords("C4'"));

    for(int i = 0; i < 5; i++) {
        atoms[i] = new Atom(ref_res->get_atom(i).get_name(),
                            ref_res->get_atom(i).get_coords());
    }
}

ResidueOP
PDBParser::_setup_residue(
        String const & key,
        Atoms const & atoms,
        ResidueTypeCOP res_type) {

    auto spl = base::split_str_by_delimiter(key, "|");
    auto atom_ptrs = std::vector<Atom const *>(res_type->get_num_atoms());

    for(auto const & a : atoms) {
        // not a valid atom for this residue
        if(! res_type->is_valid_atom_name(a.get_str_name())) {
            LOG_WARNING("PDB_Parser", a.get_str_name() + " does not belong to residue " +
                                      res_type->get_name() + ": IGNORING!");
            continue;
        }
        auto index = res_type->get_atom_index(a.get_str_name());
        atom_ptrs[index] = &a;
    }

    if(res_type->get_set_type() == SetType::RNA) {
        auto i = 1;
        auto missing_phosphate = false;
        for (auto const & a : atom_ptrs) {
            i++;
            if (i < 5 && a == nullptr) { missing_phosphate = 1; }
        }

        if (missing_phosphate) { _replace_missing_phosphate_backbone(atom_ptrs, res_type); }
    }

    auto reordered_atoms = Atoms();
    auto i = 0;
    for(auto a_ptr : atom_ptrs) {
        if(a_ptr == nullptr) {
            throw std::runtime_error(
                    "cannot setup residue: " + spl[1] + " " + spl[0] + " missing atom: " +
                    res_type->get_atom_name_at_pos(i));
        }
        reordered_atoms.push_back(std::move(*a_ptr));
        i++;
    }

    auto res_name = spl[0][0];
    auto res_num = std::stoi(spl[1]);
    auto chain_id = spl[2][0];
    auto i_code = spl[3][0];

    return std::make_shared<Residue>(res_name, res_num, chain_id, i_code, res_type,
                                     reordered_atoms, util::Uuid());
}

PDBParserResiduesOP
PDBParser::parse(
        String const & pdb_file) {

    auto parse_rna = get_bool_option_value("parse_rna");
    auto parse_proteins = get_bool_option_value("parse_proteins");
    auto parse_small_molecules = get_bool_option_value("parse_small_molecules");

    auto residues = std::make_shared<PDBParserResidues>();
    _parse_atoms_from_pdb_file(pdb_file);
    for(auto const & kv : atoms_) {
        auto spl = base::split_str_by_delimiter(kv.first, "|");
        auto has_res_type = rts_->contains_residue_type(spl[0]);
        if(has_res_type) {
            auto res_type = rts_->get_residue_type(spl[0]);
            if     (res_type->get_set_type() == SetType::RNA && parse_rna) {
                residues->RNA_residues.push_back(_setup_residue(kv.first, kv.second, res_type));
            }
            else if(res_type->get_set_type() == SetType::PROTEIN && parse_proteins) {
                residues->protein_residues.push_back(_setup_residue(kv.first, kv.second, res_type));
            }
        }
        else {
            if(!parse_small_molecules) { continue; }
            auto atom_names = Strings();
            for(auto const & a : kv.second) { atom_names.push_back(a.get_str_name()); }
            auto res_type = get_new_residue_type(spl[0], atom_names);
            residues->small_molecule_residues.push_back(_setup_residue(kv.first, kv.second, res_type));
        }
    }
    return residues;
}
}



























