//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include <dirent.h>
#include <fstream>
#include "../common.hpp"

#include <base/file_io.h>
#include <all_atom/segment_factory.h>

TEST_CASE( "Parse all pdbs to poses", "[PDBParser]" ) {
    init_unittest_safe_logging(base::LogLevel::WARN);

    auto rts = all_atom::ResidueTypeSet();
    auto seg_factory = all_atom::SegmentFactory(rts);
    auto paths = Strings{
            "/Users/jyesselm/projects/RNAMake/rnamake/resources/motifs/two_ways/",
            "/Users/jyesselm/projects/RNAMake/rnamake/resources/motifs/hairpins/",
            "/Users/jyesselm/projects/RNAMake/rnamake/resources/motifs/helices/",
            "/Users/jyesselm/projects/RNAMake/rnamake/resources/motifs/junctions/",
            "/Users/jyesselm/projects/RNAMake/rnamake/resources/motifs/tertiary_contact_hairpin_hairpin/",
            "/Users/jyesselm/projects/RNAMake/rnamake/resources/motifs/tertiary_contact_junction_hairpin/",
            "/Users/jyesselm/projects/RNAMake/rnamake/resources/motifs/tertiary_contact_junction_junction/"
    };

    auto seg_types = std::vector<util::SegmentType> {
            util::SegmentType::TWOWAY_JUNCTION,
            util::SegmentType::HAIRPIN,
            util::SegmentType::HELIX,
            util::SegmentType::NWAY_JUNCTION,
            util::SegmentType::TC_HAIRPIN_HAIRPIN,
            util::SegmentType::TC_JUNCTION_HAIRPIN,
            util::SegmentType ::TC_JUNCTION_JUNCTION
    };

    auto explained_failure = std::map<String, int>();
    explained_failure["TWOWAY.1MSW.0"] = 1;  // all DNA
    explained_failure["HAIRPIN.3AGV.0"] = 1; // no WC basepair
    explained_failure["HELIX.2QKB.0"] = 1;   // all DNA
    explained_failure["HELIX.2O5I.3"] = 1;   // all DNA
    explained_failure["HELIX.3DVZ.0"] = 1;   // all DNA
    explained_failure["HELIX.4OO8.5"] = 1;   // all DNA
    explained_failure["HELIX.2O5I.2"] = 1;   // all DNA
    explained_failure["HELIX.3SSF.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.1ZBI.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.1MSW.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.2G91.0"] = 1;   // missing O2' at end basepairs
    explained_failure["HELIX.421D.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.3BSU.2"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.3ULD.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.3BSU.3"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.1QLN.0"] = 1;   // all DNA
    explained_failure["HELIX.4BWM.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.2O5I.0"] = 1;   // all DNA
    explained_failure["HELIX.4OO8.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.2O5I.1"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.479D.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.4H8K.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.377D.1"] = 1;   // missing O2' at end basepairs
    explained_failure["HELIX.3BSU.1"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.3BSU.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.377D.0"] = 1;   // DNA/RNA hybrid
    explained_failure["HELIX.4C8Y.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.1J9H.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.1FUF.0"] = 1;   // one missing O2' 3 chains
    explained_failure["HELIX.1J9H.1"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.1FUF.1"] = 1;   // one missing O2' 3 chains
    explained_failure["HELIX.3ZP8.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.4ILL.1"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.2Y9H.3"] = 1;   // one missing O2' 3 chains
    explained_failure["HELIX.2Y9H.4"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.398D.1"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.2Y9H.5"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.2Y9H.2"] = 1;   // one missing O2' 3 chains
    explained_failure["HELIX.398D.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.3AVX.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.2Y8W.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.1EHZ.3"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.1J9H.3"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.1J9H.2"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.2Y8Y.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.1EHZ.2"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.2NZ4.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.1F7U.2"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.2Y9H.0"] = 1;   // one missing O2' 3 chains
    explained_failure["HELIX.2Y9H.1"] = 1;   // one missing O2' 3 chains
    explained_failure["HELIX.2Y9H.6"] = 1;   // one missing O2' 3 chains
    explained_failure["HELIX.3NKB.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["HELIX.2XLI.0"] = 1;   // one end missing O2' thus only one end
    explained_failure["TWOWAY.1J9H.3"] = 1;  // one end missing O2' thus only one end
    explained_failure["TWOWAY.1J9H.2"] = 1;  // one end missing O2' thus only one end
    explained_failure["TWOWAY.409D.0"] = 1;  // missing N2s on Gs
    explained_failure["TWOWAY.409D.1"] = 1;  // missing N2s on Gs
    explained_failure["TWOWAY.1JBS.0"] = 1;  // one end has a U-U pair CHECK
    explained_failure["TWOWAY.1JBS.1"] = 1;  // one end has a U-U pair CHECK
    explained_failure["TWOWAY.1J9H.0"] = 1;  // one end missing O2' thus only one end
    explained_failure["TWOWAY.1J9H.1"] = 1;  // one end missing O2' thus only one end
    explained_failure["TWOWAY.354D.0"] = 1;  // one end missing an residue CHECK
    explained_failure["TWOWAY.409D.2"] = 1;  // missing N2s on Gs
    explained_failure["HAIRPIN.1JBS.1"] = 1; // one missing O2' 2 chains
    explained_failure["HAIRPIN.1JBS.0"] = 1; // one missing O2' 2 chains
    explained_failure["HAIRPIN.1C0A.0"] = 1; // one missing O2' 2 chains
    explained_failure["HAIRPIN.1F7U.0"] = 1; // missing N2s on Gs


    int i = -1;
    for(auto const & path : paths) {
        i++;
        //if(seg_types[i] != util::SegmentType::TC_JUNCTION_JUNCTION) { continue; }

        DIR *pDIR;
        struct dirent *entry;

        pDIR = opendir(path.c_str());
        // directory does not exist
        if (ENOENT == errno) { exit(0); }

        while ((entry = readdir(pDIR)) != NULL) {
            auto fname = String(entry->d_name);
            if (fname.size() < 5) { continue; }
            if (explained_failure.find(fname) != explained_failure.end()) { continue; }
            auto pdb_path = path + "/" + fname + "/" + fname + ".pdb";
            if (!base::is_dir(path + "/" + fname)) { continue; }
            std::cout << pdb_path << std::endl;
            auto segments = seg_factory.all_segments_from_pdb(pdb_path, seg_types[i], false);
            /*
            auto i = -1;
            for(auto & seg : segments) {
                i++;
                seg_factory.align_segment_to_ref_frame(*seg);
                seg->write_pdb("test."+std::to_string(i)+".pdb");
            }*/
        }
        closedir(pDIR);
        delete entry;

    }

}