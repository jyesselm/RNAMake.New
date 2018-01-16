//
// Created by Joseph Yesselman on 1/14/18.
//

#ifndef RNAMAKE_NEW_NEW_GRAPH_H
#define RNAMAKE_NEW_NEW_GRAPH_H

#include <base/types.h>
#include <base/assertions.h>
#include <base/vector_container.h>
#include <data_structures/graph_base.h>
#include <data_structures/graph_adjacency_list.h>

namespace data_structures {

template<typename DataType, typename AdjacencyListType>
class IterList {
public:
    IterList() {}

public:
    void
    update(
            AdjacencyListType const &) {}

private:
    std::vector<Node<DataType> const *> iter_list_;
};


template<typename DataType, typename AdjacencyListType, typename IterListType>
class NewGraphBase {
public:
    NewGraphBase() :
            adjacency_list_(AdjacencyListType()) {}

    ~NewGraphBase() {}

public:
    inline
    size_t
    get_num_nodes() const { return adjacency_list_.get_num_nodes(); }

    inline
    size_t
    get_num_edges() const { return adjacency_list_.get_num_edges(); }

    inline
    std::vector<Edge const *> const &
    get_nodes_edges(
            Index ni) const {  return adjacency_list_.get_nodes_edges(); }

public:
    inline
    Index
    add_node(
            DataType const & d,
            Size n_edges) {
        return adjacency_list_.add_node(d, n_edges);
    }


public:
    AdjacencyListType adjacency_list_;
    IterListType iter_list_;

};


template<typename DataType>
using _FixedEdgeDirectedGraph = NewGraphBase<
        DataType,
        DirectedAdjacencyList<DataType, FixedEdges>,
        IterList<DataType, DirectedAdjacencyList<DataType, FixedEdges>>>;

template<typename DataType>
class FixedEdgeDirectedGraph : public _FixedEdgeDirectedGraph<DataType> {
public:
    typedef _FixedEdgeDirectedGraph<DataType> BaseClass;

public:
    FixedEdgeDirectedGraph():
            BaseClass() {}

public:
    inline
    Index
    add_node(
            DataType const & d,
            Size n_edges,
            Index n_end_index,
            NodeIndexandEdge const pie) {
        return this->adjacency_list_.add_node(d, n_edges, n_end_index, pie);
    }

    using BaseClass::add_node;
};

}

#endif //RNAMAKE_NEW_NEW_GRAPH_H
