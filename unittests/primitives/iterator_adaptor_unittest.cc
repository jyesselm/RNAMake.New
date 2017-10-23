//
// Created by Joseph Yesselman on 10/22/17.//


#include <primitives/residue.h>
#include <primitives/basepair.h>
#include <primitives/chain.h>

class AltChain {
public:
    AltChain(
            primitives::PrimitiveResidueOPs const & residues):
            residues_(residues) {}

public:
    typedef typename primitives::PrimitiveResidueOPs::const_iterator const_iterator;

    const_iterator begin() const { return residues_.begin(); }
    const_iterator end() const { return residues_.end(); }

private:
    primitives::PrimitiveResidueOPs residues_;

};

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

double
run_current_chain(
        primitives::PrimitiveResidueOPs const & res) {
    auto c = primitives::PrimitiveChain(res);
    //auto c = AltChain(res);
    double total = 0;
    for(int i = 0; i < 10000000; i++) {
        for(auto const & r : c) {
            total += r.get_num()*rand();
        }
        total = total*0.0001;
    }
    return total;
}

double
run_simple_chain(
        primitives::PrimitiveResidueOPs const & res) {
    auto c = AltChain(res);
    double total = 0;
    for(int i = 0; i < 10000000; i++) {
        for(auto const & r : c) {
            total += r->get_num()*rand();
        }
        total = total*0.0001;
    }
    return total;
}



int main(int argc, char *argv[] ) {
    auto res = primitives::PrimitiveResidueOPs();
    for(int i = 1; i < 100; i++) {
        auto r = std::make_shared<NewRes>('A', i, 'A', ' ', util::Uuid());
        res.push_back(r);
    }
    auto result1 = run_current_chain(res);
    auto result2 = run_simple_chain(res);
    auto result = result1 + result2;

}