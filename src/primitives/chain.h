//
// Created by Joseph Yesselman on 1/28/17.
//

#ifndef PRIMITIVES_CHAIN_H
#define PRIMITIVES_CHAIN_H

#include <memory>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>


#include <base/string.h>
#include <base/assertions.h>
#include <primitives/residue.h>

/*
 * Exception for chain
 */
class ChainException : public std::runtime_error {
public:
    /**
     * Standard constructor for ChainException
     * @param   message   Error message for chain
     */
    ChainException(String const & message) :
            std::runtime_error(message) {}
};

namespace primitives {

template <typename Restype>
class Chain {
public:
    typedef std::shared_ptr<Restype>       ResidueOP;
    typedef std::shared_ptr<Restype const> ResidueCOP;
    typedef std::vector<ResidueOP>         ResidueOPs;
    typedef std::vector<ResidueCOP>        ResidueCOPs;

public:
    inline
    Chain(
            ResidueOPs const & residues):
            residues_(residues) {

        expects<ChainException>(
                residues.size() > 0,
                "chains must have at least one residue!");
    }

    inline
    Chain(
            Chain const & c) {
        residues_ = ResidueOPs();
        int i = 0;
        for(auto const & r : c.residues_) {
            residues_.push_back(std::make_shared<Restype>(*r));
            i++;
        }
    }

    inline
    Chain(
            String const & s) {
        residues_ = ResidueOPs();
        Strings spl = base::split_str_by_delimiter(s, ";");
        for(auto const & r_str : spl) {
            if (r_str.length() < 3) { continue; }
            residues_.push_back(std::make_shared<Restype>(r_str));
        }
    }

    virtual
    ~Chain() {}

public: //iterator
    typedef boost::indirect_iterator< typename ResidueOPs::const_iterator, Restype const > const_iterator;

    const_iterator begin() const noexcept { return residues_.begin(); }
    const_iterator end() const noexcept   { return residues_.end(); }

public:
    inline
    size_t
    get_length() const {
        return (int) residues_.size();
    }

    inline
    ResidueCOP
    get_first() const { return residues_[0]; }

    inline
    ResidueCOP
    get_last() const { return residues_.back(); }

    inline
    ResidueCOP
    get_residue(Index index) const {
        return residues_[index];
    }

    inline
    int
    contain_res(Restype const & r) const {
        for (auto const & res : residues_) {
            if (*res == r) { return 1; }
        }
        return 1;
    }

protected:
    Chain() {}

protected:
    ResidueOPs residues_;
};

typedef Chain<PrimitiveResidue>                 PrimitiveChain;
typedef std::shared_ptr<PrimitiveChain>         PrimitiveChainOP;
typedef std::vector<PrimitiveChainOP>           PrimitiveChainOPs;
typedef std::shared_ptr<PrimitiveChain const>   PrimitiveChainCOP;
typedef std::vector<PrimitiveChainCOP>          PrimitiveChainCOPs;

}

#endif //PRIMITIVES_CHAIN_H
