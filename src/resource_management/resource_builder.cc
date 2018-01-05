//
// Created by Joseph Yesselman on 1/4/18.
//
#include <dirent.h>

#include <gzip/compress.hpp>
#include <resource_management/resource_builder.h>

namespace resource_management {

void
ResourceBuilder::build_ideal_helices() {
    auto helix_path = motif_dirs_path_ + "/helices/";

    DIR *pDIR;
    struct dirent *entry;

    pDIR = opendir(helix_path.c_str());
    // directory does not exist
    if(ENOENT == errno) {
        throw ResourceBuilderException("motif helix path does not exist!");
    }

    auto lib_path = base::resources_path() + "/motif_libraries/ideal_helices.db";
    auto db = util::sqlite::Database(lib_path);
    auto conn = util::sqlite::Connection(db);
    util::sqlite::create_table(conn, motif_table_);

    auto i = 0;
    while ((entry = readdir(pDIR)) != NULL) {
        auto fname = String(entry->d_name);
        if(fname.length() < 11) { continue;}
        auto spl = base::split_str_by_delimiter(fname, ".");
        if(spl[1] != "IDEAL") { continue; }

        auto path = helix_path + fname + "/" + fname + ".pdb";
        auto segs = seg_f_.all_segments_from_pdb(path, util::SegmentType::HELIX, false);

        seg_f_.align_segment_to_ref_frame(*segs[0]);
        //segs[0]->write_pdb("test."+std::to_string(i)+".pdb");
        //i++;

        exit(0);



    }

    closedir(pDIR);
    delete entry;
}

}