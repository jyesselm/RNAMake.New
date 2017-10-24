//
// Created by Joseph Yesselman on 1/29/17.
//

#ifndef RNAMAKE_PRIMITIVES_STRUCTURE_H
#define RNAMAKE_PRIMITIVES_STRUCTURE_H

#include <sstream>
#include <memory>

#include <base/vector_container.h>
#include <primitives/residue.h>
#include <primitives/chain.h>

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


// wrapper to avoid doing std::shared_ptr<std::vector<T>> since pybind11 wont allow this

template<typename Chaintype, typename Restype>
class Structure {
public:
    typedef std::vector<Restype>                          Residues;
    //typedef std::vector<Chaintype>                        Chains;
    //typedef std::shared_ptr<std::vector<Chaintype> >      ChainsOP;
    typedef base::VectorContainer<Chaintype>              Chains;
    typedef base::VectorContainerOP<Chaintype>            ChainsOP;


public:
    inline
    Structure(
            Residues const & res,
            Cutpoints const cut_points):
            residues_(res),
            cut_points_(cut_points) {



    }
    inline
    Structure(
            Structure const & s) {

    }

    virtual
    ~Structure() {}

public: //res iterator
    typedef typename Residues::const_iterator const_iterator;

    const_iterator begin() const { return residues_.begin(); }
    const_iterator end() const { return residues_.end(); }

public: //get_residue interface
    Restype const &
    get_residue(
            int num,
            char chain_id,
            char i_code) const{

        for (auto const & r : residues_) {
            if (num == r.get_num() && chain_id == r.get_chain_id() && i_code == r.get_i_code()) {
                return r;
            }
        }

        auto ss = std::stringstream();
        ss << "cannot find residue with num: " << num << " chain id: " << chain_id << " and i_code";
        throw StructureException(ss.str());
    }

    Restype const &
    get_residue(
            util::Uuid const & uuid) const {

        for (auto const & r : residues_) {
            if (r.get_uuid() == uuid) { return r; }
        }

        throw StructureException("cannot find residue by uuid");
    }

    Restype const &
    get_residue(
            Index index) const {

        expects<StructureException>(
                index < residues_.size(),
                "cannot get residue " + std::to_string(index) + " only " + std::to_string(residues_.size()) +
                " total residues");

        return residues_[index];
    }

    int
    get_res_index(
            Restype const & res) {
        int i = -1;
        for (auto const & r : residues_) {
            i++;
            if (r == res) { return i; }
        }
        throw StructureException("cannot find index for res: " + std::to_string(res.get_num()));
    }

public:
    ChainsOP
    get_chains() const {
        auto pos = 0;
        auto res = Residues();
        auto chains = std::vector<Chaintype>();
        auto i = 0;
        for(auto const & r : residues_) {
            if (cut_points_[pos] == i) {
                auto c = Chaintype(res);
                chains.push_back(c);
                res = Residues{r};
                pos += 1;
            } else {
                res.push_back(r);
            }
            i++;
        }
        if(res.size() > 0) {
            auto c = Chaintype(res);
            chains.push_back(c);
        }
        return std::make_shared<Chains>(chains);
    }

    size_t
    get_num_residues() { return residues_.size(); }

    size_t
    get_num_chains() { return cut_points_.size(); }

    String
    get_sequence() {
        auto i = -1;
        auto seq = String("");
        auto pos = 0;
        for(auto const & r : residues_) {
            i++;
            if(cut_points_[pos] == i) {
                seq += "&";
                pos++;
            }
            seq += r.get_name();
        }
        return seq;
    }


protected:
    Structure() {}

protected:

    Residues residues_;
    Cutpoints cut_points_;
};

typedef Structure<PrimitiveChain, PrimitiveResidue> PrimitiveStructure;


}

#endif //RNAMAKE_PRIMITIVES_STRUCTURE_H
