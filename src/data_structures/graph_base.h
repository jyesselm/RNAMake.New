//
// Created by Joseph Yesselman on 1/15/18.
//

#ifndef RNAMAKE_NEW_GRAPH_BASE_H
#define RNAMAKE_NEW_GRAPH_BASE_H

#include <base/types.h>
#include <base/assertions.h>

namespace data_structures {

/*
 * Exception for graph
 */
class GraphException : public std::runtime_error {
public:
    /**
     * Standard constructor for ChainException
     * @param   message   Error message for chain
     */
    GraphException(String const & message) :
            std::runtime_error(message) {}
};

template<typename DataType>
struct Node {
    DataType data;
    Index index;
};

struct NodeIndexandEdge {
    Index node_index;
    Index edge_index;

    String
    to_str() const {
        return "node_index: " + std::to_string(node_index) + " edge_index: " + std::to_string(edge_index);
    }
};

struct Edge {
    Index node_i, node_j, edge_i, edge_j;

    inline
    Edge(
            Index nnode_i,
            Index nnode_j,
            Index nedge_i,
            Index nedge_j) :
            node_i(nnode_i),
            node_j(nnode_j),
            edge_i(nedge_i),
            edge_j(nedge_j) {}

    inline
    bool
    operator==(Edge & other) const {
        return node_i == other.node_i && node_j == other.node_j &&
               edge_i == other.edge_i && edge_j == other.edge_j;
    }

    Index
    partner(
            Index index) const {
        expects<GraphException>(
                index == node_i || index == node_j,
                "cannot call partner if not a node in this edge");

        if (index == node_i) { return node_j; }
        else { return node_i; }

    }

    Index
    end_index(
            Index index) const {
        expects<GraphException>(
                index == node_i || index == node_j,
                "cannot call end_index if not a node in this edge");

        if (index == node_i) { return edge_i; }
        else { return edge_j; }

    }

};

// Edge Types
struct FixedEdges {};
struct DynamicEdges{};

}

#endif //RNAMAKE_NEW_GRAPH_BASE_H
