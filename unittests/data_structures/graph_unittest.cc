//
// Created by Joseph Yesselman on 10/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <primitives/residue.h>
#include <data_structures/graph.h>
#include <data_structures/directed_graph.h>

TEST_CASE( "Test Graph Data Structure ", "[Graph]" ) {

    auto r1 = primitives::Residue('A', 1, 'A', ' ', util::Uuid());
    auto r2 = primitives::Residue('A', 2, 'A', ' ', util::Uuid());

    SECTION("test basic graph") {

        auto g = data_structures::Graph<primitives::Residue>();
        g.add_node(r1);
        g.add_node(r2, 0);

        g.change_transverse_start(1);
        auto nodes = std::vector<primitives::PrimitiveResidue const *>();
        for (auto const & n : g) {
            nodes.push_back(n.data);
        }
        REQUIRE(nodes.size() == 2);
        REQUIRE(*nodes[0] == r2);
    }

    SECTION("test static edge graph") {
        auto g = data_structures::FixedNumEdgesGraph<primitives::Residue>();
        REQUIRE(g.add_node(r1, 3) == 0);
        REQUIRE(g.add_node(r2, 3, 0, 0, 0) == 1);

        //just filled it should now not be empty
        REQUIRE(g.edge_index_empty(0, 0) == false);

        auto nodes = std::vector<primitives::PrimitiveResidue const *>();
        for (auto const & n : g) {
            nodes.push_back(n.data);
        }
        REQUIRE(nodes.size() == 2);

    }

    SECTION("test directed graph") {
        auto g = data_structures::DirectedGraph<primitives::Residue>();
        REQUIRE(g.add_node(r1, 3) == 0);
        REQUIRE(g.get_node(0) == r1);

    }
}