//
// Created by Joseph Yesselman on 10/31/17.
//

#ifndef RNAMAKE_NEW_DIRECTED_GRAPH_H
#define RNAMAKE_NEW_DIRECTED_GRAPH_H

#include <data_structures/graph.h>

namespace data_structures {

template<typename DataType>
class DirectedGraph : public FixedNumEdgesGraph<DataType> {
public:
    typedef FixedNumEdgesGraph<DataType> ParentClass;
public:
    typedef std::map<Index, Index> IndexMap;

public:
    DirectedGraph() :
            ParentClass(),
            parent_(IndexMap()) {}

public:
    inline
    Index
    add_node(
            DataType const & d,
            Size n_edges) {

        return ParentClass::add_node(d, n_edges); //Not sure why I have to do this ?
    }

    inline
    Index
    add_node(
            DataType const & d,
            Size n_edges,
            Index parent_index,
            Index parent_end_index,
            Index n_end_index) {
        auto n_index = ParentClass::add_node(d, n_edges, parent_index, parent_end_index, n_end_index);
        parent_[n_index] = parent_index;
        return n_index;
    }

public:
    Index
    get_parent_index(
            Index n_index) {
        expects<GraphException>(
                parent_.find(n_index) != parent_.end(),
                "node does not have parent cannot get parent index");
        return parent_[n_index];

    }


private:
    IndexMap parent_;

};


}

#endif //RNAMAKE_NEW_DIRECTED_GRAPH_H
