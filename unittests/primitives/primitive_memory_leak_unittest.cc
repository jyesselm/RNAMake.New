//
// Created by Joseph Yesselman on 10/25/17.
//

#include <primitives/pose.h>
#include <primitives/basepair.h>


int main(int argc, char *argv[]) {
    auto r1 = primitives::Residue('A', 1, 'A', ' ', util::Uuid());
    auto r2 = primitives::Residue('G', 2, 'A', ' ', util::Uuid());
    auto r3 = primitives::Residue('U', 3, 'A', ' ', util::Uuid());
    auto r4 = primitives::Residue('A', 4, 'B', ' ', util::Uuid());
    auto r5 = primitives::Residue('C', 5, 'B', ' ', util::Uuid());
    auto r6 = primitives::Residue('U', 6, 'B', ' ', util::Uuid());

    auto name1 = primitives::generate_bp_name<primitives::Residue>(r1, r6);
    auto name2 = primitives::generate_bp_name<primitives::Residue>(r3, r4);
    auto name3 = primitives::generate_bp_name<primitives::Residue>(r2, r5);

    auto bp1 = primitives::Basepair(r1.get_uuid(), r6.get_uuid(), util::Uuid(),
                                    primitives::BasepairType::WC, std::make_shared<base::SimpleString>(name1));
    auto bp2 = primitives::Basepair(r3.get_uuid(), r4.get_uuid(), util::Uuid(),
                                    primitives::BasepairType::WC, std::make_shared<base::SimpleString>(name2));
    auto bp3 = primitives::Basepair(r2.get_uuid(), r5.get_uuid(), util::Uuid(),
                                    primitives::BasepairType::WC, std::make_shared<base::SimpleString>(name3));

    auto bps = primitives::PrimitiveBasepairs{bp3};
    auto ends = primitives::PrimitiveBasepairs{bp1, bp2};

    auto s = primitives::PrimitiveStructure(primitives::PrimitiveResidues{r1, r2, r3, r4, r5, r6}, Cutpoints{3, 6});

    auto total_string = String("");
    for(int i = 0; i < 1000000; i ++) {
        auto & end = ends[0];
        if(rand() < 0.5) {
            end = ends[1];
        }
        auto string = primitives::generate_end_id<primitives::PrimitiveStructure,
                                                  primitives::PrimitiveChain,
                                                  primitives::PrimitiveBasepair,
                                                  primitives::PrimitiveResidue>(s, bps, ends, end);
        std::cout << string << std::endl;

        break;
    }

    return 0;


}