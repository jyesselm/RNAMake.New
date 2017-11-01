//
// Created by Joseph Yesselman on 10/27/17.
//

#ifndef RNAMAKE_NEW_PARSER_H
#define RNAMAKE_NEW_PARSER_H

#include <data_structures/graph.h>
#include <secondary_structure/residue.h>
#include <secondary_structure/rna_structure.h>

namespace secondary_structure {

enum ChainStructureType { UNPAIRED, PAIRED, TERTIARY };
enum EdgeConnectionType { PARENT, CHILD, PAIR };

struct ChainGraphNode {
    std::vector<Residue const *> residues;
    ChainStructureType type;

};


class ChainGraph {
public:
    ChainGraph():
            graph_(data_structures::FixedNumEdgesGraph<ChainGraphNode>()) {}

    ~ChainGraph() {}

public:

    typedef typename data_structures::FixedNumEdgesGraph<ChainGraphNode>::const_iterator const_iterator;

    const_iterator begin() const noexcept { return graph_.begin(); }
    const_iterator end() const noexcept   { return graph_.end();   }

public:
    Index
    add_chain(
            std::vector<Residue const *> const & residues,
            ChainStructureType type) {

        auto node = ChainGraphNode{ residues, type};
        // always three connections points
        // 0: parent connection, 1: child connection, 2:
        return graph_.add_node(node, 3);

    }

    Index
    add_chain(
            std::vector<Residue const *> const & residues,
            ChainStructureType type,
            Index parent_index) {
        auto node = ChainGraphNode{ residues, type};
        return graph_.add_node(node, 3, parent_index,
                               EdgeConnectionType::CHILD, EdgeConnectionType::PARENT);

    }

    Index
    get_index_of_residue(
            Residue const & residue) {
        for(auto const & n : graph_) {
            for(auto const & r : n.data->residues) {
                if(residue == *r) { return n.index; }
            }
        }
        return -1;
    }

    void
    pair_nodes(
            Index n_i,
            Index n_j) {
        expects<data_structures::GraphException>(
                (graph_.get_node(n_i).type == ChainStructureType::PAIRED &&
                 graph_.get_node(n_j).type == ChainStructureType::PAIRED) ||
                (graph_.get_node(n_i).type == ChainStructureType::TERTIARY &&
                 graph_.get_node(n_j).type == ChainStructureType::TERTIARY),
                "must be same type and cannot be unpaired");
        graph_.add_edge(n_i, n_j, EdgeConnectionType::PAIR, EdgeConnectionType::PAIR);
    }


private:
    data_structures::FixedNumEdgesGraph<ChainGraphNode> graph_;

};


class Parser {
public:
    Parser():
            chain_graph_(ChainGraph()),
            basepairs_(Basepairs()),
            tc_basepairs_(Basepairs()) {}

    ~Parser() {}

public:
    RNAStructureOP
    parse_to_rna_structure(
            String const &,
            String const &);

private:
    void
    parse_structure_to_chain_graph(
            StructureOP);


    Index
    _add_unpaired_residues_to_graph(
            std::vector<Residue const *> const & res,
            Index parent_node_index,
            bool start_of_chain) {
        if(start_of_chain) {
            return chain_graph_.add_chain(res, ChainStructureType::UNPAIRED);
        }
        else               {
            //auto & partner_r = s->get_residue(bp.get_res1_uuid());
            //auto parent_pos =
            return chain_graph_.add_chain(res, ChainStructureType::UNPAIRED, parent_node_index);
        }
    }

    Index
    _add_paired_res_to_graph(
            Residue const * r,
            Index parent_node_index,
            Index residue_index,
            StructureOP s,
            bool start_of_chain) {
        auto partner_index = _get_parentheses_pair(residue_index, s);
        auto & partner = s->get_residue(partner_index);
        auto name = primitives::generate_bp_name<Residue>(*r, partner);

        // create new basepair from pair
        auto basepair = Basepair(r->get_uuid(), partner.get_uuid(), util::Uuid(),
                                 primitives::BasepairType::WC,
                                 std::make_shared<base::SimpleString>(name));
        basepairs_.push_back(basepair);

        if(start_of_chain) {
            return chain_graph_.add_chain(std::vector<Residue const *>{r}, ChainStructureType::PAIRED);
        }
        else {
            return chain_graph_.add_chain(std::vector<Residue const *>{r}, ChainStructureType::PAIRED,
                                          parent_node_index);
        }
    }

    int
    _get_parentheses_pair(
            Index start_pos,
            StructureOP s) {

        auto current_pos = -1;
        auto start = false;
        auto parentheses_count = 0;
        for(auto const & r : *s) {
            current_pos++;
            if(current_pos == start_pos) { start = true; }
            if(!start) { continue; }

            if     (r.get_dot_bracket() == '(') {
                parentheses_count += 1;
            }
            else if(r.get_dot_bracket() == ')') {
                parentheses_count -= 1;
                if (parentheses_count == 0) { return current_pos; }
            }
        }

        throw RNAStructureException("cannot find pair to (");

    }

    Basepair const &
    _get_existing_basepair(
            Residue const & r) {
        for(auto const & bp : basepairs_) {
            if(bp.get_res2_uuid() == r.get_uuid()) { return bp; }
        }
        for(auto const & bp: tc_basepairs_) {
            if(bp.get_res2_uuid() == r.get_uuid()) { return bp; }
        }
    }

    RNAStructureOP
    _generate_rna_structure(
            Structure const & s);

private:
    ChainGraph chain_graph_;
    Basepairs basepairs_;
    Basepairs tc_basepairs_;

};

}


#endif //RNAMAKE_NEW_PARSER_H
