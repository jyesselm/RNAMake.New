//
// Created by Joseph Yesselman on 10/21/17.
//

#ifndef RNAMAKE_NEW_GRAPH_H
#define RNAMAKE_NEW_GRAPH_H

#include <iostream>
#include <queue>

#include <boost/iterator/indirect_iterator.hpp>

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

struct Edge {
    Index node_i, node_j, edge_i, edge_j;

    inline
    Edge(
            Index nnode_i,
            Index nnode_j,
            Index nedge_i,
            Index nedge_j):
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

        if   (index == node_i) { return node_j; }
        else                   { return node_i; }

    }
};

template<typename DataType>
class BaseGraph {
public:
    typedef std::shared_ptr<DataType>       DataTypeOP;
    typedef std::shared_ptr<DataType const> DataTypeCOP;
    typedef std::vector<DataTypeOP>         DataTypeOPs;
    typedef std::vector<DataTypeCOP>        DataTypeCOPs;

    typedef std::map<Index, DataTypeOP>     DataTypeOPMap;
    typedef std::shared_ptr<const Edge>     EdgeCOP;
    typedef std::vector<EdgeCOP>               EdgeCOPs;
    typedef std::map<Index, EdgeCOPs>       EdgeCOPMap;

public:
    inline
    BaseGraph():
            nodes_(DataTypeOPMap()),
            edges_(EdgeCOPMap()),
            index_(0),
            iter_list_(DataTypeCOPs()),
            rebuild_list_(0),
            transverse_start_(-1) {}

    ~BaseGraph() {}

public:

    typedef boost::indirect_iterator< typename DataTypeCOPs::const_iterator, DataType const > const_iterator;

    const_iterator begin() const noexcept { return iter_list_.begin(); }
    const_iterator end() const noexcept   { return iter_list_.end(); }

public:

    void
    change_transverse_start(
            Index start) {

        expects<GraphException>(
                nodes_.find(start) != nodes_.end(),
                "cannot find node of index: " + std::to_string(start));
        transverse_start_ = start;
        _rebuild_iter_list(transverse_start_);
    }

public: //getters
    size_t
    get_num_nodes() { return nodes_.size(); }

    EdgeCOPs const &
    get_edges(
            Index index) {
        expects<GraphException>(
                edges_.find(index) != edges_.end(),
                "this node has no edges : " + std::to_string(index));
        return edges_[index];
    }

    size_t
    get_num_edges(
            Index index) {
        if(edges_.find(index) == edges_.end()) { return 0; }
        return edges_[index].size();
    }

    DataTypeCOP
    get_node(
            Index index) {
        expects<GraphException>(
                nodes_.find(index) != nodes_.end(),
                "cannot find node of index: " + std::to_string(index));
        return nodes_[index];
    }

    bool
    are_nodes_connected(
            Index n1,
            Index n2) {
        expects<GraphException>(
                edges_.find(n1) != edges_.end(),
                "cannot find node of index: " + std::to_string(n1));

        for(auto const & edge : edges_[n1]) {
            if(edge->node_i == n2 or edge->node_j == n2) { return true; }
        }
        return false;
    }

private:
    virtual
    void
    _rebuild_iter_list(int) = 0;

protected:
    DataTypeOPMap nodes_;
    EdgeCOPMap edges_;
    DataTypeCOPs iter_list_;
    Flag rebuild_list_;
    Index index_, transverse_start_;

};

template<typename DataType>
class Graph : public BaseGraph<DataType> {
public:
    typedef BaseGraph<DataType> BaseClass;

public:
    typedef std::shared_ptr<DataType>       DataTypeOP;
    typedef std::vector<DataTypeOP>         DataTypeOPs;
    typedef std::shared_ptr<DataType const> DataTypeCOP;
    typedef std::vector<DataTypeCOP>        DataTypeCOPs;
    typedef std::shared_ptr<const Edge>     EdgeCOP;
    typedef std::vector<EdgeCOP>            EdgeCOPs;

public:
    inline
    Graph() : BaseClass() {}

public:
    Index
    add_node(
            DataTypeOP d) {

        this->nodes_[this->index_] = std::make_shared<DataType>(*d);
        this->index_ += 1;
        _rebuild_iter_list();
        return this->index_-1;

    }

    Index
    add_node(
            DataTypeOP d,
            Index parent_index) {

        // parent should exist
        expects<GraphException>(
                this->nodes_.find(parent_index) != this->nodes_.end(),
                "cannot add node to graph, parent with index: " + std::to_string(parent_index) +
                " does not exist");

        auto n_index = add_node(d);
        auto num_of_parent_edges = this->get_num_edges(parent_index);
        auto edge = std::make_shared<Edge const>(parent_index, n_index, num_of_parent_edges, 0);
        if(num_of_parent_edges == 0) {
            this->edges_[parent_index] = EdgeCOPs();
        }
        this->edges_[n_index] = EdgeCOPs();
        this->edges_[parent_index].push_back(edge);
        this->edges_[n_index].push_back(edge);
        _rebuild_iter_list();

        return n_index;

    }

private:
    void
    _rebuild_iter_list() {
        expects<GraphException>(
                this->get_num_nodes() > 0,
                "cannot rebuilt iter list without nodes, bad this will happen");

        if(this->transverse_start_ == -1) {
            this->transverse_start_ = 1000;
            for (auto const & kv : this->nodes_) {
                if (kv.first < this->transverse_start_) { this->transverse_start_ = kv.first; }
            }
        }

        return _rebuild_iter_list(this->transverse_start_);
    }

    void
    _rebuild_iter_list(
            Index start) {

        auto open = std::queue<Index>();
        auto seen = std::map<Index, int>();
        this->iter_list_ = DataTypeCOPs();

        auto get_neighbors = [&](
                Index index,
                std::map<Index, int> & cur_seen) {
            auto neighbors = std::vector<Index>();
            if(this->get_num_edges(index) == 0) { return neighbors; }
            auto edges = this->get_edges(index);
            for(auto const & e : edges) {
                if(cur_seen.find(e->partner(index)) == cur_seen.end()) {
                    neighbors.push_back(e->partner(index));
                    cur_seen[e->partner(index)] = 1;
                }
            }
            return neighbors;
        };

        open.push(start);
        seen[start] = 1;

        while(open.size() > 0) {
            auto current = open.front();
            open.pop();
            this->iter_list_.push_back(this->get_node(current));
            auto neighbors = get_neighbors(current, seen);
            for(auto const & n : neighbors) { open.push(n); }
        }

        // ensure you find all nodes not connected or in another sub graph
        for(auto const & kv : this->nodes_) {
            if(seen.find(kv.first)  != seen.end()) { continue; }
            auto new_start = kv.first;
            open.push(new_start);
            seen[new_start] = 1;

            while(open.size() > 0) {
                auto current = open.front();
                open.pop();
                this->iter_list_.push_back(this->get_node(current));
                auto neighbors = get_neighbors(current, seen);
                for(auto const & n : neighbors) { open.push(n); }
            }
        }



    }

};


}

#endif //RNAMAKE_NEW_GRAPH_H
