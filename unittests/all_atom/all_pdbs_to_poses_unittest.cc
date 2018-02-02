//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include <dirent.h>
#include <fstream>
#include "../common.hpp"

#include <base/log.h>
#include <all_atom/residue_type_set.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/pose.h>

TEST_CASE( "Parse all pdbs to poses", "[PDBParser]" ) {

    auto rts = all_atom::ResidueTypeSet();
    auto pdb_parser = all_atom::PDBParser(rts);
    auto path = String("/Users/jyesselm/projects/REDESIGN/resources/non-redundant-rnas/");

    DIR *pDIR;
    struct dirent *entry;

    pDIR = opendir(path.c_str());
    // directory does not exist
    if(ENOENT == errno) { exit(0); }

    while ((entry = readdir(pDIR)) != NULL) {
        auto fname = String(entry->d_name);
        //if(fname != "1DUQ") { continue; }
        if(fname.length() != 4) { continue; }
        std::cout << fname << std::endl;
        auto p = all_atom::get_pose_from_pdb(path + "/" + fname + "/" + fname + ".pdb", pdb_parser);

    }
    closedir(pDIR);
    delete entry;

}