//
// Created by Joseph Yesselman on 2/15/17.
//

#ifndef PRIMITIVES_RNA_STRUCTURE_H
#define PRIMITIVES_RNA_STRUCTURE_H

#include <vector>
#include <map>

#include <base/string.h>
#include <base/simple_string.h>
#include <util/uuid.h>
#include <primitives/structure.h>
#include <primitives/basepair.h>

/*
 * Exception for RNA Structure
 */

class RNAStructureException : public std::runtime_error {
public:
    /**
     * Standard constructor for RNAStructureException
     * @param   message   Error message for rna structure
     */
    RNAStructureException(String const & message):
            std::runtime_error(message)
    {}
};

namespace primitives {
/*
template<typename BPtype, typename Structuretype, typename Chaintype, typename Restype>
class RNAStructure {
public:// types
    typedef std::shared_ptr<BPtype> BasepairOP;
    typedef std::vector<BasepairOP> BasepairOPs;
    typedef std::shared_ptr<Structuretype> StructureOP;
    typedef std::shared_ptr<Chaintype> ChainOP;
    typedef std::vector<ChainOP> ChainOPs;
    typedef std::shared_ptr<Restype> ResidueOP;
    typedef std::vector<ResidueOP> ResidueOPs;

public:
    RNAStructure(
            StructureOP const & structure,
            BasepairOPs const & basepairs,
            BasepairOPs const & ends,
            base::SimpleStringOPs const & end_ids,
            base::SimpleStringOP const & name):
            structure_(structure),
            basepairs_(basepairs),
            ends_(ends),
            end_ids_(end_ids),
            name_(name) {

        if(end_ids_.size() != ends_.size()) {
            throw RNAStructureException(
                    "RNAStructure must have the same number of ends as end_ids has " +
                    std::to_string(ends_.size()) + " ends and " + std::to_string(end_ids.size()) +
                    "end ids");
        }


    }

protected:
    // let dervived classes fill in members
    RNAStructure() {}

public: //iterators
    // residue iterator
    typedef typename ResidueOPs::iterator iterator;
    typedef typename ResidueOPs::const_iterator const_iterator;

    iterator begin() { return structure_->begin(); }
    iterator end() { return structure_->end(); }

    const_iterator begin() const { return structure_->begin(); }
    const_iterator end() const { return structure_->end(); }

    // basepair iterator
    typedef typename BasepairOPs::iterator bp_iterator;
    typedef typename BasepairOPs::const_iterator const_bp_iterator;

    bp_iterator bp_begin() { return basepairs_.begin(); }
    bp_iterator bp_end() { return basepairs_.end(); }

    const_bp_iterator bps_begin() const { return basepairs_.begin(); }
    const_bp_iterator bps_end() const { return basepairs_.end(); }

    // end iterator
    bp_iterator ends_begin() { return ends_.begin(); }
    bp_iterator ends_end() { return ends_.end(); }

    const_bp_iterator ends_begin() const { return ends_.begin(); }
    const_bp_iterator ends_end() const { return ends_.end(); }

public: //structure wrappers
    inline
    String
    get_sequence() { return structure_->sequence(); }

    inline
    ResidueOP const &
    get_residue(
            int num,
            char chain_id,
            char i_code) const {
        return structure_->get_residue(num, chain_id, i_code); }

    inline
    ResidueOP const &
    get_residue(
            util::Uuid const & uuid) const {
        return structure_->get_residue(uuid); }

    inline
    ResidueOP const &
    get_residue(
            int index) const {
        return structure_->get_residue(index); }

    inline
    size_t
    get_num_residues() const { return structure_->get_num_residues(); }

    inline
    size_t
    get_num_chains() const { return structure_->get_num_chains(); }

public: //get basepairs interface

    BasepairOPs
    get_basepairs(util::Uuid const & bp_uuid) const {
        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_uuid() == bp_uuid) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == bp_uuid ||
                bp->get_res2_uuid() == bp_uuid) { bps.push_back(bp); }

        }

        if(bps.size() == 0) {
            throw RNAStructureException("could not find any basepairs with this uuid for residues or basepairs");
        }

        return bps;
    }

    BasepairOPs
    get_basepairs(
            util::Uuid const & uuid1,
            util::Uuid const & uuid2) const {

        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_res1_uuid() == uuid1 && bp->get_res2_uuid() == uuid2) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == uuid2 && bp->get_res2_uuid() == uuid1) { bps.push_back(bp); }
        }

        if(bps.size() == 0) {
            throw RNAStructureException("could not find any basepairs with these two uuids");
        }

        return bps;
    }

    BasepairOPs
    get_basepairs(
            String const & name) const {
        auto bps = BasepairOPs();
        for (auto const & bp : basepairs_) {
            if (name == bp->get_name()->get_str()) { bps.push_back(bp); return bps; }
        }

        throw RNAStructureException("could not find basepair with name " + name);

    }


public: // get basepair interface  (single basepair!)

    BasepairOP const &
    get_basepair(util::Uuid const & bp_uuid) const {
        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_uuid() == bp_uuid) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == bp_uuid || bp->get_res2_uuid() == bp_uuid) { bps.push_back(bp); }

        }
        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this uuid");
        }
        if (bps.size() == 1) { return bps[0]; }
        else {
            throw RNAStructureException("no basepairs matching this uuid");
        }
    }

    BasepairOP const &
    get_basepair(
            util::Uuid const & uuid1,
            util::Uuid const & uuid2) const {

        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_res1_uuid() == uuid1 && bp->get_res2_uuid() == uuid2) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == uuid2 && bp->get_res2_uuid() == uuid1) { bps.push_back(bp); }
        }
        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching residue uuids");
        }
        if (bps.size() == 1) { return bps[0]; }
        else {
            throw RNAStructureException("no basepair found matching residue uuids supplied");
        }
    }

    BasepairOP const &
    get_basepair(String const & name) const {
        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_name()->get_str() == name) { bps.push_back(bp); }
        }

        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this name: " + name);
        }
        if (bps.size() == 1) { return bps[0]; }
        else {
            throw RNAStructureException("no basepair found matching residue uuids supplied");
        }

    }

    inline
    BasepairOP const &
    get_basepair(int index) const {

        if(index >= basepairs_.size()) {
            throw RNAStructureException(
                    "cannot get basepair " + std::to_string(index) + " only " + std::to_string(basepairs_.size()) +
                    " total residues");
        }
        return basepairs_[index]; }


public: // get end interace

    BasepairOP const &
    get_end(util::Uuid const & bp_uuid) const{
        BasepairOPs bps;
        for (auto const & bp : ends_) {
            if (bp->get_uuid() == bp_uuid) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == bp_uuid ||
                bp->get_res2_uuid() == bp_uuid) { bps.push_back(bp); }

        }
        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this uuid");
        }
        if (bps.size() == 1) { return bps[0]; }
        else {
            throw RNAStructureException("no end found matching basepair uuid supplied");
        }
    }

    BasepairOP const &
    get_end(
            util::Uuid const & uuid1,
            util::Uuid const & uuid2) const {

        BasepairOPs bps;
        for (auto const & bp : ends_) {
            if (bp->get_res1_uuid() == uuid1 &&
                bp->get_res2_uuid() == uuid2) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == uuid2 &&
                bp->get_res2_uuid() == uuid1) { bps.push_back(bp); }
        }
        if (bps.size() > 1) {
            throw RNAStructureException("got more than one end matching residue uuids");
        }
        if (bps.size() == 1) { return bps[0]; }
        else {
            throw RNAStructureException("no end found matching residue uuids supplied");

        }
    }

    BasepairOP const &
    get_end(base::SimpleStringOP const & name) const {
        BasepairOPs bps;
        for (auto const & bp : ends_) {
            if (bp->get_name() == name) { bps.push_back(bp); }
        }

        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this name: " + name->get_str());
        }
        if (bps.size() == 1) { return bps[0]; }
        else {
            throw RNAStructureException("cannot find end with name: " + name->get_str());
        }
    }

    BasepairOP const &
    get_end(String const & name) const {
        BasepairOPs bps;
        for (auto const & bp : ends_) {
            if (bp->get_name()->get_str() == name) { bps.push_back(bp); }
        }

        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this name: " + name);
        }
        if (bps.size() == 1) { return bps[0]; }
        else {
            throw RNAStructureException("cannot find end with name: " + name);
        }
    }

    inline
    BasepairOP const &
    get_end(int index) const {
        if(index >= ends_.size()) {
            throw RNAStructureException(
                    "trying to get end: " + std::to_string(index) + " there are only " +
                    std::to_string(ends_.size()));
        }
        return ends_[index];
    }

public: // get end by end id
    // avoid confliction with getting by name ... not pretty
    BasepairOP const &
    get_end_by_id(String const & end_id) const {
        BasepairOPs bps;
        int i = -1;
        for (auto const & bp : ends_) {
            i++;
            if (end_ids_[i]->get_str() == end_id) { bps.push_back(bp); }
        }

        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this end_id: " + end_id);
        }
        if (bps.size() == 1) { return bps[0]; }
        else {
            throw RNAStructureException("cannot find end with end_id: " + end_id);
        }
    }

    BasepairOP const &
    get_end_by_id(base::SimpleStringOP const & end_id) const {
        BasepairOPs bps;
        int i = -1;
        for (auto const & bp : ends_) {
            i++;
            if (end_ids_[i] == end_id) { bps.push_back(bp); }
        }

        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this end_id: " + end_id->get_str());
        }
        if (bps.size() == 1) { return bps[0]; }
        else {
            throw RNAStructureException("cannot find end with end_id: " + end_id->get_str());
        }
    }

public: // other getters

    inline
    base::SimpleStringOP const &
    get_end_id(int index) const {
        if(index >= end_ids_.size()) {
            throw RNAStructureException(
                    "trying to get end_id: " + std::to_string(index) + " there are only " +
                    std::to_string(end_ids_.size()));
        }
        return end_ids_[index];
    }

    int
    get_end_index(base::SimpleStringOP const & name) const {
        auto bp = get_end(name);
        if(bp == nullptr) { throw RNAStructureException("cannot find end with name: " + name->get_str()); }
        int i = 0;
        for(auto const & end : ends_) {
            if(bp == end) { return i; }
            i++;
        }

    }

    int
    get_end_index(String const & end_id) const {
        int i = 0;
        for(auto const & ei : end_ids_) {
            if(ei->get_str() == end_id) { return i; }
            i++;
        }
        throw RNAStructureException("cannot find end with end_id: " + end_id);


    }

    ResidueOPs
    get_bp_res(BPtype const & bp) const {
        auto res = ResidueOPs(2);
        res[0] = get_residue(bp.get_res1_uuid());
        res[1] = get_residue(bp.get_res2_uuid());
        return res;
    }

    size_t
    get_num_basepairs() const{ return basepairs_.size(); }

    size_t
    get_num_ends() const{ return ends_.size(); }

    base::SimpleStringOP const &
    get_name() { return name_; }

protected:
    std::shared_ptr<Structuretype> structure_;
    std::vector<std::shared_ptr<BPtype> > basepairs_;
    std::vector<std::shared_ptr<BPtype> > ends_;
    base::SimpleStringOPs end_ids_;
    base::SimpleStringOP name_;

};

typedef RNAStructure<PrimitiveBasepair, PrimitiveStructure, PrimitiveChain, PrimitiveResidue> PrimitiveRNAStructure;
typedef std::shared_ptr<PrimitiveRNAStructure> PrimitiveRNAStructureOP;
typedef std::vector<PrimitiveRNAStructureOP>   PrimitiveRNAStructureOPs;


template<typename BPtype, typename Structuretype>
std::vector<std::shared_ptr<BPtype> >
ends_from_basepairs(
        Structuretype const & s,
        std::vector<std::shared_ptr<BPtype> > const & bps) {
    auto chain_end_uuids = std::vector<util::Uuid>();
    for(auto const & c : s.get_chains()) {
        chain_end_uuids.push_back(c->first()->uuid());
        if(c->length() > 1) {
            chain_end_uuids.push_back(c->last()->uuid());
        }
    }

    auto ends = std::vector<std::shared_ptr<BPtype> >();
    for(auto const & bp : bps) {
        if(bp->bp_type() == BasepairType::NC) { continue; }
        if(std::find(chain_end_uuids.begin(), chain_end_uuids.end(), bp->res1_uuid()) == chain_end_uuids.end())  { continue; }
        if(std::find(chain_end_uuids.begin(), chain_end_uuids.end(), bp->res2_uuid()) == chain_end_uuids.end())  { continue; }
        ends.push_back(bp);
    }

    return ends;

}

PrimitiveBasepairOP
get_res_wc_or_gu_basepair(
        PrimitiveBasepairOPs const &,
        PrimitiveBasepairOPs const &,
        PrimitiveResidueOP const &);

String
generate_end_id(
        PrimitiveStructureOP const &,
        PrimitiveBasepairOPs const &,
        PrimitiveBasepairOPs const &,
        PrimitiveBasepairOP  const &);
*/
/*Strings
end_id_to_seq_and_db(String const & ss_id) {
    auto ss = String("");
    auto seq = String("");
    auto spl = base::split_str_by_delimiter(ss_id, "_");
    for(int i = 0; i < spl.size()-1; i+=2) {
        seq += spl[i];
        for(auto const & e : spl[i+1]) {
            if     (e == 'L') { ss += "("; }
            else if(e == 'R') { ss += ")"; }
            else if(e == 'U') { ss += "."; }
            else {
                throw std::runtime_error("unexpected symbol in ss_id: " + std::to_string(e) );
            }
        }

        if(i != spl.size()-2) {
            seq += "&";
            ss  += "&";
        }
    }

    auto dummy_str = Strings(2);
    dummy_str[0] = seq; dummy_str[1] = ss;
    return dummy_str;
}*/


}

#endif //PRIMITIVES_RNA_STRUCTURE_H
