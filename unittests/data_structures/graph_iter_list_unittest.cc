//
// Created by Joseph Yesselman on 10/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <primitives/residue.h>
#include <data_structures/graph_iter_list.h>


TEST_CASE( "Test Graph Data Structure ", "[Graph]" ) {

    SECTION("test editing nodes") {
        auto adj_list = data_structures::FixedEdged_AL<int>();
        adj_list.add_node(0, 5);
        adj_list.add_node(1, 5);

        auto iter_list = data_structures::IterList<int, data_structures::FixedEdged_AL<int>>();
        iter_list.transversal(adj_list, 0);

        for(auto const & n : iter_list) {
            n->data() = 10;
        }

        for(auto & n : iter_list) {
            REQUIRE(n->data() == 10);
        }
    }

    SECTION("test with undirected adjacency list") {
        auto adj_list = data_structures::FixedEdged_AL<int>();
        adj_list.add_node(0, 5);
        adj_list.add_node(1, 5);
        adj_list.add_node(2, 5);
        adj_list.add_node(3, 5);
        adj_list.add_node(4, 5);
        adj_list.add_node(5, 5);

        adj_list.add_edge(data_structures::NodeIndexandEdge{0, 0}, data_structures::NodeIndexandEdge{1, 0});
        adj_list.add_edge(data_structures::NodeIndexandEdge{1, 1}, data_structures::NodeIndexandEdge{2, 0});
        adj_list.add_edge(data_structures::NodeIndexandEdge{2, 1}, data_structures::NodeIndexandEdge{3, 0});
        adj_list.add_edge(data_structures::NodeIndexandEdge{3, 1}, data_structures::NodeIndexandEdge{1, 2});
        adj_list.add_edge(data_structures::NodeIndexandEdge{3, 2}, data_structures::NodeIndexandEdge{4, 0});
        adj_list.add_edge(data_structures::NodeIndexandEdge{4, 1}, data_structures::NodeIndexandEdge{0, 2});

        /* adj_list connectivity
         *
         * 0 <-> 1 <-> 2 <-> 3 <-> 4   5
         * ^     ^           ^     ^
         * |     |           |     |
         * |     -------------     |
         * -------------------------
         */

        auto iter_list = data_structures::IterList<int, data_structures::FixedEdged_AL<int>>();
        iter_list.transversal(adj_list, 0);
        auto count = 0;

        for (auto const & n : iter_list) { count += 1; }
        REQUIRE(count == adj_list.get_num_nodes());

        iter_list.path_transversal(adj_list, 0, 2);
        auto path = std::vector<Index>();
        auto target = std::vector<Index>{0, 1, 2};
        for (auto const & n : iter_list) {
            path.push_back(n->index());
        }
        REQUIRE(path == target);

        // no viable path
        REQUIRE_THROWS_AS(iter_list.path_transversal(adj_list, 0, 5), data_structures::GraphException);

        iter_list.path_transversal(adj_list, 0, 3);
        path = std::vector<Index>();
        target = std::vector<Index>{0, 1, 3};
        for (auto const & n : iter_list) {
            path.push_back(n->index());
        }
        REQUIRE(path == target);
    }

    SECTION("test with directed adjacency list") {
        auto adj_list = data_structures::DirectedAdjacencyList<int, data_structures::FixedEdges>();
        adj_list.add_node(0, 5);
        adj_list.add_node(1, 5, 0, data_structures::NodeIndexandEdge{0, 0});
        adj_list.add_node(2, 5, 0, data_structures::NodeIndexandEdge{0, 1});
        adj_list.add_node(3, 5, 0, data_structures::NodeIndexandEdge{2, 1});
        adj_list.add_node(4, 5, 0, data_structures::NodeIndexandEdge{3, 1});
        adj_list.add_node(5, 5, 0, data_structures::NodeIndexandEdge{3, 2});
        adj_list.add_edge(data_structures::NodeIndexandEdge{5, 1}, data_structures::NodeIndexandEdge{1, 1});

        /* adj_list connectivity
         *
         *      5 -------
         *      ^       |
         *      |       |
         * 2 -> 3 -> 4  |
         * ^            |
         * |            |
         * 0 -> 1 -------
         *
         */

        auto iter_list = data_structures::DirectedIterList<int, data_structures::FixedEdged_DAL<int>>();
        iter_list.transversal(adj_list, 0);
        auto path = std::vector<Index>();
        auto target = std::vector<Index>{0, 1, 2, 3, 4, 5};
        for(auto const & n : iter_list) { path.push_back(n->index()); }
        REQUIRE(path == target);

        iter_list.path_transversal(adj_list, 0, 4);
        path = std::vector<Index>();
        target = std::vector<Index>{0, 2, 3, 4};
        for(auto const & n : iter_list) { path.push_back(n->index()); }
        REQUIRE(path == target);

        iter_list.path_transversal(adj_list, 0, 5);
        path = std::vector<Index>();
        target = std::vector<Index>{0, 2, 3, 5};
        for(auto const & n : iter_list) { path.push_back(n->index()); }
        REQUIRE(path == target);

        path = std::vector<Index>();
        target = std::vector<Index>{2, 3, 5, 4};
        iter_list.sub_graph_transversal(adj_list, 2, 5);
        for(auto const & n : iter_list) { path.push_back(n->index()); }
        REQUIRE(path == target);





    }











}