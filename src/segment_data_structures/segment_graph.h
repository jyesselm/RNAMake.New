//
// Created by Joseph Yesselman on 1/14/18.
//

#ifndef RNAMAKE_NEW_SEGMENT_GRAPH_H
#define RNAMAKE_NEW_SEGMENT_GRAPH_H

#include <data_structures/graph.h>
#include <all_atom/aligner.h>

namespace segment_data_structures {

template <typename SegmentType, typename AlignerType>
class SegmentGraph {
public:
    SegmentGraph():
            aligner_(AlignerType()),
            graph_(data_structures::FixedEdgeDirectedGraph<SegmentType>()){}

public:

    typedef typename data_structures::FixedEdgeDirectedGraph<SegmentType>::const_iterator const_iterator;
    typedef typename data_structures::FixedEdgeDirectedGraph<SegmentType>::iterator iterator;

    iterator begin(){ return graph_.begin(); }
    iterator end()  { return graph_.end(); }

    const_iterator begin() const noexcept { return graph_.begin(); }
    const_iterator end() const noexcept   { return graph_.end(); }

public:


public:
    inline
    size_t
    get_num_segments() const {
        return graph_.get_num_nodes();
    }

    inline
    SegmentType const &
    get_segment(
            Index ni) const {
        return graph_.get_node_data(ni);
    }

    inline
    SegmentType &
    get_segment(
            Index ni) {
        return graph_.get_node_data(ni);
    }

public:
    inline
    bool
    has_parent(
            Index ni) const {
        return graph_.has_parent(ni);
    }

    inline
    Index
    get_parent_index(
            Index ni) const {
        return graph_.get_parent_index(ni);
    }

    inline
    Index
    get_parent_end_index(
            Index ni) const {
        return graph_.get_parent_end_index(ni);
    }


public:
    Index
    add_segment(
            SegmentType const & seg) {
        auto ni = graph_.add_node(seg, seg.get_num_ends());
        _update_default_transveral();
        return ni;
    }

    Index
    add_segment(
            SegmentType const & seg,
            Index parent_index,
            String const & parent_end_name) {
        auto seg_copy = seg;
        auto & parent = graph_.get_node_data(parent_index);
        auto parent_end_index = parent.get_end_index(parent_end_name);
        aligner_.align(parent.get_end(parent_end_index), seg_copy);
        auto ni = graph_.add_node(seg_copy, seg.get_num_ends(), 0,
                                  data_structures::NodeIndexandEdge{parent_index, parent_end_index});
        _update_default_transveral();
        return ni;
    }

public:
    void
    remove_segment(
            Index ni) {
        graph_.remove_node(ni);
        auto roots = graph_.get_root_indexes();
        if(roots.size() > 0) {
            graph_.setup_transversal(roots[0]);
        }
    }


public:
    String
    get_segment_end_name(
            Index n_index,
            Index end_index) const {
        return graph_.get_node_data(n_index).get_end(end_index).get_name_str();
    }

public:
    void
    write_nodes_to_pdbs(
            String const & name) const {
        for(auto const & n : graph_) {
            n->data().write_pdb(name+"."+std::to_string(n->index())+".pdb");
        }
    }

protected:
    void
    _update_default_transveral() {
        auto roots = graph_.get_root_indexes();
        if(roots.size() > 0) {
            graph_.setup_transversal(roots[0]);
        }
    }


protected:
    data_structures::FixedEdgeDirectedGraph<SegmentType> graph_;
    AlignerType aligner_;
};

}


namespace all_atom {

typedef segment_data_structures::SegmentGraph<Segment, Aligner> SegmentGraph;

}

#endif //RNAMAKE_NEW_SEGMENT_GRAPH_H
