//
// Created by Joseph Yesselman on 1/29/17.
//

#ifndef RNAMAKE_PRIMITIVES_BASEPAIR_H
#define RNAMAKE_PRIMITIVES_BASEPAIR_H

#include <base/simple_string.h>
#include <util/uuid.h>
//#include <util/x3dna.h>
#include <primitives/residue.h>

/*
 * Exception for basepair
 */
class BasepairException : public std::runtime_error {
public:
    /**
     * Standard constructor for BasepairException
     * @param   message   Error message for basepair
     */
    BasepairException(String const & message) :
            std::runtime_error(message) {}
};

namespace primitives {

// WC: watson crick basepair
// GU: gu basepair
// NC : mismatched basepair (non-conical)
enum BasepairType { WC, GU, NC};

class Basepair {
public:
    inline
    Basepair(
            util::Uuid const & res1_uuid,
            util::Uuid const & res2_uuid,
            util::Uuid const & uuid,
            BasepairType const & bp_type,
            base::SimpleStringOP const & name):
            res1_uuid_(res1_uuid),
            res2_uuid_(res2_uuid),
            uuid_(uuid),
            bp_type_(bp_type),
            name_(name){}

    virtual
    ~Basepair() {}

public:
    /**
    * equal operator checks whether the unique indentifier is the same
    * @param other another basepair to check if its the same
    */
    inline
    bool
    operator==(Basepair & other) {
        return uuid_ == other.uuid_;
    }

    inline
    bool
    operator!=(Basepair & other) const {
        return uuid_ != other.uuid_;
    }

protected:
    inline
    Basepair():
            res1_uuid_(util::Uuid()),
            res2_uuid_(util::Uuid()),
            uuid_(util::Uuid()) {}

public:
    util::Uuid const &
    get_partner(util::Uuid const &);

    inline
    BasepairType const &
    get_bp_type() { return bp_type_; }

    inline
    util::Uuid const &
    get_uuid() { return uuid_; }

    inline
    base::SimpleStringOP const &
    get_name() { return name_; }

    inline
    util::Uuid const &
    get_res1_uuid() const { return res1_uuid_; }

    inline
    util::Uuid const &
    get_res2_uuid() const { return res2_uuid_; }


protected:
    util::Uuid uuid_;
    util::Uuid res1_uuid_, res2_uuid_;
    BasepairType bp_type_;
    base::SimpleStringOP name_;
};

typedef std::shared_ptr<Basepair> BasepairOP;
typedef std::vector<BasepairOP>   BasepairOPs;

typedef Basepair    PrimitiveBasepair;
typedef BasepairOP  PrimitiveBasepairOP;
typedef BasepairOPs PrimitiveBasepairOPs;

String
generate_bp_name(
        PrimitiveResidueOP,
        PrimitiveResidueOP);

/*template <typename Restype>
Basepair::BasepairType
get_bp_type(std::vector<std::shared_ptr<Restype>> const & res,
            X3dna::X3dnaBPType const & x3dna_bp_type) {
    auto bp_str = String();
    bp_str += res[0]->name(); bp_str += res[1]->name();
    auto wc_names = Strings{"GC", "CG", "AU", "UA"};
    // not in WC orientation, is non-cononical
    if(x3dna_bp_type != X3dna::X3dnaBPType::cWUW) { return BasepairType::NC; }
    // has correct residues to be a WC basepair
    if (std::find(wc_names.begin(), wc_names.end(), bp_str) != wc_names.end()) { return BasepairType::WC; }
    if (bp_str == "GU" || bp_str == "UG" ) { return BasepairType::GU; }
    return BasepairType::NC;
}
*/
}


#endif //TEST_BASEPAIR_H
