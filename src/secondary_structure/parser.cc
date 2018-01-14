//
// Created by Joseph Yesselman on 10/27/17.
//

#include <base/unique_ptr.h>
#include <secondary_structure/parser.h>

namespace secondary_structure {

PoseOP
Parser::parse_to_pose(
        String const & sequence,
        String const & dot_bracket) {

    // make sure things are reset
    basepairs_ = Basepairs();
    tc_basepairs_ = Basepairs();

    auto s = get_structure_from_secondary_structure(sequence, dot_bracket);
    parse_structure_to_chain_graph(*s);

    return _generate_pose(*s);
}

SegmentOP
Parser::parse_to_segment(
        String const & sequence,
        String const & dot_bracket) {
    auto p = parse_to_pose(sequence, dot_bracket);
    return std::make_shared<Segment>(*p, util::SegmentType::SEGMENT, 0);

}

void
Parser::parse_structure_to_chain_graph(
        Structure const & s) {

    auto current_residues = std::vector<Residue const *>();
    auto residue_pos = -1;
    auto last_index = -1;
    for(auto const & r: s) {
        residue_pos++;
        auto start_of_chain = s.is_residue_start_of_chain(r);
        if     (r.get_dot_bracket() == '.') {
            current_residues.push_back(&r);
        }
        else if(current_residues.size() > 0)  { // done with unpaired region make sure to add it to graph
            last_index = _add_unpaired_residues_to_graph(current_residues, last_index, start_of_chain);
            current_residues = std::vector<Residue const *>();
        }

        if(r.get_dot_bracket() == '(') {
            last_index = _add_paired_res_to_graph(&r, last_index, residue_pos, s, start_of_chain);
        }
        else if(r.get_dot_bracket() == ')') {
            if(start_of_chain) {
                last_index = chain_graph_.add_chain(std::vector<Residue const *>{&r},
                                                    ChainStructureType::PAIRED,
                                                    last_index);
            }
            else {
                last_index = chain_graph_.add_chain(std::vector<Residue const *>{&r},
                                                    ChainStructureType::PAIRED);
            }
            auto & bp = _get_existing_basepair(r);
            auto & partner_r = s.get_residue(bp.get_res1_uuid());
            auto partner_node_index = chain_graph_.get_index_of_residue(partner_r);
            chain_graph_.pair_nodes(partner_node_index, last_index);
        }
    }


}

PoseOP
Parser::_generate_pose(
        Structure const & s) {
    auto current_basepairs = Basepairs();
    for(auto const & bp : basepairs_) {
        // do both residues in basepair exist in structure
        try {
            s.get_residue(bp.get_res1_uuid());
            s.get_residue(bp.get_res2_uuid());
        }
        catch(StructureException) { continue; }
        current_basepairs.push_back(bp);
    }

    for(auto const & bp : tc_basepairs_) {
        // do both residues in basepair exist in structure
        try {
            s.get_residue(bp.get_res1_uuid());
            s.get_residue(bp.get_res2_uuid());
        }
        catch(StructureException) { continue; }
        current_basepairs.push_back(bp);
    }
    auto end_indexes = get_end_indexes_from_basepairs(s, current_basepairs)->get_data();

    auto end_ids = base::SimpleStringCOPs();
    for(auto const & ei : end_indexes) {
        auto end_id = generate_end_id(s, current_basepairs, current_basepairs[ei]);
        end_ids.push_back(std::make_shared<base::SimpleString const>(end_id));
    }
    auto name = std::make_shared<base::SimpleString const>("from_secondary_structure");
    auto p = std::make_shared<Pose>(s, current_basepairs, end_indexes, end_ids, name);
    return p;

}



};
