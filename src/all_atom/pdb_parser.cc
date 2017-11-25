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
        if (startswith_ == ("ATOM  ") || startswith_ == ("HETATM") == 0) {
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


    /*auto atom_ptrs = std::vector<Atom *>(res_type->get_num_atoms());
    for(int i = 0; i < atoms.size(); i++) {
        // not a valid atom for this residue
        if(! res_type->is_valid_atom_name(atoms[i].get_str_name())) {
            LOG_WARNING("PDB_Parser", atoms[i].get_str_name() + " does not belong to residue " +
                                      res_type->get_name() + ": IGNORING!");
            continue;
        }
        auto index = res_type->get_atom_index(atoms[i].get_str_name());
        atom_ptrs[i] = &atoms[i];
    }

    for(auto const & a : atom_ptrs) {
        if(a == nullptr) { std::cout << "NULL" << std::endl;}
        else {
            std::cout << a->get_str_name() << std::endl;
        }
    }*/

}

ResidueOP
PDBParser::_set_residue(
        String const &) {

    return ResidueOP(nullptr);
}



ResidueOPs const &
PDBParser::parse(
        String const & pdb_file,
        int protein,
        int rna) {

    residues_ = ResidueOPs();

    auto lines = base::get_lines_from_file(pdb_file);
    String startswith;
    String atomname, resname, resnum, chid, alt;
    math::Point coords;
    String sx, sy, sz;
    double x, y, z;

    Strings atomnames, resnames, chainids, icodes, resnums;
    math::Points coordinates;

    for (auto const & line : lines) {
        startswith = line.substr(0, 6);
        if (startswith.compare("ATOM  ") == 0 ||
            startswith.compare("HETATM") == 0) {
            atomname = line.substr(12, 4);
            atomname = base::trim(atomname);
            resname = line.substr(17, 4);
            resname = base::trim(resname);
            chid = line.substr(21, 1);
            alt = line.substr(16, 1);

            sx = line.substr(30, 8);
            sy = line.substr(38, 8);
            sz = line.substr(46, 8);
            sx = base::trim(sx);
            sy = base::trim(sy);
            sz = base::trim(sz);

            x = std::stod(sx);
            y = std::stod(sy);
            z = std::stod(sz);
            coords = math::Point(x, y, z);

            resnum = line.substr(22, 4);
            resnum = base::trim(resnum);

            atomnames.push_back(atomname);
            resnames.push_back(resname);
            chainids.push_back(chid);
            resnums.push_back(resnum);
            icodes.push_back(line.substr(26, 1));
            coordinates.push_back(coords);


        } else if (startswith.compare("ENDMDL") == 0 || startswith.substr(0, 3).compare("END") == 0) {
            break;
        }
    }

    /*String key;
    std::map<String, AtomOPs> residue_atoms;
    int already_has = 0;
    
    for(int i = 0; i < atomnames.size(); i++) {
        if(resnames[i].compare("HOH") == 0) { continue; }
        key = resnames[i] + " " + resnums[i] + " " + chainids[i] + " " + icodes[i];
        if(residue_atoms.find(key) == residue_atoms.end()) {
            residue_atoms[key] = AtomOPs();
        }
        already_has = 0;
        for(auto const & a : residue_atoms[key]) {
            if(a->name().compare(atomnames[i]) == 0) {
                //already_has = 1;
                //break;
            }
        }
        if(already_has) { continue;}
        residue_atoms[key].push_back(std::make_shared<Atom>(atomnames[i], coordinates[i]));
    }
    
    
    ResidueTypeOP rtype;
    Strings spl;
    String icode = " ";
    ResidueOP r;
    for(auto & kv : residue_atoms) {
        if(kv.second.size() < 6) { continue; }
        spl = split_str_by_delimiter(kv.first, " ");
        if(!rts_.contains_rtype(spl[0])) { continue; }
        rtype = rts_.get_type(spl[0]);
        
        if(!protein && rtype->set_type() == SetType::PROTEIN) { continue; }
        if(!rna && rtype->set_type() == SetType::RNA) { continue; }
 
        icode = " ";
        if(spl.size() > 3) { icode = spl[3]; }
        if(icode.size() == 0) { icode = " "; }
        r = std::make_shared<Residue>(kv.second, rtype, spl[0][0], std::stoi(spl[1]),
                                      spl[2][0], icode[0]);
        residues_.push_back(r);
        
    }*/

    return residues_;
}

}



























