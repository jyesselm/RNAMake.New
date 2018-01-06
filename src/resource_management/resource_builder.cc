//
// Created by Joseph Yesselman on 1/4/18.
//
#include <dirent.h>

#include <base/string.h>
#include <base/gzip/compress.hpp>
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

    try { std::remove(lib_path.c_str()); }
    catch(...) { }

    auto db = util::sqlite::Database(lib_path);
    auto conn = util::sqlite::Connection(db);
    util::sqlite::create_table(conn, motif_table_);

    auto i = 0;
    auto start_insert_str = String("INSERT INTO data_table (id, data, name, end_name, end_id) VALUES (");
    auto insert_str = String("");

    while ((entry = readdir(pDIR)) != NULL) {
        auto fname = String(entry->d_name);
        if(fname.length() < 10) { continue;}
        auto spl = base::split_str_by_delimiter(fname, ".");
        if(spl[1] != "IDEAL") { continue; }

        auto path = helix_path + fname + "/" + fname + ".pdb";
        auto segs = seg_f_.all_segments_from_pdb(path, util::SegmentType::HELIX, false);

        seg_f_.align_segment_to_ref_frame(*segs[1]);

        auto j = segs[0]->get_json();
        auto j_str = j.dump_minimized();
        compressed_str_ = base::gzip::compress(j_str.c_str(), j_str.size());
        blob_ = std::vector<uint8_t>(compressed_str_.begin(), compressed_str_.end());
        segs[1]->write_pdb(segs[0]->get_name()->get_str() + ".pdb");
        std::cout << segs[0]->get_name()->get_str() << " " << segs[0]->get_end(0).get_name()->get_str() << std::endl;

        conn.bind(1, blob_);
        insert_str  = start_insert_str + base::quoted_string(std::to_string(i)) + ", ?, ";
        insert_str += base::quoted_string(segs[0]->get_name()->get_str()) + ", ";
        insert_str += base::quoted_string(segs[0]->get_end(0).get_name()->get_str()) + ", ";
        insert_str += base::quoted_string(segs[0]->get_end_id(0)->get_str()) + ");";
        conn.exec(insert_str);
        i++;

    }

    closedir(pDIR);
    delete entry;
}

}