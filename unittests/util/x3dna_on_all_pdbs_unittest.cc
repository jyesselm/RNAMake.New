//
// Created by Joseph Yesselman on 12/10/17.
//

#include <iostream>
#include <dirent.h>
#include <fstream>
#include "../common.hpp"

#include <base/file_io.h>
#include <util/x3dna.h>

TEST_CASE( "run x3dna on all pdbs", "[X3dnaParser]" ) {
    auto path = String("/Users/jyesselm/projects/REDESIGN/resources/non-redundant-rnas/");

    DIR *pDIR;
    struct dirent *entry;

    auto x = util::X3dna();
    x.set_rebuild_files(false);

    pDIR = opendir(path.c_str());
    // directory does not exist
    if(ENOENT == errno) { exit(0); }

    while ((entry = readdir(pDIR)) != NULL) {
        auto fname = String(entry->d_name);
        //if(fname != "2EZ6") { continue; }
        if(fname.length() != 4) { continue; }

        if(!base::file_exists(path + "/" + fname + "/ref_frames.dat")) { continue; }

        auto x3dna_bps = x.get_basepairs(path + "/" + fname + "/" + fname + ".pdb");
    }
    closedir(pDIR);
    delete entry;

}