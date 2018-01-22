//
// Created by Joseph Yesselman on 10/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <primitives/residue.h>
#include <data_structures/graph.h>

TEST_CASE( "Test Graph Data Structure ", "[Graph]" ) {
    auto g = data_structures::FixedEdgeDirectedGraph<int>();
    g.add_node(0, 3);
    g.add_node(1, 3, 0, data_structures::NodeIndexandEdge{0, 0});

    REQUIRE(g.get_num_nodes() == 2);

    struct X {
        int x; int y;
    };

    auto g1 = data_structures::FixedEdgeDirectedGraph<X>();
    g1.add_node(X{0, 0}, 3);

    REQUIRE(g1.get_node_data(0).x == 0);
}