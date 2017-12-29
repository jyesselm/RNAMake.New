//
// Created by Joseph Yesselman on 2/15/17.
//

#ifndef PRIMITIVES_RNA_STRUCTURE_H
#define PRIMITIVES_RNA_STRUCTURE_H

#include <vector>
#include <map>
#include <memory>

#include <base/assertions.h>
#include <base/string.h>
#include <base/simple_string.h>
#include <base/vector_container.h>
#include <util/uuid.h>
#include <primitives/structure.h>
#include <primitives/basepair.h>

/*
 * Exception for RNA Structure
 */

class PoseException : public std::runtime_error {
public:
    /**
     * Standard constructor for RNAStructureException
     * @param   message   Error message for rna structure
     */
    PoseException(String const & message):
            std::runtime_error(message)
    {}
};

namespace primitives {

template<typename BPtype, typename Structuretype, typename Chaintype, typename Restype>
class Pose {
public:// types

    typedef std::vector<Restype>                 Residues;
    typedef base::VectorContainerOP<Restype>     ResiduesOP;
    //typedef std::vector<BPtype>                  Basepairs;
    typedef base::VectorContainerOP<BPtype>      BasepairsOP;


public:
    Pose(
            Structuretype const & structure,
            std::vector<BPtype> const & basepairs,
            std::vector<BPtype> const & ends,
            base::SimpleStringCOPs const & end_ids,
            base::SimpleStringCOP name):
            structure_(structure),
            basepairs_(basepairs),
            ends_(ends),
            end_ids_(end_ids),
            name_(name) {

        expects<PoseException>(
                end_ids_.size() == ends_.size(),
                "RNAStructure must have the same number of ends as end_ids has " +
                std::to_string(ends_.size()) + " ends and " + std::to_string(end_ids.size()) +
                "end ids");
    }

    Pose(
            Pose const & rs):
            structure_(rs.structure_),
            basepairs_(rs.basepairs_),
            ends_(rs.ends_),
            end_ids_(rs.end_ids_),
            name_(rs.name_) {}

    Pose(
            String const & s) {
        auto spl = base::split_str_by_delimiter(s, "&");
    }

protected:
    // let dervived classes fill in members
    Pose() {}

public: //iterators
    // residue iterator
    typedef typename Residues::const_iterator const_iterator;

    const_iterator begin() const { return structure_.begin(); }
    const_iterator end() const   { return structure_.end(); }

    // basepair iterator
    typedef typename std::vector<BPtype>::const_iterator const_bp_iterator;

    const_bp_iterator bps_begin() const { return basepairs_.begin(); }
    const_bp_iterator bps_end() const { return basepairs_.end(); }

    // end iterator
    const_bp_iterator ends_begin() const { return ends_.begin(); }
    const_bp_iterator ends_end() const { return ends_.end(); }

public: //structure wrappers
    inline
    String
    get_sequence() { return structure_.sequence(); }

    inline
    Restype const &
    get_residue(
            int num,
            char chain_id,
            char i_code) const {
        return structure_.get_residue(num, chain_id, i_code); }

    inline
    Restype const &
    get_residue(
            util::Uuid const & uuid) const {
        return structure_.get_residue(uuid); }

    inline
    Restype const &
    get_residue(
            Index index) const {
        return structure_.get_residue(index); }

    inline
    size_t
    get_num_residues() const { return structure_.get_num_residues(); }

    inline
    size_t
    get_num_chains() const { return structure_.get_num_chains(); }

public: //get basepairs interface

    BasepairsOP
    get_basepairs(
            util::Uuid const & bp_uuid) const {
        auto bps = std::vector<BPtype>();
        for (auto const & bp : basepairs_) {
            if (bp.get_uuid() == bp_uuid) { bps.push_back(bp); }
            if (bp.get_res1_uuid() == bp_uuid ||
                bp.get_res2_uuid() == bp_uuid) { bps.push_back(bp); }

        }

        if(bps.size() == 0) {
            throw PoseException("could not find any basepairs with this uuid for residues or basepairs");
        }

        return std::make_shared<base::VectorContainer<BPtype>>(bps);
    }

