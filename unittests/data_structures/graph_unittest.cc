//
// Created by Joseph Yesselman on 10/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <primitives/residue.h>
#include <data_structures/graph.h>

TEST_CASE( "Test Graph Data Structure ", "[Graph]" ) {

    auto g = data_structures::Graph<primitives::Residue>();
    auto r1 = std::make_shared<primitives::Residue>('A', 1, 'A', ' ', util::Uuid());
    auto r2 = std::make_shared<primitives::Residue>('A', 2, 'A', ' ', util::Uuid());

    g.add_node(r1);
    g.add_node(r2, 0);

    auto nodes = primitives::PrimitiveResidues();
    for(auto const & n : g) {
        nodes.push_back(n);
    }
    REQUIRE(nodes.size() == 2);

    g.change_transverse_start(1);
    nodes = primitives::PrimitiveResidues();
    for(auto const & n : g) {
        nodes.push_back(n);
    }
    REQUIRE(nodes.size() == 2);
    REQUIRE(nodes[0] == *r2);
}