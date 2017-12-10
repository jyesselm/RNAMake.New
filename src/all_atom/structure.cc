//
// Created by Joseph Yesselman on 11/28/17.
//

#include "all_atom/structure.h"
#include <all_atom/pdb_parser.h>

namespace all_atom {

String
Structure::get_pdb_str(
        int acount,
        int rnum,
        char chain_id) {

    auto s = String();
    auto i = -1;
    auto pos = 0;
    for(auto const & r : residues_) {
        i++;
        if (cut_points_[pos] == i) {

        }
    }
    return s;

}


// external functions
int
are_residues_connected_RNA(
        Residue const & r1,
        Residue const & r2) {

    auto o3 = String("O3'");
    auto p  = String("P");
    // 5' to 3'
    if (r1.get_coords(o3).distance(r2.get_coords(p)) < 3.0) { return 1; }
    // 3' to 5'
    if (r2.get_coords(o3).distance(r1.get_coords(p)) < 3.0) { return -1; }
    return 0;
}

int
are_residues_connected_protein(
        Residue const & r1,
        Residue const & r2) {

    auto c = String("C");
    auto n = String("N");
    // nter to cter
    if (r1.get_coords(c).distance(r2.get_coords(n)) < 1.4) { return 1; }
    // cter to nter
    if (r2.get_coords(c).distance(r1.get_coords(n)) < 1.4) { return -1; }
    return 0;
}


int
are_residues_connected(
        Residue const & r1,
        Residue const & r2)  {

    if     (r1.get_res_set_type() == SetType::RNA)     { return are_residues_connected_RNA(r1, r2); }
    else if(r1.get_res_set_type() == SetType::PROTEIN) { return are_residues_connected_protein(r1, r2); }
    else if(r1.get_res_set_type() == SetType::UNKNOWN) { return 0; } // small molecules are not connected into polymers
    else                                               { // just in case I add something late
        throw StructureException("this set type is not currently supported");
    }

}


ResidueOP
_get_first_residues_in_chain(
        ResidueOPs const & residues,
        std::map<ResidueOP, int> const & seen) {

    auto five_prime_end = true;
    for(auto const & r1 : residues) {
        if (seen.find(r1) != seen.end()) { continue; }
        five_prime_end = true;
        for(auto const & r2 : residues) {
            if(seen.find(r2) != seen.end()) { continue; } // not sure if I need this
            if(are_residues_connected(*r1, *r2) == -1) {
                five_prime_end = false;
                break;
            }
        }
        if(five_prime_end) { return r1; }
    }
    return nullptr;

}

ResidueOP
_get_next_residue(
        ResidueOPs const & residues,
        std::map<ResidueOP, int> const & seen) {
    auto lowest = ResidueOP(nullptr);
    for(auto const & r : residues) {
        if(seen.find(r) != seen.end()) { continue; }
        if(lowest == nullptr) { lowest = r;}
        if(r->get_chain_id() < lowest->get_chain_id()) {
            lowest = r;
        }
        else if(r->get_chain_id() == lowest->get_chain_id() && r->get_num() < lowest->get_num()) {
            lowest = r;
        }
    }
    return lowest;
}


StructureOP
get_structure_from_residues(
        ResidueOPs const & residues) {

    expects<StructureException>(
            residues.size() > 0,
            "no residues were supplied in get_structure_from_residues");

    auto current = ResidueOP(nullptr);
    auto all_res = Residues();
    auto chain_cuts = Indexes();
    auto seen = std::map<ResidueOP, int>();

    // has to be all RNA OR all Protein etc
    auto res_set_type = residues[0]->get_res_set_type();
    for(auto const & r : residues) {
        if(r->get_res_set_type() != res_set_type) {
            throw StructureException(
                    "cannot generate structure with two different residue types");
        }
    }

    while(true) {
        current = _get_first_residues_in_chain(residues, seen);
        // backup get lowest residue num and chain id
        if    (current == nullptr && seen.size() != residues.size()) {
            current = _get_next_residue(residues, seen);
        }
        else if(seen.size() == residues.size()) { break; }

        seen[current] = 1;
        auto found = true;
        while(found) {
            all_res.push_back(*current);
            found = false;
            for(auto const & r : residues) {
                if(seen.find(r) != seen.end()) { continue; }
                if(are_residues_connected(*current, *r) == 1) {
                    current = r;
                    found = true;
                    break;
                }
            }
            if(found) { seen[current] = 1; }
            else {
                chain_cuts.push_back((int)all_res.size());
                break;
            }
        }
    }

    if(chain_cuts.back() != (int)all_res.size()) {
        chain_cuts.push_back((int) all_res.size());
    }

    return std::make_shared<Structure>(all_res, chain_cuts);
}


StructureOP
get_structure_from_pdb(
        String const & pdb_name,
        ResidueTypeSetOP rts,
        SetType set_type) {

    auto pdb_parser = PDBParser(rts);
    auto residues = pdb_parser.parse(pdb_name);
    if     (set_type == SetType::RNA)     { return get_structure_from_residues(residues->RNA_residues); }
    else if(set_type == SetType::PROTEIN) { return get_structure_from_residues(residues->protein_residues); }
    else if(set_type == SetType::UNKNOWN) { return get_structure_from_residues(residues->small_molecule_residues); }
    else {
        throw StructureException("residue set type not supported");
    }

}

}




























