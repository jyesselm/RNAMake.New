//
// Created by Joseph Yesselman on 1/29/17.
//

#ifndef RNAMAKE_PRIMITIVES_STRUCTURE_H
#define RNAMAKE_PRIMITIVES_STRUCTURE_H

#include <sstream>

#include "primitives/residue.h"
#include "primitives/chain.h"

class StructureException : public std::runtime_error {
public:
    /**
     * Standard constructor for StructureException
     * @param   message   Error message for structure
     */
    StructureException(String const & message) :
            std::runtime_error(message) {}
};

namespace primitives {

template<typename Chaintype, typename Restype>
class Structure {
public:
    typedef std::shared_ptr<Restype>         ResidueOP;
    typedef std::shared_ptr<Restype const>   ResidueCOP;
    typedef std::vector<ResidueOP>           ResidueOPs;
    typedef std::vector<ResidueCOP>          ResidueCOPs;

    typedef std::shared_ptr<Chaintype>       ChainOP;
    typedef std::vector<ChainOP>             ChainOPs;
    typedef std::shared_ptr<Chaintype const> ChainCOP;
    typedef std::vector<ChainCOP>            ChainCOPs;

public:
    inline
    Structure(
            ResidueOPs const & res,
            Ints const chain_cuts):
            residues_(res),
            chain_cuts_(chain_cuts) {}
    inline
    Structure(
            Structure const & s) {

    }

    virtual
    ~Structure() {}

public: //res iterator
    typedef typename ResidueCOPs::const_iterator const_iterator;

    const_iterator begin() const { return residues_.begin(); }
    const_iterator end() const { return residues_.end(); }

public: //get_residue interface
    ResidueCOP
    get_residue(
            int num,
            char chain_id,
            char i_code) const{

        for (auto const & r : residues_) {
            if (num == r->get_num() && chain_id == r->get_chain_id() && i_code == r->get_i_code()) {
                return r;
            }
        }

        auto ss = std::stringstream();
        ss << "cannot find residue with num: " << num << " chain id: " << chain_id << " and i_code";
        throw StructureException(ss.str());
    }

    ResidueCOP
    get_residue(
            util::Uuid const & uuid) const {

        for (auto const & r : residues_) {
            if (r->get_uuid() == uuid) { return r; }
        }

        throw StructureException("cannot find residue by uuid");
    }

    ResidueCOP
    get_residue(
            int index) const {

        if(index >= residues_.size()) {
            throw StructureException(
                    "cannot get residue " + std::to_string(index) + " only " + std::to_string(residues_.size()) +
                    " total residues");
        }

        return residues_[index];
    }

    int
    get_res_index(ResidueOP const & res) {
        int i = -1;
        for (auto const & r : residues_) {
            i++;
            if (r == res) { return i; }
        }
        throw StructureException("cannot find index for res: " + std::to_string(res->get_num()));
    }

public:
    ChainCOPs
    get_chains() const {
        auto pos = 0;
        auto res = ResidueOPs();
        auto chains = ChainCOPs();
        auto i = 0;
        for(auto const & r : residues_) {
            if (chain_cuts_[pos] == i) {
                auto c = std::make_shared<Chaintype const>(res);
                chains.push_back(c);
                res = ResidueOPs();
                res.push_back(r);
                pos += 1;
            } else {
                res.push_back(r);
            }
            i++;
        }
        if(res.size() > 0) {
            auto c = std::make_shared<Chaintype const>(res);
            chains.push_back(c);
        }
        return chains;
    }

    size_t
    get_num_residues() { return residues_.size(); }

    size_t
    get_num_chains() { return chain_cuts_.size(); }

    String
    get_sequence() {
        auto i = -1;
        auto seq = String("");
        auto pos = 0;
        for(auto const & r : residues_) {
            i++;
            if(chain_cuts_[pos] == i) {
                seq += "&";
                pos++;
            }
            seq += r->get_name();
        }
        return seq;
    }


protected:
    Structure() {}

protected:

    ResidueOPs residues_;
    Ints chain_cuts_;
};

typedef Structure<PrimitiveChain, PrimitiveResidue> PrimitiveStructure;
typedef std::shared_ptr<PrimitiveStructure>         PrimitiveStructureOP;
typedef std::vector<PrimitiveStructureOP>           PrimitiveStructureOPs;

typedef std::shared_ptr<PrimitiveStructure const>    PrimitiveStructureCOP;
typedef std::vector<PrimitiveStructureCOP>           PrimitiveStructureCOPs;

}

#endif //RNAMAKE_PRIMITIVES_STRUCTURE_H
