//
// Created by Joseph Yesselman on 10/27/17.
//

#include <iostream>
#include "../common.hpp"

#include <secondary_structure/parser.h>

TEST_CASE( "Test Secondary Structure Parser ", "[SecondaryStructureParser]" ) {
    /*SECTION("test ChainGraph") {
        auto s = secondary_structure::get_structure_from_secondary_structure("ACG&CGU","(((&)))");
        auto g = secondary_structure::ChainGraph();

        for(auto const & r : *s) {
            g.add_chain(std::vector<secondary_structure::Residue const *>({&r}),
                        secondary_structure::ChainStructureType::PAIRED);
        }

        REQUIRE(g.get_index_of_residue(s->get_residue(0)) == 0);
        g.pair_nodes(0, 5);

        REQUIRE_THROWS_AS(g.pair_nodes(0, 5), data_structures::GraphException);
    }*/

    SECTION("test parse") {
        auto p = secondary_structure::Parser();
        auto rs = p.parse_to_pose("GAC&GUC", "(((&)))");
        REQUIRE(rs->get_num_residues() == 6);
        REQUIRE(rs->get_num_ends() == 2);
        REQUIRE(rs->get_num_basepairs() == 3);
    }

}