    BasepairsOP
    get_basepairs(
            util::Uuid const & uuid1,
            util::Uuid const & uuid2) const {

        auto bps = std::vector<BPtype>();
        for (auto const & bp : basepairs_) {
            if (bp.get_res1_uuid() == uuid1 && bp.get_res2_uuid() == uuid2) { bps.push_back(bp); }
            if (bp.get_res1_uuid() == uuid2 && bp.get_res2_uuid() == uuid1) { bps.push_back(bp); }
        }

        if(bps.size() == 0) {
            throw PoseException("could not find any basepairs with these two uuids");
        }

        return std::make_shared<base::VectorContainer<BPtype>>(bps);
    }

    BasepairsOP
    get_basepairs(
            String const & name) const {
        auto bps = std::vector<BPtype>();
        for (auto const & bp : basepairs_) {
            if (name == bp.get_name()->get_str()) {
                bps.push_back(bp);
            }
        }

        if(bps.size() == 0) {
            throw PoseException("could not find any basepairs with this name: " + name);
        }

        return std::make_shared<base::VectorContainer<BPtype>>(bps);

    }

public: // get basepair interface  (single basepair!)

    BPtype const &
    get_basepair(
            util::Uuid const & bp_uuid) const {
        auto bps = std::vector<BPtype const *>();
        for (auto const & bp : basepairs_) {
            if (bp.get_uuid() == bp_uuid) { bps.push_back(&bp); }
            if (bp.get_res1_uuid() == bp_uuid || bp.get_res2_uuid() == bp_uuid) { bps.push_back(&bp); }
        }
        if (bps.size() > 1) {
            throw PoseException("got more than one basepair matching this uuid");
        }
        if (bps.size() == 1) { return *bps[0]; }
        else {
            throw PoseException("no basepairs matching this uuid");
        }
    }

    BPtype const &
    get_basepair(
            util::Uuid const & uuid1,
            util::Uuid const & uuid2) const {

        auto bps = std::vector<BPtype const *>();
        for (auto const & bp : basepairs_) {
            if (bp.get_res1_uuid() == uuid1 && bp.get_res2_uuid() == uuid2) { bps.push_back(&bp); }
            if (bp.get_res1_uuid() == uuid2 && bp.get_res2_uuid() == uuid1) { bps.push_back(&bp); }
        }
        if (bps.size() > 1) {
            throw PoseException("got more than one basepair matching residue uuids");
        }
        if (bps.size() == 1) { return *bps[0]; }
        else {
            throw PoseException("no basepair found matching residue uuids supplied");
        }
    }

    BPtype const &
    get_basepair(
            String const & name) const {
        auto bps = std::vector<BPtype const *>();
        for (auto const & bp : basepairs_) {
            if (bp.get_name()->get_str() == name) { bps.push_back(&bp); }
        }

        if (bps.size() > 1) {
            throw PoseException("got more than one basepair matching this name: " + name);
        }
        if (bps.size() == 1) { return *bps[0]; }
        else {
            throw PoseException("no basepair found matching residue uuids supplied");
        }

    }

    inline
    BPtype const &
    get_basepair(
            Index index) const {
        expects<PoseException>(
                index < basepairs_.size(),
                "cannot get basepair " + std::to_string(index) + " only " + std::to_string(basepairs_.size()) +
                " total residues");
        return basepairs_[index]; }


public: // get end interace

    BPtype const &
    get_end(
            util::Uuid const & bp_uuid) const{
        auto bps = std::vector<BPtype const *>();
        for (auto const & bp : ends_) {
            if (bp.get_uuid() == bp_uuid) { bps.push_back(&bp); }
            if (bp.get_res1_uuid() == bp_uuid ||
                bp.get_res2_uuid() == bp_uuid) { bps.push_back(&bp); }

        }
        if (bps.size() > 1) {
            throw PoseException("got more than one basepair matching this uuid");
        }
        if (bps.size() == 1) { return *bps[0]; }
        else {
            throw PoseException("no end found matching basepair uuid supplied");
        }
    }

