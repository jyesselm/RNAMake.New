//
// Created by Joseph Yesselman on 12/20/17.
//

#include <base/paths.h>
#include <base/logger.h>
#include <util/segment_type.h>
#include <all_atom/segment_factory.h>

namespace all_atom {

SegmentOP
SegmentFactory::segment_from_pdb(
        String const & pdb_path,
        util::SegmentType segment_type,
        bool rebuild_x3dna_files) {

    auto me = _get_segment_elements_from_pdb(pdb_path, rebuild_x3dna_files);
    auto org_struc = me->rna;
    auto org_ends = me->ends;

    LOG_INFO("SegmentFactory",
             "creating segment from pdb: " + pdb_path + " with aligned end: " +
             me->ends[0].get_name()->get_str() );

    _standardize_motif_elements(*me, 0);
    _align_motif_elements_to_frame(ref_motif_->get_end(0), *me, 0);

    me->rna = *(_get_aligned_structure(me->rna));
    _get_aligned_ends(me->rna, me->ends);
    _remove_beads_from_end_res(me->ends, me->rna);
    _setup_end_ids(me->rna, me->basepairs, me->ends, me->end_ids);

    auto dot_bracket_str = primitives::get_dot_bracket_from_end_id(me->end_ids[0]->get_str());
    auto dot_bracket = std::make_shared<base::SimpleString>(dot_bracket_str);

    _align_motif_elements_back_to_org_frame(org_ends, org_struc, *me);
    auto aligned_end = 0;
    if(segment_type == util::SegmentType::HAIRPIN) { aligned_end = -1; }

    return std::make_shared<Segment>(me->rna, me->basepairs, me->ends, me->end_ids, me->name,
                                     me->proteins, me->small_molecules, dot_bracket,
                                     segment_type, aligned_end);
}

SegmentOPs
SegmentFactory::all_segments_from_pdb(
        String const & pdb_path,
        util::SegmentType segment_type,
        bool rebuild_x3dna_files) {

    auto segments = SegmentOPs();
    auto org_me = _get_segment_elements_from_pdb(pdb_path, rebuild_x3dna_files);

    for(int i = 0; i < org_me->ends.size(); i++) {
        auto me = std::make_shared<SegmentElements>(*org_me);

        LOG_INFO("SegmentFactory",
                 "creating segment from pdb: " + pdb_path + " with aligned end: " +
                 me->ends[i].get_name()->get_str());

        _standardize_motif_elements(*me, i);
        _align_motif_elements_to_frame(ref_motif_->get_end(0), *me, i);

        me->rna = *(_get_aligned_structure(me->rna));
        _get_aligned_ends(me->rna, me->ends);
        _remove_beads_from_end_res(me->ends, me->rna);
        _setup_end_ids(me->rna, me->basepairs, me->ends, me->end_ids);

        auto dot_bracket_str = primitives::get_dot_bracket_from_end_id(me->end_ids[0]->get_str());
        auto dot_bracket = std::make_shared<base::SimpleString>(dot_bracket_str);

        _align_motif_elements_back_to_org_frame(org_me->ends, org_me->rna, *me);
        auto aligned_end = 0;
        if (segment_type == util::SegmentType::HAIRPIN) { aligned_end = -1; }

        auto seg = std::make_shared<Segment>(me->rna, me->basepairs, me->ends, me->end_ids, me->name,
                                             me->proteins, me->small_molecules, dot_bracket,
                                             segment_type, aligned_end);
        segments.push_back(seg);

    }
    return segments;
}

void
SegmentFactory::align_segment_to_ref_frame(
        Segment & seg) {
    aligner_.align(ref_motif_->get_end(0), seg);
}

void
SegmentFactory::_standardize_motif_elements(
        SegmentElements & m_elements,
        Index end_index) {
    _align_motif_elements_to_frame(base_helix_->get_end(1), m_elements, end_index);

    // figure out whats the best way to add this segment to the base helix
    auto steric_clashes_1 =  _num_steric_clashes(m_elements, *base_helix_);
    auto steric_clashes_2 = 0;
    if(steric_clashes_1 > 2) {
        m_elements.ends[end_index].invert_reference_frame();
        _align_motif_elements_to_frame(base_helix_->get_end(1), m_elements, end_index);
        steric_clashes_2 =  _num_steric_clashes(m_elements, *base_helix_);
        if(steric_clashes_2 != 2) {
            LOG_WARNING("SegmentFactory",
                        " there is no aligment without clashes this may lead to issues during building!");
        }
        if(steric_clashes_2 > steric_clashes_1) {
            m_elements.ends[end_index].invert_reference_frame();
        }
    }

    // fix all base pair ends so new segments can be built on them
    for(int i = 0; i < m_elements.ends.size(); i++) {
        if(i == end_index) { continue; }
        auto aligned_helix = _get_aligned_segment(m_elements.ends[i], *added_helix_);
        steric_clashes_1  = _num_steric_clashes(m_elements, *aligned_helix);
        steric_clashes_1 += base_helix_->get_num_steric_clashes(*aligned_helix);
        if(steric_clashes_1 == 2) { continue; }

        m_elements.ends[i].invert_reference_frame();
        aligned_helix = _get_aligned_segment(m_elements.ends[i], *added_helix_);
        steric_clashes_2  = _num_steric_clashes(m_elements, *aligned_helix);
        steric_clashes_2 += base_helix_->get_num_steric_clashes(*aligned_helix);

        if(steric_clashes_2 != 2) {
            LOG_WARNING("SegmentFactory",
                        "basepair end: " + m_elements.ends[i].get_name()->get_str() +
                        " generates clashes when helix is added this may lead toissues during building!");
        }

        if(steric_clashes_2 > steric_clashes_1) {
            m_elements.ends[i].invert_reference_frame();
        }

    }


}

void
SegmentFactory::_align_motif_elements_to_frame(
        Basepair const & ref_bp,
        SegmentElements & m_elements,
        Index end_index) {

    auto rot = dot(ref_bp.get_ref_frame().get_transposed(),
                   m_elements.ends[end_index].get_ref_frame());
    rot.unitarize();
    auto r_t = rot.get_transposed();
    auto t = -ref_bp.get_center();
    m_elements.transform(r_t, t);
    m_elements.move(ref_bp.get_center() - m_elements.ends[end_index].get_center());

    // do additional sugar alignment
    sugar_dist_1_ = ref_bp.get_res1_c1_prime_coord().distance(m_elements.ends[end_index].get_res1_c1_prime_coord());
    sugar_dist_2_ = ref_bp.get_res1_c1_prime_coord().distance(m_elements.ends[end_index].get_res2_c1_prime_coord());

    if(sugar_dist_1_ > 5 && sugar_dist_2_ > 5) {
        LOG_WARNING("SegmentFactory",
                    "difference in sugar c1' coords between reference and aligned is greater than 5. "
                    "This could lead to alignment issues!");
        return;
    }

    if(sugar_dist_1_ < 5) {
        sugar_diff_1_ =  ref_bp.get_res1_c1_prime_coord() - m_elements.ends[end_index].get_res1_c1_prime_coord();
        sugar_diff_2_ =  ref_bp.get_res2_c1_prime_coord() - m_elements.ends[end_index].get_res2_c1_prime_coord();
    }
    else {
        sugar_diff_1_ =  ref_bp.get_res1_c1_prime_coord() - m_elements.ends[end_index].get_res1_c1_prime_coord();
        sugar_diff_2_ =  ref_bp.get_res2_c1_prime_coord() - m_elements.ends[end_index].get_res2_c1_prime_coord();
    }
    avg_sugar_diff_ = (sugar_diff_1_ + sugar_diff_2_) / 2;
    m_elements.move(avg_sugar_diff_);

}

void
SegmentFactory::_align_motif_elements_back_to_org_frame(
        Basepairs const & org_ends,
        Structure const & org_struc,
        SegmentElements & me) {

    Basepair *  org_aligned_end = nullptr;
    for(auto & end : org_ends) {
        if(end.get_name() == me.ends[0].get_name()) {
            org_aligned_end = new Basepair(end);
        }
    }

    _align_motif_elements_to_frame(*org_aligned_end, me, 0);
    auto are_overlaid = _are_structures_overlaid(org_struc, me.rna);
    if(are_overlaid) {
        delete org_aligned_end;
        return;
    }

    org_aligned_end->invert_reference_frame();
    _align_motif_elements_to_frame(*org_aligned_end, me, 0);
    are_overlaid = _are_structures_overlaid(org_struc, me.rna);
    delete org_aligned_end;

    if(!are_overlaid) {
        throw SegmentFactoryException("cannot return segment to original reference frame!");
    }

}

void
SegmentFactory::_align_segment_to_frame(
        Basepair const & ref_bp,
        Segment & seg) {

    auto rot = dot(ref_bp.get_ref_frame().get_transposed(), seg.get_aligned_end().get_ref_frame());
    rot.unitarize();
    auto r_t = rot.get_transposed();
    auto t = -ref_bp.get_center();

    seg.transform(r_t, t);
    seg.move(ref_bp.get_center() - seg.get_aligned_end().get_center());

    // do additional sugar alignment
    sugar_dist_1_ = ref_bp.get_res1_c1_prime_coord().distance(seg.get_aligned_end().get_res1_c1_prime_coord());
    sugar_dist_2_ = ref_bp.get_res1_c1_prime_coord().distance(seg.get_aligned_end().get_res2_c1_prime_coord());

    if(sugar_dist_1_ > 5 && sugar_dist_2_ > 5) {
        LOG_WARNING("SegmentFactory",
                    "difference in sugar c1' coords between reference and aligned is greater than 5. "
                            "This could lead to alignment issues!");
        return;
    }

    if(sugar_dist_1_ < 5) {
        sugar_diff_1_ =  ref_bp.get_res1_c1_prime_coord() - seg.get_aligned_end().get_res1_c1_prime_coord();
        sugar_diff_2_ =  ref_bp.get_res2_c1_prime_coord() - seg.get_aligned_end().get_res2_c1_prime_coord();
    }
    else {
        sugar_diff_1_ =  ref_bp.get_res1_c1_prime_coord() - seg.get_aligned_end().get_res1_c1_prime_coord();
        sugar_diff_2_ =  ref_bp.get_res2_c1_prime_coord() - seg.get_aligned_end().get_res2_c1_prime_coord();
    }

    avg_sugar_diff_ = (sugar_diff_1_ + sugar_diff_2_) / 2;
    seg.move(avg_sugar_diff_);

}

SegmentOP
SegmentFactory::_get_aligned_segment(
        Basepair const & ref_bp,
        Segment & seg) {
    auto seg_copy = std::make_shared<Segment>(seg);
    _align_segment_to_frame(ref_bp, *seg_copy);
    return seg_copy;
}

int
SegmentFactory::_num_steric_clashes(
        SegmentElements const & m_elements,
        Segment const & seg) {

    int steric_clash_count = 0;
    for(auto const & r1 : seg) {
        for(auto const & r2 : m_elements.rna) {
            if(residue_steric_clash_RNA(r1, r2)) { steric_clash_count += 1; }
        }

        for(auto const & r2 : m_elements.proteins) {
            if(residue_steric_clash_RNA(r1, r2)) { steric_clash_count += 1; }
        }

        for(auto const & r2 : m_elements.small_molecules) {
            if(residue_steric_clash_RNA(r1, r2)) { steric_clash_count += 1; }
        }
    }

    return steric_clash_count;


}

int
SegmentFactory::_are_structures_overlaid(
        Structure const & s1,
        Structure const & s2) {

    for(auto const & r1 : s1)  {
        auto & r2 = s2.get_residue(r1.get_uuid());
        if(r1.get_center().distance(r2.get_center()) > 0.1) { return 0; }
    }
    return 1;

}

PoseOP
SegmentFactory::_setup_ref_motif() {
    auto path = base::resources_path() + "/required_motifs/ref_motif/ref_motif.pdb";
    auto me = _get_segment_elements_from_pdb(path, false);

    auto end_ids = base::SimpleStringCOPs();
    _setup_end_ids(me->rna, me->basepairs, me->ends, end_ids);

    auto dot_bracket_str = primitives::get_dot_bracket_from_end_id(end_ids[0]->get_str());
    auto dot_bracket = std::make_shared<base::SimpleString>(dot_bracket_str);

    return std::make_shared<Pose>(me->rna, me->basepairs, me->ends, end_ids, me->name,
                                  me->proteins, me->small_molecules, dot_bracket);
}

SegmentOP
SegmentFactory::_setup_base_helix() {
    auto path = base::resources_path() + "/required_motifs/base_helix/base_helix.pdb";
    auto me = _get_segment_elements_from_pdb(path, false);

    _align_motif_elements_to_frame(ref_motif_->get_end(0), *me, 0);

    auto aligned_s = _get_aligned_structure(me->rna);
    _get_aligned_ends(*aligned_s, me->ends);
    _remove_beads_from_end_res(me->ends, *aligned_s);

    auto end_ids = base::SimpleStringCOPs();
    _setup_end_ids(*aligned_s, me->basepairs, me->ends, end_ids);

    auto dot_bracket_str = primitives::get_dot_bracket_from_end_id(end_ids[0]->get_str());
    auto dot_bracket = std::make_shared<base::SimpleString>(dot_bracket_str);

    return std::make_shared<Segment>(*aligned_s, me->basepairs, me->ends, end_ids, me->name,
                                     me->proteins, me->small_molecules, dot_bracket,
                                     util::SegmentType::HELIX, 0);
}

SegmentFactory::SegmentElementsOP
SegmentFactory::_get_segment_elements_from_pdb(
        String const & pdb_path,
        bool rebuild_x3dna_files) {

    auto filename = base::filename(pdb_path);
    auto name = std::make_shared<base::SimpleString>(filename.substr(0, filename.length() - 4));

    auto residues = pdb_parser_.parse(pdb_path);
    if(! residues->has_RNA()) {
        throw SegmentFactoryException("Segments must contain RNA!");
    }

    auto rna_structure = get_structure_from_residues(residues->RNA_residues);
    auto protein_structure = std::make_shared<Structure>(Residues(), Cutpoints());
    auto small_molecules = std::make_shared<Structure>(Residues(), Cutpoints());

    if(residues->has_protein()) {
        protein_structure = get_structure_from_residues(residues->protein_residues);
    }
    if(residues->has_small_molecules()) {
        small_molecules = get_structure_from_residues(residues->small_molecule_residues);
    }

    if(!rebuild_x3dna_files) { x3dna_.set_rebuild_files(false); }
    auto x3dna_basepairs = x3dna_.get_basepairs(pdb_path);
    auto bps = get_basepairs_from_x3dna(x3dna_basepairs, *rna_structure)->get_data();
    auto ends = get_ends_from_basepairs(*rna_structure, bps)->get_data();
    _remove_ends_from_basepairs(ends, bps);

    if(ends.size() == 0) {
        throw SegmentFactoryException("Segments must contain at least one basepair end!");
    }

    x3dna_.set_rebuild_files(true);
    return std::make_shared<SegmentElements>(name, *rna_structure, *protein_structure, *small_molecules,
                                             bps, ends);


}

void
SegmentFactory::_setup_end_ids(
        Structure const & s,
        Basepairs const & bps,
        Basepairs const & ends,
        base::SimpleStringCOPs & end_ids) {

    for(auto const & end : ends) {
        auto end_id = generate_end_id(s, bps, ends, end);
        end_ids.push_back(std::make_shared<base::SimpleString>(end_id));
    }
}

StructureOP
SegmentFactory::_get_aligned_structure(
        Structure const & s) {

    auto best = 1000;
    auto ref_center = ref_motif_->get_end(0).get_center();
    auto chains = s.get_chains();
    auto reordered_chains = std::vector<Chain const *>();
    auto i = -1;
    auto pos = -1;
    for(auto const & c : *chains) {
        i++;
        auto first_center = c.get_first().get_center();
        auto dist = first_center.distance(ref_center);
        if(dist < best) {
            best = dist;
            pos = i;
        }
    }

    reordered_chains.push_back(&chains->at(pos));
    i = -1;
    for(auto const & c : *chains) {
        i++;
        if(i == pos) { continue; }
        reordered_chains.push_back(&c);
    }

    auto res = Residues();
    auto chain_cuts = Cutpoints();

    for(auto const & c : reordered_chains) {
        for (auto const & r : *c) {
            res.push_back(r);
        }
        chain_cuts.push_back(res.size());
    }

    return std::make_shared<Structure>(res, chain_cuts);


}

void
SegmentFactory::_get_aligned_ends(
        Structure const & s,
        Basepairs & ends) {

    auto best = 1000;
    auto ref_center = ref_motif_->get_end(0).get_center();
    auto i = -1;
    auto pos = -1;
    for(auto const & end : ends) {
        i++;
        auto dist =   ref_center.distance(end.get_center());
        if(dist < best) {
            best = dist;
            pos = i;
        }
    }

    if(pos != 0) { std::swap(ends[0], ends[pos]); }

    // res1 should always be the start of a chain not res2
    for(auto & end : ends) {
        auto & r = s.get_residue(end.get_res2_uuid());
        if(s.is_residue_start_of_chain(r)) { end.swap_residue_positions(); }
    }

}

void
SegmentFactory::_remove_ends_from_basepairs(
        Basepairs const & ends,
        Basepairs & bps) {

    for(auto const & e : ends) {
        for(auto i = 0; i < bps.size(); i++) {
            if(e == bps[i]) {
                bps.erase(std::remove(bps.begin(), bps.end(), bps[i]), bps.end());
                break;
            }
        }
    }
}

void
SegmentFactory::_remove_beads_from_end_res(
        Basepairs const & ends,
        Structure & s) {

    s.remove_residue_beads(ends[0].get_res1_uuid());
    s.remove_residue_beads(ends[0].get_res2_uuid());

}

}
