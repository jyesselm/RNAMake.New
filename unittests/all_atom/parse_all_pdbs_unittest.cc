//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include <dirent.h>
#include <fstream>
#include "../common.hpp"

#include <base/logger.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/structure.h>


TEST_CASE( "Parse all pdbs in set ", "[PDBParser]" ) {
    auto rts = all_atom::ResidueTypeSet();
    auto path = String("/Users/jyesselm/projects/REDESIGN/resources/non-redundant-rnas/");

    DIR *pDIR;
    struct dirent *entry;

    pDIR = opendir(path.c_str());
    // directory does not exist
    if(ENOENT == errno) { exit(0); }

    while ((entry = readdir(pDIR)) != NULL) {
        auto fname = String(entry->d_name);
        //if(fname != "2EZ6") { continue; }
        if(fname.length() != 4) { continue; }
        std::cout << fname << std::endl;
        auto pdb_parser = all_atom::PDBParser(rts);
        auto residues = pdb_parser.parse(path + "/" + fname + "/" + fname + ".pdb");
        if(residues->has_RNA()) {
            auto rna_struc = all_atom::get_structure_from_residues(residues->RNA_residues);
        }
        if(residues->has_protein()) {
            auto protein_struc = all_atom::get_structure_from_residues(residues->protein_residues);
        }
        if(residues->has_small_molecules()) {
            auto sm_struc = all_atom::get_structure_from_residues(residues->small_molecule_residues);

        }

    }
    closedir(pDIR);
    delete entry;

}