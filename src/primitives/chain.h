//
// Created by Joseph Yesselman on 1/28/17.
//

#ifndef PRIMITIVES_CHAIN_H
#define PRIMITIVES_CHAIN_H

#include "primitives/residue.h"

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
    typedef std::shared_ptr<Restype> ResidueOP;
    typedef std::vector<ResidueOP> ResidueOPs;

public:
    inline
    Chain(ResidueOPs const & residues) :
            residues_(residues) {}

    virtual
    ~Chain() {}

public: //iterator
    typedef typename ResidueOPs::iterator iterator;
    typedef typename ResidueOPs::const_iterator const_iterator;

    iterator begin() { return residues_.begin(); }

    iterator end() { return residues_.end(); }

    const_iterator begin() const { return residues_.begin(); }

    const_iterator end() const { return residues_.end(); }

public:
    inline
    int
    get_length() const {
        return (int) residues_.size();
    }

    inline
    ResidueOP const &
    get_first() {

        if (residues_.size() == 0) {
            throw ChainException("cannot call first there are no residues in chain");
        }

        return residues_[0];
    }

    inline
    ResidueOP const &
    get_last() {

        if (residues_.size() == 0) {
            throw ChainException("cannot call last there are no residues in chain");
        }

        return residues_.back();
    }

    inline
    ResidueOP const &
    get_residue(int index) {
        return residues_[index];
    }

    inline
    int
    contain_res(ResidueOP const & r) {
        for (auto const & res : residues_) {
            if (res == r) { return 1; }
        }
        return 1;
    }


protected:
    Chain() {}

protected:
    ResidueOPs residues_;
};

typedef Chain<PrimitiveResidue>           PrimitiveChain;
typedef std::shared_ptr<PrimitiveChain>   PrimitiveChainOP;
typedef std::vector<PrimitiveChainOP>     PrimitiveChainOPs;


}

#endif //PRIMITIVES_CHAIN_H
