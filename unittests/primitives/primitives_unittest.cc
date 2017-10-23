
#include <iostream>
#include "../common.hpp"

#include <primitives/residue.h>
#include <primitives/basepair.h>
#include <primitives/chain.h>

TEST_CASE( "Test Primitive Classes ", "[Primitives]" ) {

    SECTION("Test get_bp_name") {
        auto r1 = std::make_shared<primitives::Residue>('A', 1, 'A', ' ', util::Uuid());
        auto r2 = std::make_shared<primitives::Residue>('A', 2, 'A', ' ', util::Uuid());
        REQUIRE(primitives::generate_bp_name(r1, r2) == "A1-A2");

    }

    SECTION("test polymorphism") {
        class NewRes : public primitives::Residue {
        public:
            inline
            NewRes(
                    char name,
                    int num,
                    char chain_id,
                    char i_code,
                    util::Uuid const & uuid) :
                    primitives::Residue(name, num, chain_id, i_code, uuid),
                    unique_(1) {}

            int
            get_unique() const { return unique_; }

            void
            set_unique(int val) { unique_ = val; }

        private:
            int unique_;

        };

        auto r1 = std::make_shared<NewRes>('A', 1, 'A', ' ', util::Uuid());
        auto r2 = std::make_shared<NewRes>('A', 2, 'A', ' ', util::Uuid());
        REQUIRE(primitives::generate_bp_name(r1, r2) == "A1-A2");

        auto res = std::vector<std::shared_ptr<NewRes>>();
        res.push_back(r1); res.push_back(r2);
        auto c = primitives::Chain<NewRes>(res);
        auto first = c.get_first();

        REQUIRE(first->get_unique() == 1);
        REQUIRE(r1 == first);

        for(auto const & r : c) {
            REQUIRE(r.get_name() == 'A');
        }

        // can't do this due to const pointer
        //first->set_unique(10);
        //std::cout << c.get_first().unique() << std::endl;
    }

}