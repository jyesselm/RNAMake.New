//
// Created by Joseph Yesselman on 10/31/17.
//

#ifndef RNAMAKE_NEW_RNA_STRUCTURE_TYPE_GRAPH_H
#define RNAMAKE_NEW_RNA_STRUCTURE_TYPE_GRAPH_H

#include <data_structures/directed_graph.h>
#include <base/simple_string.h>

namespace data_structures {

template<typename RNASegmentType, typename AlignerType>
class RNASegmentTypeGraph {
public:
    typedef std::shared_ptr<RNASegmentType> RNASegmentTypeOP;

public:
    RNASegmentTypeGraph():
            dg_(DirectedGraph<RNASegmentType>()){}

public: //iterators


public:
    Index
    add_rna_segment(
            RNASegmentTypeOP rs) {
        auto rs_copy = std::make_shared<RNASegmentType>(*rs);
        return dg_.add_node(rs_copy, rs_copy->get_num_ends());
    }

    Index
    add_rna_segment(
            RNASegmentTypeOP rs,
            Index parent_index,
            base::SimpleStringCOP parent_end_name) {
        auto parent = dg_.get_node(parent_index);
        auto parent_end_index = parent->get_end_index(parent_end_name);

        expects<GraphException>(
                parent_end_index != parent->block_end(),
                "cannot add rna_segment to parent's blocked end");

        auto rs_copy = std::make_shared<RNASegmentType>(*rs);
        aligner_.align(parent->get_end(parent_end_index), rs_copy);
        return dg_.add_node(rs_copy, rs_copy->get_num_ends(), parent_index, parent_end_index, 0);
    }

public:
    RNASegmentType const *
    get_rna_segment(
            Index n_index) const {
        return dg_.get_node(n_index);
    }

public:
    base::SimpleStringCOP
    get_segment_end_name(
            Index n_index,
            Index end_index) {
        return dg_.get_node(n_index)->get_end(end_index).get_name();
    }

private:
    bool
    _in_graph_already(
            RNASegmentTypeOP rs) {
        //for(auto const & n : )
        return true;
    }

private:
    DirectedGraph<RNASegmentType> dg_;
    AlignerType aligner_;
};

}


#endif //RNAMAKE_NEW_RNA_STRUCTURE_TYPE_GRAPH_H
