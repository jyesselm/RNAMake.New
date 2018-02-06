//
// Created by Joseph Yesselman on 1/14/18.
//

#ifndef RNAMAKE_NEW_SEGMENT_GRAPH_H
#define RNAMAKE_NEW_SEGMENT_GRAPH_H

#include <data_structures/graph.h>
#include <all_atom/aligner.h>
#include <resource_management/resource_manager.h>

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

    inline
    std::vector<data_structures::Edge const *> const &
    get_segment_connections(
            Index ni) const {
        return graph_.get_node_edges(ni);
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

template <typename SegmentType, typename AlignerType>
using SegmentGraphOP = std::shared_ptr<SegmentGraph<SegmentType, AlignerType>>;

struct NodeIndexandEdgeCompare {
    bool operator () (
            data_structures::NodeIndexandEdge const & nie1,
            data_structures::NodeIndexandEdge const & nie2) const {
        return nie1.node_index > nie2.node_index;
    }
};

template <typename SegmentType, typename AlignerType>
SegmentGraphOP<SegmentType, AlignerType>
convert_ideal_helices_to_basepair_steps(
        SegmentGraph<SegmentType, AlignerType> const & g,
        resource_management::ResourceManager const & rm) {
    typedef data_structures::NodeIndexandEdge NodeIndexandEdge;
    auto new_g = std::make_shared<SegmentGraph<SegmentType, AlignerType>>();
    auto index_convert = std::map<NodeIndexandEdge, NodeIndexandEdge, NodeIndexandEdgeCompare>();
    auto args = StringStringMap{{"name", "HELIX.IDEAL"}};
    auto aligner = AlignerType();
    for(auto const & n : g) {
        if(n->data().get_segment_type() != util::SegmentType::HELIX) {
            continue;
        }
        else if(n->data().get_name_str().substr(0, 5) != "HELIX") {
            continue;
        }

        auto num_res = n->data().get_num_residues();
        auto num_bp_steps = num_res / 2;

        auto start = rm.get_segment(args);
        auto pos = -1;

        if(g.has_parent(n->index())) {

        }
        else { // no parent but make sure its starting from the correct orientation
            aligner.align(n->data().get_end(0), *start);
            pos = new_g->add_segment(*start);
        }

        for(int i = 0; i < num_bp_steps-2; i++) {
            auto ideal_bp_step = rm.get_segment(args);
            pos = new_g->add_segment(*ideal_bp_step, pos, start->get_end_name(1));
        }

        auto org_nie = NodeIndexandEdge{n->index(), 1};
        auto new_nie = NodeIndexandEdge{pos, 1};

        index_convert[org_nie] = new_nie;

        //std::cout << n->index() << " " << n->data().get_name_str() << std::endl;
    }

    new_g->write_nodes_to_pdbs("new");

    return new_g;
}

}


namespace all_atom {

typedef segment_data_structures::SegmentGraph<Segment, Aligner> SegmentGraph;

}

#endif //RNAMAKE_NEW_SEGMENT_GRAPH_H
