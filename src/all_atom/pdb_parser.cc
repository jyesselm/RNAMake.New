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

void
PDBParser::_setup_ref_residue(
        String const & pdb_file) {
    _parse_atoms_from_pdb_file(pdb_file);
    

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



