    BPtype const &
    get_end(
            util::Uuid const & uuid1,
            util::Uuid const & uuid2) const {

        auto bps = std::vector<BPtype const *>();
        for (auto const & bp : ends_) {
            if (bp.get_res1_uuid() == uuid1 &&
                bp.get_res2_uuid() == uuid2) { bps.push_back(&bp); }
            if (bp.get_res1_uuid() == uuid2 &&
                bp.get_res2_uuid() == uuid1) { bps.push_back(&bp); }
        }
        if (bps.size() > 1) {
            throw PoseException("got more than one end matching residue uuids");
        }
        if (bps.size() == 1) { return *bps[0]; }
        else {
            throw PoseException("no end found matching residue uuids supplied");

        }
    }

    BPtype const &
    get_end(
            base::SimpleStringCOP name) const {
        auto bps = std::vector<BPtype const *>();
        for (auto const & bp : ends_) {
            if (bp.get_name() == name) { bps.push_back(&bp); }
        }

        if (bps.size() > 1) {
            throw PoseException("got more than one basepair matching this name: " + name->get_str());
        }
        if (bps.size() == 1) { return *bps[0]; }
        else {
            throw PoseException("cannot find end with name: " + name->get_str());
        }
    }

    BPtype const &
    get_end(
            String const & name) const {
        auto bps = std::vector<BPtype const *>();
        for (auto const & bp : ends_) {
            if (bp.get_name()->get_str() == name) { bps.push_back(&bp); }
        }

        if (bps.size() > 1) {
            throw PoseException("got more than one basepair matching this name: " + name);
        }
        if (bps.size() == 1) { return *bps[0]; }
        else {
            throw PoseException("cannot find end with name: " + name);
        }
    }

    inline
    BPtype const &
    get_end(
            Index index) const {

        expects<PoseException>(
                index < ends_.size(),
                "trying to get end: " + std::to_string(index) + " there are only " +
                std::to_string(ends_.size()));

        return ends_[index];
    }

public: // get end by end id
    // avoid confliction with getting by name ... not pretty
    BPtype const &
    get_end_by_id(
            String const & end_id) const {
        auto bps = std::vector<BPtype const *>();
        int i = -1;
        for (auto const & bp : ends_) {
            i++;
            if (end_ids_[i]->get_str() == end_id) { bps.push_back(&bp); }
        }

        if (bps.size() > 1) {
            throw PoseException("got more than one basepair matching this end_id: " + end_id);
        }
        if (bps.size() == 1) { return *bps[0]; }
        else {
            throw PoseException("cannot find end with end_id: " + end_id);
        }
    }

    BPtype const &
    get_end_by_id(
            base::SimpleStringCOP end_id) const {
        auto bps = std::vector<BPtype const *>();
        int i = -1;
        for (auto const & bp : ends_) {
            i++;
            if (end_ids_[i] == end_id) { bps.push_back(&bp); }
        }

        if (bps.size() > 1) {
            throw PoseException("got more than one basepair matching this end_id: " + end_id->get_str());
        }
        if (bps.size() == 1) { return *bps[0]; }
        else {
            throw PoseException("cannot find end with end_id: " + end_id->get_str());
        }
    }

public: // other getters

    inline
    base::SimpleStringCOP
    get_end_id(
            Index index) const {
        if(index >= end_ids_.size()) {
            throw PoseException(
                    "trying to get end_id: " + std::to_string(index) + " there are only " +
                    std::to_string(end_ids_.size()));
        }
        return end_ids_[index];
    }

    int
    get_end_index(
            base::SimpleStringCOP name) const {
        auto & bp = get_end(name);
        int i = 0;
        for(auto const & end : ends_) {
            if(bp == end) { return i; }
            i++;
        }

    }

    int
    get_end_index(
            String const & end_id) const {
        int i = 0;
        for(auto const & ei : end_ids_) {
            if(ei->get_str() == end_id) { return i; }
            i++;
        }
        throw PoseException("cannot find end with end_id: " + end_id);


    }

