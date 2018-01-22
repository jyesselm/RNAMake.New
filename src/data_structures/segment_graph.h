//
// Created by Joseph Yesselman on 10/31/17.
//

#ifndef RNAMAKE_NEW_RNA_STRUCTURE_TYPE_GRAPH_H
#define RNAMAKE_NEW_RNA_STRUCTURE_TYPE_GRAPH_H

#include <data_structures/directed_graph.h>
#include <base/simple_string.h>

/*
namespace data_structures {

template<typename SegmentType, typename AlignerType>
class SegmentGraph {
public:
    typedef std::shared_ptr<SegmentType> SegmentTypeOP;

public:
    SegmentGraph():
            dg_(DirectedGraph<SegmentType>()){}

public: //iterators
    typedef typename DirectedGraph<SegmentType>::const_iterator const_iterator;

    const_iterator begin() const noexcept { return dg_.begin(); }
    const_iterator end() const noexcept   { return dg_.end(); }


public:
    Index
    add_segment(
            SegmentTypeOP rs) {
        auto rs_copy = std::make_shared<SegmentType>(*rs);
        return dg_.add_node(rs_copy, rs_copy->get_num_ends());
    }

    Index
    add_segment(
            SegmentTypeOP rs,
            Index parent_index,
            base::SimpleStringCOP parent_end_name) {
        auto parent = dg_.get_node(parent_index);
        auto parent_end_index = parent->get_end_index(parent_end_name);

        expects<GraphException>(
                parent_end_index != parent->get_aligned_end_index(),
                "cannot add rna_segment to parent's blocked end");

        auto rs_copy = std::make_shared<SegmentType>(*rs);
        aligner_.align(parent->get_end(parent_end_index), *rs_copy);
        return dg_.add_node(rs_copy, rs_copy->get_num_ends(), parent_index, parent_end_index, 0);
    }

public:
    SegmentType const *
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
            SegmentTypeOP rs) {
        //for(auto const & n : )
        return true;
    }

private:
    DirectedGraph<SegmentType> dg_;
    AlignerType aligner_;
};

}
*/

#endif //RNAMAKE_NEW_RNA_STRUCTURE_TYPE_GRAPH_H
