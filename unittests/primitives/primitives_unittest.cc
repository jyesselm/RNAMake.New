
#include <iostream>
#include "../common.hpp"

#include <primitives/residue.h>
#include <primitives/basepair.h>
#include <primitives/chain.h>
#include <primitives/structure.h>
#include <primitives/pose.h>

// dont do this, just for testing not main code!
using namespace primitives;

TEST_CASE( "Test Primitive Classes ", "[Primitives]" ) {

    class NewRes : public Residue {
    public:
        inline
        NewRes(
                char name,
                int num,
                char chain_id,
                char i_code,
                util::Uuid const & uuid) :
                Residue(name, num, chain_id, i_code, uuid),
                unique_(1) {}

        int
        get_unique() const { return unique_; }

        void
        set_unique(int val) { unique_ = val; }

    private:
        int unique_;

    };

    typedef Chain<NewRes> NewChain;
    typedef Structure<NewChain, NewRes> NewStructure;

    SECTION("Test get_bp_name") {
        auto r1 = Residue('A', 1, 'A', ' ', util::Uuid());
        auto r2 = Residue('A', 2, 'A', ' ', util::Uuid());
        REQUIRE(generate_bp_name<Residue>(r1, r2) == "A1-A2");

    }

    // Chains
    SECTION("test polymorphism") {

        auto r1 = NewRes('A', 1, 'A', ' ', util::Uuid());
        auto r2 = NewRes('A', 2, 'A', ' ', util::Uuid());
        REQUIRE(generate_bp_name<NewRes>(r1, r2) == "A1-A2");

        auto res = std::vector<NewRes>();
        res.push_back(r1); res.push_back(r2);
        auto c = Chain<NewRes>(res);
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
        auto c = Chain<NewRes>(res);

        r1.set_unique(10);
        REQUIRE(c.get_first().get_unique() == 1);

    }

    //Structure
    SECTION("test copy") {
        auto r1 = NewRes('A', 1, 'A', ' ', util::Uuid());
        auto r2 = NewRes('A', 2, 'A', ' ', util::Uuid());
        auto res = std::vector<NewRes>{r1, r2};
        auto s = NewStructure(res, Cutpoints{2});
        auto s2 = NewStructure(s);

    }

    SECTION("test get chains") {
        auto r1 = NewRes('A', 1, 'A', ' ', util::Uuid());
        auto r2 = NewRes('A', 2, 'A', ' ', util::Uuid());
        auto res = std::vector<NewRes>{r1, r2};

        auto s = NewStructure(res, Cutpoints{2});
        auto chains = s.get_chains();
        REQUIRE(chains->size() == 1);
        REQUIRE(r1 == chains->at(0).get_first());
    }

    SECTION("test pose") {
        auto r1 = Residue('A', 1, 'A', ' ', util::Uuid()); // 0
        auto r2 = Residue('G', 2, 'A', ' ', util::Uuid()); // 1
        auto r3 = Residue('U', 3, 'A', ' ', util::Uuid()); // 2
        auto r4 = Residue('A', 4, 'B', ' ', util::Uuid()); // 3
        auto r5 = Residue('C', 5, 'B', ' ', util::Uuid()); // 4
        auto r6 = Residue('U', 6, 'B', ' ', util::Uuid()); // 5

        auto name1 = generate_bp_name(r1, r6);
        auto name2 = generate_bp_name(r3, r4);
        auto name3 = generate_bp_name(r2, r5);

        auto bp1 = Basepair(r1.get_uuid(), r6.get_uuid(), util::Uuid(),
                                        BasepairType::WC, std::make_shared<base::SimpleString>(name1));
        auto bp2 = Basepair(r3.get_uuid(), r4.get_uuid(), util::Uuid(),
                                        BasepairType::WC, std::make_shared<base::SimpleString>(name2));
        auto bp3 = Basepair(r2.get_uuid(), r5.get_uuid(), util::Uuid(),
                                        BasepairType::WC, std::make_shared<base::SimpleString>(name3));

        auto bps = PrimitiveBasepairs{bp1, bp2, bp3};
        auto res = PrimitiveResidues{r1, r2, r3, r4, r5, r6};
        auto cut_points = Cutpoints{3, 6};

        auto s = PrimitiveStructure(res, cut_points);
        auto end_indexes = get_end_indexes_from_basepairs<PrimitiveBasepair, PrimitiveStructure>(s, bps);
        auto name = std::make_shared<base::SimpleString>("test");

        REQUIRE(end_indexes->size() == 2);
        REQUIRE(end_indexes->at(0) == 0);
        REQUIRE(end_indexes->at(1) == 1);

        auto end_ids = base::SimpleStringCOPs();
        for(auto const & ei : *end_indexes) {
            auto end_id = generate_end_id<PrimitiveStructure, PrimitiveChain, PrimitiveBasepair, PrimitiveResidue>(s, bps, bps[ei]);
            end_ids.push_back(std::make_shared<base::SimpleString>(end_id));
        }

        auto p = PrimitivePose(s, bps, end_indexes->get_data(), end_ids, name);

        REQUIRE(end_ids[0]->get_str() == "AGU_LLL_ACU_RRR");

        SECTION("test improper init") {
            REQUIRE_THROWS_AS(PrimitivePose(s, bps, end_indexes->get_data(), base::SimpleStringCOPs(), name),
                              PoseException);
        }

        SECTION("test get residue") {
            auto r = p.get_residue(1, 'A', ' ');
            REQUIRE(r == r1);

            r = p.get_residue(r1.get_uuid());
            REQUIRE(r == r1);

            r = p.get_residue(0);
            REQUIRE(r == r1);

            REQUIRE_THROWS_AS(p.get_residue(util::Uuid()), StructureException);
            REQUIRE_THROWS_AS(p.get_residue(100), StructureException);
            REQUIRE_THROWS_AS(p.get_residue(100, 'A', ' '), StructureException);
            REQUIRE_THROWS_AS(p.get_residue(1, 'A', 'F'), StructureException);
            REQUIRE_THROWS_AS(p.get_residue(1, 'B', ' '), StructureException);
        }

        SECTION("test get end") {
            auto end = p.get_end(0);
            REQUIRE(end == bp1);

            end = p.get_end(bp1.get_name());
            REQUIRE(end == bp1);

            end = p.get_end(bp1.get_uuid());
            REQUIRE(end == bp1);

            end = p.get_end(bp1.get_res1_uuid(), bp1.get_res2_uuid());
            REQUIRE(end == bp1);

            REQUIRE_THROWS_AS(p.get_end("FAKE"), PoseException);
            REQUIRE_THROWS_AS(p.get_end(100), PoseException);
            REQUIRE_THROWS_AS(p.get_end(util::Uuid()), PoseException);
            REQUIRE_THROWS_AS(p.get_end(util::Uuid(), util::Uuid()), PoseException);

        }

        SECTION("test get end by end id") {
            auto end = p.get_end_by_id(end_ids[0]);
            REQUIRE(end == bp1);

            end = p.get_end_by_id(end_ids[1]->get_str());
            REQUIRE(end == bp2);

            REQUIRE_THROWS_AS(p.get_end_by_id("FAKE"), PoseException);
        }

    }



}