    ResiduesOP
    get_bp_res(
            BPtype const & bp) const {
        auto res = std::vector<Restype>();
        res.push_back(get_residue(bp.get_res1_uuid()));
        res.push_back(get_residue(bp.get_res2_uuid()));
        return std::make_shared<base::VectorContainer<Restype>>(res);
    }

    size_t
    get_num_basepairs() const{ return basepairs_.size(); }

    size_t
    get_num_ends() const{ return ends_.size(); }

    base::SimpleStringCOP
    get_name() { return name_; }


protected:
    Structuretype structure_;
    std::vector<BPtype> basepairs_;
    std::vector<BPtype> ends_;
    base::SimpleStringCOPs end_ids_;
    base::SimpleStringCOP name_;

};

typedef Pose<PrimitiveBasepair, PrimitiveStructure, PrimitiveChain, PrimitiveResidue> PrimitivePose;
typedef std::shared_ptr<PrimitivePose> PrimitivePoseOP;


template<typename BPtype, typename Structuretype>
base::VectorContainerOP<BPtype>
get_ends_from_basepairs(
        Structuretype const & s,
        std::vector<BPtype> const & bps) {
    auto start_chain_end_uuids = std::vector<util::Uuid>();
    auto end_chain_end_uuids = std::vector<util::Uuid>();

    for(auto const & r : s) {
        if(s.is_residue_start_of_chain(r)) { start_chain_end_uuids.push_back(r.get_uuid()); }
        if(s.is_residue_end_of_chain(r))   { end_chain_end_uuids.push_back(r.get_uuid()); }
    }

    auto ends = std::vector<BPtype>();
    for(auto const & bp : bps) {
        if(bp.get_bp_type() == BasepairType::NC) { continue; }

        if     (std::find(start_chain_end_uuids.begin(), start_chain_end_uuids.end(), bp.get_res1_uuid()) != start_chain_end_uuids.end() &&
                std::find(end_chain_end_uuids.begin(), end_chain_end_uuids.end(), bp.get_res2_uuid()) != end_chain_end_uuids.end()) {
            ends.push_back(bp);
        }
        else if(std::find(start_chain_end_uuids.begin(), start_chain_end_uuids.end(), bp.get_res2_uuid()) != start_chain_end_uuids.end() &&
                std::find(end_chain_end_uuids.begin(), end_chain_end_uuids.end(), bp.get_res1_uuid()) != end_chain_end_uuids.end()) {
            ends.push_back(bp);
        }

    }

    return std::make_shared<base::VectorContainer<BPtype>>(ends);

}

template<typename BPtype, typename Restype>
BPtype const *
get_res_wc_or_gu_basepair(
        std::vector<BPtype> const & basepairs,
        std::vector<BPtype> const & ends,
        Restype const & r) {

    for(auto const & bp : basepairs) {
        if(bp.get_bp_type() == BasepairType::NC) { continue; }
        if(bp.get_res1_uuid() == r.get_uuid() ||
            bp.get_res2_uuid() == r.get_uuid()) {
            return &bp;
        }
    }

    for(auto const & bp : ends) {
        if(bp.get_bp_type() == BasepairType::NC) { continue; }
        if(bp.get_res1_uuid() == r.get_uuid() ||
            bp.get_res2_uuid() == r.get_uuid()) {
            return &bp;
        }
    }

    return nullptr;

}

template<typename Structuretype, typename Chaintype, typename BPtype, typename Restype>
String
generate_end_id(
        Structuretype const & s,
        std::vector<BPtype> const & bps,
        std::vector<BPtype> const & ends,
        BPtype const & end) {

    auto open_chains = std::vector<Chaintype const *>();
    auto chains = s.get_chains();
    for(auto const & c : *chains) {
        if(c.get_first().get_uuid() == end.get_res1_uuid() ||
           c.get_first().get_uuid() == end.get_res2_uuid()) {
            open_chains.push_back(&c);
            break;
        }
    }

    if(open_chains.size() == 0) {
        throw std::runtime_error("could not find chain to start with");
    }

    auto seen_res    = std::map<util::Uuid, int>();
    auto seen_bps    = std::map<BPtype const *, int>();
    auto seen_chains = std::map<Chaintype const *, int>();
    seen_chains[open_chains[0]] = 1;

    BPtype const * bp = nullptr;
    auto ss_chains = std::vector<Strings>();
    auto seq = String("");
    auto ss = String("");
    auto dot_bracket = ' ';

    auto best_chains = std::vector<Chaintype const *>();
    Chaintype const * best_chain;
    Chaintype const * c = nullptr;
    auto best_score = 0;
    auto score = 0;
    auto pos = 0;

    while( open_chains.size() > 0) {
        c = open_chains[0];
        open_chains.erase(open_chains.begin());

        for (auto const & r : *c) {
            dot_bracket = '.';
            bp = get_res_wc_or_gu_basepair(bps, ends, r);
            if (bp != nullptr && bp->get_bp_type() != BasepairType::NC) {
                auto & partner_res_uuid = bp->get_partner(r.get_uuid());
                auto & partner_res = s.get_residue(partner_res_uuid);
                if (seen_bps.find(bp) == seen_bps.end() &&
                    seen_res.find(r.get_uuid()) == seen_res.end() &&
                    seen_res.find(partner_res.get_uuid()) == seen_res.end()) {
                    seen_res[r.get_uuid()] = 1;
                    dot_bracket = '(';
                } else if (seen_res.find(partner_res.get_uuid()) != seen_res.end()) {
                    if (seen_res[partner_res.get_uuid()] > 1) { dot_bracket = '.'; }
                    else {
                        dot_bracket = ')';
                        seen_res[r.get_uuid()] = 1;
                        seen_res[partner_res.get_uuid()] += 1;
                    }
                }
            }
            ss += dot_bracket;
            seq += r.get_name();
            if (bp != nullptr) { seen_bps[bp] = 1; }
        }

        auto dummy_str = Strings(2);
        dummy_str[0] = seq; dummy_str[1] = ss;
        ss_chains.push_back(dummy_str);
        ss = "";
        seq = "";
        best_score = -1;
        best_chains = std::vector<Chaintype const *>();
        for(auto const & c : *chains) {
            if(seen_chains.find(&c) != seen_chains.end()) { continue; }
            score = 0;
            for(auto const & r : c) {
                bp = get_res_wc_or_gu_basepair(bps, ends, r);
                if(bp != nullptr && seen_bps.find(bp) == seen_bps.end()) { score += 1; }
            }
            if(score > best_score) { best_score = score; }
        }

        for(auto const & c: *chains) {
            if(seen_chains.find(&c) != seen_chains.end()) { continue; }
            score = 0;
            for(auto const & r : c) {
                bp = get_res_wc_or_gu_basepair(bps, ends, r);
                if(bp != nullptr && seen_bps.find(bp) == seen_bps.end()) { score += 1; }
            }
            if(score == best_score) { best_chains.push_back(&c);  }
        }

        best_chain = nullptr;
        best_score = 1000;
        for(auto const & c: best_chains) {
            auto i = -1;
            pos = 1000;
            for(auto const & r : *c) {
                i++;
                bp = get_res_wc_or_gu_basepair(bps, ends, r);
                if(bp != nullptr && seen_bps.find(bp) != seen_bps.end()) {
                    pos = i;
                    break;
                }
            }
            if(pos < best_score) {
                best_score = pos;
                best_chain = c;
            }
        }

        if(best_chain == nullptr) { break; }
        seen_chains[best_chain] = 1;
        open_chains.push_back(best_chain);
    }

    auto ss_id = String("");
    auto i = 0;
    for(auto const & ss_chain : ss_chains) {
        ss_id += ss_chain[0] + "_";
        for(auto const & e : ss_chain[1]) {
            if     (e == '(') {
                ss_id += "L";
            }
            else if(e == ')') {
                ss_id += "R";
            }
            else if(e == '.') {
                ss_id += "U";
            }
            else {
                throw PoseException(
                        "unexpected symbol in dot bracket notation: " + std::to_string(e));
            }
        }
        if(i != ss_chains.size()-1) { ss_id += "_"; }
        i++;
    }

    return ss_id;

};

template<typename Structuretype, typename Chaintype, typename BPtype, typename Restype>
String
generate_secondary_structure(
        Structuretype const & s,
        std::vector<BPtype> const & bps,
        std::vector<BPtype> const & ends) {

    auto open_chains = std::vector<Chaintype const *>();
    auto chains = s.get_chains();

    if(chains->size() == 0) { return String(""); }
    open_chains.push_back(&chains->get_data()[0]);

    auto seen_res    = std::map<util::Uuid, int>();
    auto seen_bps    = std::map<BPtype const *, int>();
    auto seen_chains = std::map<Chaintype const *, int>();
    seen_chains[open_chains[0]] = 1;

    BPtype const * bp = nullptr;
    auto seq = String("");
    auto ss = String("");
    auto dot_bracket = ' ';

    auto best_chains = std::vector<Chaintype const *>();
    Chaintype const * best_chain;
    Chaintype const * c = nullptr;
    auto best_score = 0;
    auto score = 0;
    auto pos = 0;
    auto final_ss = String("");

    while( open_chains.size() > 0) {
        c = open_chains[0];
        open_chains.erase(open_chains.begin());

        for (auto const & r : *c) {
            dot_bracket = '.';
            bp = get_res_wc_or_gu_basepair(bps, ends, r);
            if (bp != nullptr && bp->get_bp_type() != BasepairType::NC) {
                auto & partner_res_uuid = bp->get_partner(r.get_uuid());
                auto & partner_res = s.get_residue(partner_res_uuid);
                if (seen_bps.find(bp) == seen_bps.end() &&
                    seen_res.find(r.get_uuid()) == seen_res.end() &&
                    seen_res.find(partner_res.get_uuid()) == seen_res.end()) {
                    seen_res[r.get_uuid()] = 1;
                    dot_bracket = '(';
                } else if (seen_res.find(partner_res.get_uuid()) != seen_res.end()) {
                    if (seen_res[partner_res.get_uuid()] > 1) { dot_bracket = '.'; }
                    else {
                        dot_bracket = ')';
                        seen_res[r.get_uuid()] = 1;
                        seen_res[partner_res.get_uuid()] += 1;
                    }
                }
            }
            ss += dot_bracket;
            seq += r.get_name();
            if (bp != nullptr) { seen_bps[bp] = 1; }
        }

        if(final_ss.length() > 0) { final_ss += "&"; }
        final_ss += ss;

        ss = "";
        seq = "";
        best_score = -1;
        best_chains = std::vector<Chaintype const *>();
        for(auto const & c : *chains) {
            if(seen_chains.find(&c) != seen_chains.end()) { continue; }
            score = 0;
            for(auto const & r : c) {
                bp = get_res_wc_or_gu_basepair(bps, ends, r);
                if(bp != nullptr && seen_bps.find(bp) == seen_bps.end()) { score += 1; }
            }
            if(score > best_score) { best_score = score; }
        }

        for(auto const & c: *chains) {
            if(seen_chains.find(&c) != seen_chains.end()) { continue; }
            score = 0;
            for(auto const & r : c) {
                bp = get_res_wc_or_gu_basepair(bps, ends, r);
                if(bp != nullptr && seen_bps.find(bp) == seen_bps.end()) { score += 1; }
            }
            if(score == best_score) { best_chains.push_back(&c);  }
        }

        best_chain = nullptr;
        best_score = 1000;
        for(auto const & c: best_chains) {
            auto i = -1;
            pos = 1000;
            for(auto const & r : *c) {
                i++;
                bp = get_res_wc_or_gu_basepair(bps, ends, r);
                if(bp != nullptr && seen_bps.find(bp) != seen_bps.end()) {
                    pos = i;
                    break;
                }
            }
            if(pos < best_score) {
                best_score = pos;
                best_chain = c;
            }
        }

        if(best_chain == nullptr) { break; }
        seen_chains[best_chain] = 1;
        open_chains.push_back(best_chain);
    }

    return final_ss;

};

String
get_dot_bracket_from_end_id(
        String const &);

}

#endif //PRIMITIVES_RNA_STRUCTURE_H
