//
// Created by Joseph Yesselman on 12/20/17.
//

#ifndef RNAMAKE_NEW_SEGMENT_FACTORY_H
#define RNAMAKE_NEW_SEGMENT_FACTORY_H

#include <util/x3dna.h>
#include <all_atom/aligner.h>
#include <all_atom/segment.h>
#include <all_atom/pdb_parser.h>
#include <all_atom/pose.h>
#include <all_atom/residue_type_set.h>

namespace all_atom {

class SegmentFactoryException : public std::runtime_error {
public:
    SegmentFactoryException(
            String const & message):
            std::runtime_error(message) {}
};

class SegmentFactory {
public:
    struct SegmentElements {
        base::SimpleStringCOP name;
        base::SimpleStringCOPs end_ids;
        Structure rna, proteins, small_molecules;
        Basepairs basepairs;
        Basepairs ends;

        inline
        SegmentElements(
                base::SimpleStringCOP nname,
                Structure const & rna_struc,
                Structure const & protein_struc,
                Structure const & small_molecule_struc,
                Basepairs const & bps,
                Basepairs const & nends):
                name(nname),
                end_ids(base::SimpleStringCOPs()),
                rna(rna_struc),
                proteins(protein_struc),
                small_molecules(small_molecule_struc),
                basepairs(bps),
                ends(nends) {}

        inline
        void
        move(
                math::Point const & p) {
            rna.move(p);
            proteins.move(p);
            small_molecules.move(p);
            for(auto & bp : basepairs) { bp.move(p); }
            for(auto & bp : ends) { bp.move(p); }
        }

        inline
        void
        transform(
                math::Matrix const & r,
                math::Point const & t) {
            auto dummy = math::Point();
            rna.transform(r, t, dummy);
            proteins.transform(r, t, dummy);
            small_molecules.transform(r, t, dummy);
            for(auto & bp : basepairs) { bp.transform(r, t, dummy); }
            for(auto & bp : ends) { bp.transform(r, t, dummy); }
        }

    };

    typedef std::shared_ptr<SegmentElements> SegmentElementsOP;

public:
    SegmentFactory(
            ResidueTypeSetCOP rts):
            rts_(rts),
            x3dna_(util::X3dna()),
            pdb_parser_(PDBParser(rts)) {
        // dont need to rebuild x3dna files for ref motifs
        x3dna_.set_rebuild_files(false);
        ref_motif_  = _setup_ref_motif();
        base_helix_ = _setup_base_helix();
        added_helix_ = std::make_shared<Segment>(*base_helix_);
        x3dna_.set_rebuild_files(true);
    }

public:
    SegmentOP
    segment_from_pdb(
            String const & pdb_path,
            util::SegmentType segment_type = util::SegmentType::SEGMENT,
            bool rebuild_x3dna_files = true);

    SegmentOPs
    all_segments_from_pdb(
            String const & pdb_path,
            util::SegmentType segment_type = util::SegmentType::SEGMENT,
            bool rebuild_x3dna_files = true);

    void
    align_segment_to_ref_frame(
            Segment &);

private:
    PoseOP
    _setup_ref_motif();

    SegmentOP
    _setup_base_helix();

    SegmentElementsOP
    _get_segment_elements_from_pdb(
            String const &,
            bool);

private:

    void
    _setup_end_ids(
            Structure const &,
            Basepairs const &,
            Basepairs const &,
            base::SimpleStringCOPs &);

    StructureOP
    _get_aligned_structure(
            Structure const &);

    void
    _get_aligned_ends(
            Structure const &,
            Basepairs &);

    void
    _remove_ends_from_basepairs(
            Basepairs const &,
            Basepairs &);

    void
    _remove_beads_from_end_res(
            Basepairs const &,
            Structure &);

    void
    _standardize_motif_elements(
            SegmentElements &,
            Index);

    void
    _align_motif_elements_to_frame(
            Basepair const &,
            SegmentElements &,
            Index);

    void
    _align_motif_elements_back_to_org_frame(
            Basepairs const &,
            Structure const &,
            SegmentElements &);

    void
    _align_segment_to_frame(
            Basepair const &,
            Segment &);

    SegmentOP
    _get_aligned_segment(
            Basepair const &,
            Segment &);

    int
    _num_steric_clashes(
            SegmentElements const &,
            Segment const &);

    int
    _are_structures_overlaid(
            Structure const &,
            Structure const &);



private:
    ResidueTypeSetCOP rts_;
    Aligner aligner_;
    util::X3dna x3dna_;
    PDBParser pdb_parser_;
    PoseOP ref_motif_;
    SegmentOP base_helix_, added_helix_;
    String name_;
    // variables for aligning
    math::Point sugar_diff_1_, sugar_diff_2_, avg_sugar_diff_;
    double sugar_dist_1_, sugar_dist_2_;
};

typedef std::shared_ptr<SegmentFactory> SegmentFactoryOP;


}

#endif //RNAMAKE_NEW_SEGMENT_FACTORY_H
