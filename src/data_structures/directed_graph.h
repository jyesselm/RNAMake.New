//
// Created by Joseph Yesselman on 10/31/17.
//

#ifndef RNAMAKE_NEW_DIRECTED_GRAPH_H
#define RNAMAKE_NEW_DIRECTED_GRAPH_H

#include <base/vector_container.h>
#include <data_structures/graph_old.h>

namespace data_structures {

template<typename DataType>
class DirectedGraph : public FixedNumEdgesGraph<DataType> {
public:
    typedef FixedNumEdgesGraph<DataType> ParentClass;
public:
    typedef std::map<Index, Index>       IndexMap;
    typedef std::shared_ptr<DataType>    DataTypeOP;
    typedef std::vector<Edge const *>    Edges;

public:
    DirectedGraph() :
            ParentClass(),
            parent_(IndexMap()) {}

    DirectedGraph(
            DirectedGraph const & dg) {
        for(auto const & n : dg) {
            auto n_data = std::make_shared<DataType>(*n.data);
            if(!has_parent(n.index)) {
                _add_node_with_index(n_data, (Index)dg.get_num_edges(n.index), n.index);
            }
            else {
                auto parent_index = dg.get_parent_index(n.index);
                auto parent_end_index = dg.get_parent_edge_index(n.index);
                auto end_index_to_parent = dg.get_edge_index_to_parent(n.index);
                _add_node_with_index(n_data, (Index)dg.get_num_edges(n.index), parent_index,
                                     parent_end_index, end_index_to_parent, n.index);
            }
        }
        for(auto const & e : *dg.get_all_edges()) {
            if(this->edge_exist(e->node_i, e->node_j, e->edge_i, e->edge_j)) { continue; }
            this->add_edge(e->node_i, e->node_j, e->edge_i, e->edge_j);
        }
    }

public:
    inline
    Index
    add_node(
            DataTypeOP d,
            Size n_edges) {

        return ParentClass::add_node(d, n_edges); //Not sure why I have to do this ?
    }

    inline
    Index
    add_node(
            DataTypeOP d,
            Size n_edges,
            Index parent_index,
            Index parent_end_index,
            Index n_end_index) {
        auto n_index = ParentClass::add_node(d, n_edges, parent_index, parent_end_index, n_end_index);
        parent_[n_index] = parent_index;
        return n_index;
    }

public:

    inline
    void
    set_node(
            DataTypeOP d,
            Index n_index) {
        expects<GraphException>(
                this->nodes_.find(n_index) != this->nodes_.end(),
                "a node with this index must already exist");

        this->nodes_[n_index] = std::make_shared<DataType>(*d);
        for(auto & n : this->iter_list_) {
            if(n.index == n_index) {
                n.data = this->nodes_.find(n_index)->second.get();
            }
        }
    }

public:
    base::VectorContainerOP<int>
    get_roots() {
        auto roots = Ints();
        for(auto const & kv : this->nodes_) {
            if(parent_.find(kv.first) == parent_.end()) {
                roots.push_back(kv.first);
            }
        }
        return std::make_shared<base::VectorContainer<int>>(roots);
    }

    Index
    get_parent_index(
            Index n_index) const {
        expects<GraphException>(
                parent_.find(n_index) != parent_.end(),
                "node does not have parent cannot get parent index");
        return parent_.find(n_index)->second;

    }

    bool
    has_parent(
            Index n_index) const {
        if(parent_.find(n_index) == parent_.end()) { return false; }
        else                                       { return true;  }
    }

    Index
    get_parent_edge_index(
            Index ni) const {
        auto pi = get_parent_index(ni);
        auto & parent_edges = this->get_edges(pi);
        int i = -1;
        for(auto const & e : parent_edges) {
            i++;
            if(e == nullptr) { continue; }
            if(e->partner(pi) == ni) { return i;}
        }
        throw GraphException("something went entirely wrong here");
    }

    base::VectorContainerOP<Edge const *>
    get_all_edges() const {
        auto all_edges = Edges();

        for(auto const & kv : this->edges_) {
            for (auto const & e : kv.second) {
                if (e == nullptr) { continue; }
                if (std::find(all_edges.begin(), all_edges.end(), e) != all_edges.end()) {
                    continue;
                }
                all_edges.push_back(e);
            }
        }

        return std::make_shared<base::VectorContainer<Edge const *>>(all_edges);
    }

    bool
    edge_exist(
            Index ni,
            Index nj,
            Index ei,
            Index ej) const {
        for(auto const & edge : this->get_edges(ni)) {
            if(edge == nullptr) { continue; }
            if(edge->partner(ni) != nj) { continue; }
            if(edge->end_index(ni) != ei) { continue; }
            if(edge->end_index(nj) != ej) { continue; }
            return true;
        }
        return false;
    }

    Index
    get_edge_index_to_parent(
            Index ni) const {
        auto pi = get_parent_index(ni);
        for(auto const & edge : this->get_edges(ni)) {
            if(edge == nullptr) { continue; }
            if(edge->partner(ni) == pi) { return edge->end_index(ni); }
        }
        //this should never happen
        throw GraphException("could not find edge to parent index");
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

        if(this->iter_list_.size() != this->nodes_.size()) {
            this->iter_list_.resize(this->nodes_.size());
        }

        auto open = std::queue<Index>();
        auto seen = std::map<Index, int>();

        auto roots = get_roots();
        auto pos = 0;
        for(auto const & root : *roots) {
            open.push(root);
            seen[root] = 1;

            while(open.size() > 0) {
                auto current = open.front();
                open.pop();
                for(auto const & e : this->get_edges(current)) {
                    if(e == nullptr) { continue; }
                    auto partner = e->partner(current);
                    if(has_parent(partner) && get_parent_index(partner) == current) {
                        if(seen.find(partner) != seen.end()) { continue; }
                        seen[partner] = 1;
                        open.push(partner);
                    }
                }
                this->iter_list_[pos].index = current;
                this->iter_list_[pos].data = this->nodes_[current].get();
                pos += 1;

            }

        }
    }

private:
    // allow exact copying from another graph
    Index
    _add_node_with_index(
            DataTypeOP d,
            Size n_edges,
            Index n_index) {

        this->nodes_[n_index] = d;
        this->edges_[n_index] = Edges(n_edges);
        this->index_ = n_index + 1;
        _rebuild_iter_list();
        return n_index;
    }

    Index
    _add_node_with_index(
            DataTypeOP d,
            Size n_edges,
            Index parent_index,
            Index parent_end_index,
            Index n_end_index,
            Index n_index) {

        _add_node_with_index(d, n_edges, n_index);
        this->add_edge(parent_index, n_index, parent_end_index, n_end_index);
        parent_[n_index] = parent_index;

        return n_index;
    }

private:
    IndexMap parent_;

};


}

#endif //RNAMAKE_NEW_DIRECTED_GRAPH_H
