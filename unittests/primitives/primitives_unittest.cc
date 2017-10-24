
#include <iostream>
#include "../common.hpp"

#include <primitives/residue.h>
#include <primitives/basepair.h>
#include <primitives/chain.h>
#include <primitives/structure.h>

TEST_CASE( "Test Primitive Classes ", "[Primitives]" ) {
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

    typedef primitives::Chain<NewRes> Chain;
    typedef primitives::Structure<Chain, NewRes> Structure;

    SECTION("Test get_bp_name") {
        auto r1 = primitives::Residue('A', 1, 'A', ' ', util::Uuid());
        auto r2 = primitives::Residue('A', 2, 'A', ' ', util::Uuid());
        REQUIRE(primitives::generate_bp_name<primitives::Residue>(r1, r2) == "A1-A2");

    }

    // Chains
    SECTION("test polymorphism") {

        auto r1 = NewRes('A', 1, 'A', ' ', util::Uuid());
        auto r2 = NewRes('A', 2, 'A', ' ', util::Uuid());
        REQUIRE(primitives::generate_bp_name<NewRes>(r1, r2) == "A1-A2");

        auto res = std::vector<NewRes>();
        res.push_back(r1); res.push_back(r2);
        auto c = primitives::Chain<NewRes>(res);
        auto & first = c.get_first();

        // can't do this due to const ref
        //first.set_unique(10);

        REQUIRE(first.get_unique() == 1);
        REQUIRE(r1 == first);

        for(auto const & r : c) {
            REQUIRE(r.get_name() == 'A');
        }

    }

    SECTION("test deep copy") {
        auto r1 = NewRes('A', 1, 'A', ' ', util::Uuid());
        auto r2 = NewRes('A', 2, 'A', ' ', util::Uuid());
        auto res = std::vector<NewRes>{r1, r2};
        auto c = primitives::Chain<NewRes>(res);

        r1.set_unique(10);
        REQUIRE(c.get_first().get_unique() == 1);

    }

    //Structure
    SECTION("test get chains") {
        auto r1 = NewRes('A', 1, 'A', ' ', util::Uuid());
        auto r2 = NewRes('A', 2, 'A', ' ', util::Uuid());
        auto res = std::vector<NewRes>{r1, r2};

        auto s = Structure(res, Cutpoints{2});
        /*auto chains = s.get_chains();
        REQUIRE(chains->size() == 1);
        REQUIRE(r1 == chains->at(0).get_first());
        delete chains;
         */

    }

}