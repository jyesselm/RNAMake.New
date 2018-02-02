//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include "../common.hpp"

#include <all_atom/residue_type_set.h>
#include <all_atom/segment_factory.h>

TEST_CASE( "Test all atom segment", "[AllAtomSegments]" ) {
    init_unittest_safe_logging();

    SECTION("test loading segments from pdbs") {
        auto rts = all_atom::ResidueTypeSet();
        auto seg_factory = all_atom::SegmentFactory(rts);
        auto path = base::unittest_resources_path() + "/all_atom/HELIX.IDEAL.2/HELIX.IDEAL.2.pdb";
        auto seg = seg_factory.segment_from_pdb(path, util::SegmentType::HELIX, false);

        auto segs = seg_factory.all_segments_from_pdb(path, util::SegmentType::HELIX, false);

        int i = -1;
        for (auto & seg: segs) {
            i++;
            seg_factory.align_segment_to_ref_frame(*seg);
            seg->write_pdb("test." + std::to_string(i) + ".pdb");
        }

        auto j = seg->get_json();
        auto seg2 = all_atom::Segment(j, rts);
        REQUIRE(seg2.is_equal(*seg, false));

        auto p = math::Point(2, 2, 2);
        seg2.move(p);
        REQUIRE(!seg2.is_equal(*seg, false));
    }

    SECTION("test generating segments from components") {
        auto rts = all_atom::ResidueTypeSet();
        auto seg_factory = all_atom::SegmentFactory(rts);
        auto path = base::unittest_resources_path() + "/all_atom/HELIX.IDEAL.2/HELIX.IDEAL.2.pdb";

        auto parser = all_atom::PDBParser(rts);
        auto residues = parser.parse(path);

        auto rna_structure = all_atom::get_structure_from_residues(residues->RNA_residues);

        auto x = util::X3dna();
        x.set_rebuild_files(false);
        auto x3dna_bps = x.get_basepairs(path);
        auto bps = get_basepairs_from_x3dna(x3dna_bps, *rna_structure)->get_data();

        auto proteins = all_atom::Structure(all_atom::Residues(), Cutpoints());
        auto small_molecules = all_atom::Structure(all_atom::Residues(), Cutpoints());

        auto seg1 = seg_factory.segment_from_components("HELIX.IDEAL.2", *rna_structure, bps, proteins,
                                                        small_molecules, util::SegmentType::HELIX);

        auto seg2 = seg_factory.segment_from_pdb(path, util::SegmentType::HELIX, false);

        REQUIRE(seg1->is_equal(*seg2, false));




    }



}
