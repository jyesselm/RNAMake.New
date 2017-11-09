//
// Created by Joseph Yesselman on 10/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <base/unique_ptr.h>
#include <primitives/residue.h>
#include <data_structures/graph.h>
#include <data_structures/directed_graph.h>

TEST_CASE( "Test Graph Data Structure ", "[Graph]" ) {
    auto r1 = std::make_shared<primitives::Residue>('A', 1, 'A', ' ', util::Uuid());
    auto r2 = std::make_shared<primitives::Residue>('A', 2, 'A', ' ', util::Uuid());
    auto r3 = std::make_shared<primitives::Residue>('A', 3, 'A', ' ', util::Uuid());
    auto r4 = std::make_shared<primitives::Residue>('A', 4, 'A', ' ', util::Uuid());

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
        REQUIRE(nodes[0]->get_num() == 2);
    }

    SECTION("test static edge graph") {
        auto g = data_structures::FixedNumEdgesGraph<primitives::Residue>();
        REQUIRE(g.add_node(r1, 3) == 0);
        REQUIRE(g.add_node(r2, 3, 0, 0, 0) == 1);

        //just filled it should now not be empty
        REQUIRE(!g.edge_index_empty(0, 0));

        auto nodes = std::vector<primitives::PrimitiveResidue const *>();
        for (auto const & n : g) {
            nodes.push_back(n.data);
        }
        REQUIRE(nodes.size() == 2);

    }

    SECTION("test directed graph") {
        auto g = data_structures::DirectedGraph<primitives::Residue>();
        REQUIRE(g.add_node(r1, 3) == 0);
        REQUIRE(g.add_node(r2, 3, 0, 0, 0) == 1);
        REQUIRE(g.get_node(0) == r1.get());
        REQUIRE(g.get_parent_index(1) == 0);

        REQUIRE_THROWS_AS(g.get_parent_index(0), data_structures::GraphException);

        REQUIRE(g.has_parent(1));
        REQUIRE(!g.has_parent(0));

        REQUIRE(g.get_parent_edge_index(1) == 0);

        auto all_edges = g.get_all_edges();
        REQUIRE(all_edges->size() == 1);

        REQUIRE(g.edge_exist(0, 1, 0, 0));

        SECTION("test simple copy") {
            auto g2 = data_structures::DirectedGraph<primitives::Residue>(g);
            REQUIRE(g2.get_num_nodes() == 2);
        }

        g.set_node(r3, 0);
        REQUIRE(g.get_node(0)->get_num() == 3);

    }
}