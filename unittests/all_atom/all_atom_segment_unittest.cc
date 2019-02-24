//
// Created by Joseph Yesselman on 12/3/17.
//

#include <iostream>
#include "../common.hpp"

#include <math/numerical.h>
#include <util/random_number_generator.h>
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
        }

        // test loading from json
        auto j = seg->get_json();
        auto seg2 = all_atom::Segment(j, rts);
        REQUIRE(seg2.is_equal(*seg, CheckUUID::NO));

        auto p = math::Point(2, 2, 2);
        seg2.move(p);
        REQUIRE(!seg2.is_equal(*seg, CheckUUID::NO));

        // test copying
        seg2 = all_atom::Segment(*seg);
        REQUIRE(seg2 == *seg);

        seg2.move(p);
        REQUIRE(seg2 != *seg);

    }

    SECTION("test sterics") {
        auto rts = all_atom::ResidueTypeSet();
        auto seg_factory = all_atom::SegmentFactory(rts);
        auto path = base::unittest_resources_path() + "/all_atom/HELIX.IDEAL.2/HELIX.IDEAL.2.pdb";
        auto seg = seg_factory.segment_from_pdb(path, util::SegmentType::HELIX, false);

        auto seg2 = all_atom::Segment(*seg);

        SECTION("test RNA/RNA steric clashes") {
            auto steric_clash_count = seg2.get_num_steric_clashes(*seg);

            REQUIRE(steric_clash_count > 1);
            REQUIRE(seg2.steric_clash(*seg) == true);

            auto p = math::Point(100, 100, 100);
            seg2.move(p);
            steric_clash_count = seg2.get_num_steric_clashes(*seg);

            REQUIRE(steric_clash_count == 0);
            REQUIRE(seg2.steric_clash(*seg) == false);

        }

        SECTION("test RNA / protein steric clashes") {
            auto path = base::unittest_resources_path() + "/all_atom/6g19.pdb";
            auto seg_w_protein = seg_factory.segment_from_pdb(path, util::SegmentType::HELIX, false);

            seg_factory.align_segment_to_ref_frame(seg2);
            seg_factory.align_segment_to_ref_frame(*seg_w_protein);

            auto p = math::Point(25, 0, 0);
            seg2.move(p);

            auto steric_clash_count = seg2.get_num_steric_clashes(*seg_w_protein);

            REQUIRE(steric_clash_count > 1);
            REQUIRE(seg2.steric_clash(*seg_w_protein) == true);

            p = math::Point(100, 100, 100);
            seg2.move(p);

            steric_clash_count = seg2.get_num_steric_clashes(*seg_w_protein);

            REQUIRE(steric_clash_count == 0);
            REQUIRE(seg2.steric_clash(*seg_w_protein) == false);
        }
    }

    SECTION("test conversion to secondary structure") {
        auto rts = all_atom::ResidueTypeSet();
        auto seg_factory = all_atom::SegmentFactory(rts);
        auto path = base::unittest_resources_path() + "/all_atom/HELIX.IDEAL.2/HELIX.IDEAL.2.pdb";
        auto seg = seg_factory.segment_from_pdb(path, util::SegmentType::HELIX, false);

        auto seg_ss = seg->get_secondary_structure();

        REQUIRE(seg_ss->get_name() == seg->get_name());
        REQUIRE(seg_ss->get_num_chains() == seg->get_num_chains());
        REQUIRE(seg_ss->get_num_residues() == seg->get_num_residues());
        REQUIRE(seg_ss->get_num_basepairs() == seg->get_num_basepairs());
        REQUIRE(seg_ss->get_num_ends() == seg->get_num_ends());

        //secondary structure specific
        REQUIRE(seg_ss->get_dot_bracket() == seg->get_dot_bracket());
        REQUIRE(seg_ss->get_sequence() == seg->get_sequence());

        // can change sequence in secondary structure, should be different from all_atom segment
        seg_ss->set_residue_identity(0, 'A');

        REQUIRE(seg_ss->get_sequence() != seg->get_sequence());

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

        REQUIRE(seg1->is_equal(*seg2, CheckUUID::NO));


    }



}
