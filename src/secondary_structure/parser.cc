//
// Created by Joseph Yesselman on 10/27/17.
//

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
    // make sure things are reset
    basepairs_ = Basepairs();
    tc_basepairs_ = Basepairs();

    auto s = get_structure_from_secondary_structure(sequence, dot_bracket);
    parse_structure_to_chain_graph(*s);

    //return std::make_shared<Segment>(*p, util::SegmentType::SEGMENT, 0);
    return _generate_segment(*s);

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


void
Parser::_get_basepairs_in_structure(
        Structure const & s,
        Basepairs & current_basepairs) {

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
}

PoseOP
Parser::_generate_pose(
        Structure const & s) {
    auto current_basepairs = Basepairs();
    _get_basepairs_in_structure(s, current_basepairs);

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


SegmentOP
Parser::_generate_segment(
        Structure const & s) {
    auto current_basepairs = Basepairs();
    _get_basepairs_in_structure(s, current_basepairs);

    auto end_indexes = get_end_indexes_from_basepairs(s, current_basepairs)->get_data();

    if(end_indexes.size() == 0) {
        throw ParserException("segments must include at least one end!");
    }

    auto end_ids = base::SimpleStringCOPs();
    for(auto const & ei : end_indexes) {
        auto end_id = generate_end_id(s, current_basepairs, current_basepairs[ei]);
        end_ids.push_back(std::make_shared<base::SimpleString const>(end_id));
    }
    auto name = std::make_shared<base::SimpleString const>("from_secondary_structure");
    auto seg = std::make_shared<Segment>(s, current_basepairs, end_indexes, end_ids, name,
                                         util::SegmentType::SEGMENT, 0, util::Uuid());
    return seg;
}


};
