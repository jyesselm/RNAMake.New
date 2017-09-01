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
            SimpleStringOPs const & end_ids,
            SimpleStringOP const & name) :
            structure_(structure),
            basepairs_(basepairs),
            ends_(ends),
            end_ids_(end_ids),
            name_(name) {}

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
    ResidueOP const
    get_residue(
            int num,
            char chain_id,
            char i_code) const {
        return structure_->get_residue(num, chain_id, i_code); }

    inline
    ResidueOP const
    get_residue(
            Uuid const & uuid) const {
        return structure_->get_residue(uuid); }

    inline
    ResidueOP const &
    get_residue(
            int index) const {
        return structure_->get_residue(index); }

    inline
    size_t
    get_num_residues() const { return structure_->num_residues(); }

    inline
    size_t
    get_num_chains() const { return structure_->num_chains(); }

public: //get basepairs interface

    BasepairOPs
    get_basepairs(Uuid const & bp_uuid) {
        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_uuic() == bp_uuid) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == bp_uuid || bp->get_res2_uuid() == bp_uuid) { bps.push_back(bp); }

        }
        return bps;
    }

    BasepairOPs
    get_basepairs(
            Uuid const & uuid1,
            Uuid const & uuid2) {

        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_res1_uuid() == uuid1 && bp->get_res2_uuid() == uuid2) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == uuid2 && bp->get_res2_uuid() == uuid1) { bps.push_back(bp); }
        }
        return bps;
    }

    BasepairOPs
    get_basepairs(
            String const & name) {
        for (auto const & bp : basepairs_) {
            if (name == bp->get_name()) { return BasepairOPs{bp}; }
        }

        throw RNAStructureException("could not find basepair with name " + name);

    }


public: // get basepair interface  (single basepair!)

    BasepairOP
    get_basepair(Uuid const & bp_uuid) {
        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_uuid() == bp_uuid) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == bp_uuid || bp->get_res2_uuid() == bp_uuid) { bps.push_back(bp); }

        }
        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this uuid");
        }
        if (bps.size() == 0) { return bps[0]; }
        else { return BasepairOP(nullptr); }
    }

    BasepairOP
    get_basepair(
            Uuid const & uuid1,
            Uuid const & uuid2) {

        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_res1_uuid() == uuid1 && bp->get_res2_uuid() == uuid2) { bps.push_back(bp); }
            if (bp->get_res1_uuid() == uuid2 && bp->get_res2_uuid() == uuid1) { bps.push_back(bp); }
        }
        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching residue uuids");
        }
        if (bps.size() == 0) { return bps[0]; }
        else { return BasepairOP(nullptr); }
    }

    BasepairOP
    get_basepair(String const & name) {
        BasepairOPs bps;
        for (auto const & bp : basepairs_) {
            if (bp->get_name().get_str() == name) { bps.push_back(bp); }
        }

        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this name: " + name);
        }
        if (bps.size() == 0) { return bps[0]; }
        else { return BasepairOP(nullptr); }

    }

    inline
    BasepairOP const &
    get_basepair(int index) const { return basepairs_[index]; }


public: // get end interace

    BasepairOP
    get_end(Uuid const & bp_uuid) {
        BasepairOPs bps;
        for (auto const & bp : ends_) {
            if (bp->uuid() == bp_uuid) { bps.push_back(bp); }
            if (bp->res1()->uuid() == bp_uuid || bp->res2()->uuid() == bp_uuid) { bps.push_back(bp); }

        }
        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this uuid");
        }
        if (bps.size() == 0) { return bps[0]; }
        else { return BasepairOP(nullptr); }
    }

    BasepairOP
    get_end(
            Uuid const & uuid1,
            Uuid const & uuid2) {

        BasepairOPs bps;
        for (auto const & bp : ends_) {
            if (bp->res1()->uuid() == uuid1 && bp->res2()->uuid() == uuid2) { bps.push_back(bp); }
            if (bp->res1()->uuid() == uuid2 && bp->res2()->uuid() == uuid1) { bps.push_back(bp); }
        }
        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching residue uuids");
        }
        if (bps.size() == 0) { return bps[0]; }
        else { return BasepairOP(nullptr); }
    }

    BasepairOP
    get_end(SimpleString const & name) {
        BasepairOPs bps;
        for (auto const & bp : ends_) {
            if (bp->name()== name) { bps.push_back(bp); }
        }

        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this name: " + name.get_str());
        }
        if (bps.size() == 0) { return bps[0]; }
        else { return BasepairOP(nullptr); }
    }

    BasepairOP
    get_end(String const & end_id) {
        BasepairOPs bps;
        int i = -1;
        for (auto const & bp : ends_) {
            i++;
            if (end_ids_[i]->to_str() == end_id) { bps.push_back(ends_[i]); }
        }

        if (bps.size() > 1) {
            throw RNAStructureException("got more than one basepair matching this end_id: " + end_id);
        }
        if (bps.size() == 0) { return bps[0]; }
        else { return BasepairOP(nullptr); }
    }

    inline
    BasepairOP const &
    get_end(int index) const { return ends_[index]; }

public: // other getters

    inline
    SimpleString const &
    get_end_id(int index) { return *end_ids_[index]; }

    int
    get_end_index(SimpleString const & name) {
        auto bp = get_end(name);
        if(bp == nullptr) { throw RNAStructureException("cannot find end with name: " + name.get_str()); }
        int i = 0;
        for(auto const & end : ends_) {
            if(bp == end) { return i; }
            i++;
        }

    }

    int
    get_end_index(String const & end_id) {
        int i = 0;
        for(auto const & ei : end_ids_) {
            if(ei->to_str() == end_id) { return i; }
            i++;
        }
        throw RNAStructureException("cannot find end with end_id: " + end_id);


    }

    ResidueOPs
    get_bp_res(BPtype const & bp) const {
        return ResidueOPs({get_residue(bp.res1_uuid()),
                           get_residue(bp.res2_uuid())});
    }

    size_t
    get_num_basepairs() const{ return basepairs_.size(); }

    size_t
    get_num_ends() const{ return ends_.size(); }

    SimpleString const &
    get_name() { return *name_; }

protected:
    std::shared_ptr<Structuretype> structure_;
    std::vector<std::shared_ptr<BPtype>> basepairs_;
    std::vector<std::shared_ptr<BPtype>> ends_;
    SimpleStringOPs end_ids_;
    SimpleStringOP name_;

};


template<typename BPtype, typename Structuretype>
std::vector<std::shared_ptr<BPtype>>
ends_from_basepairs(
        Structuretype const & s,
        std::vector<std::shared_ptr<BPtype>> const & bps) {
    auto chain_end_uuids = std::vector<Uuid>();
    for(auto const & c : s.get_chains()) {
        chain_end_uuids.push_back(c->first()->uuid());
        if(c->length() > 1) {
            chain_end_uuids.push_back(c->last()->uuid());
        }
    }

    auto ends = std::vector<std::shared_ptr<BPtype>>();
    for(auto const & bp : bps) {
        if(bp->bp_type() == BasepairType::NC) { continue; }
        if(std::find(chain_end_uuids.begin(), chain_end_uuids.end(), bp->res1_uuid()) == chain_end_uuids.end())  { continue; }
        if(std::find(chain_end_uuids.begin(), chain_end_uuids.end(), bp->res2_uuid()) == chain_end_uuids.end())  { continue; }
        ends.push_back(bp);
    }

    return ends;

}

template<typename BPtype, typename Restype>
std::shared_ptr<BPtype>
get_res_wc_or_gu_basepair(
        std::vector<std::shared_ptr<BPtype>> const & basepairs,
        std::vector<std::shared_ptr<BPtype>> const & ends,
        Restype const & r) {

    for(auto const & bp : basepairs) {
        if((bp->res1_uuid() == r.uuid() || bp->res2_uuid() == r.uuid()) &&
            bp->bp_type() != BasepairType::NC) {
            return bp;
        }
    }

    for(auto const & bp : ends) {
        if((bp->res1_uuid() == r.uuid() || bp->res2_uuid() == r.uuid()) &&
           bp->bp_type() != BasepairType::NC) {
            return bp;
        }
    }
    return nullptr;
}

template<typename BPtype, typename Structuretype, typename Chaintype, typename Restype>
String
assign_end_id(
        Structuretype const & s,
        std::vector<std::shared_ptr<BPtype>> const & bps,
        std::vector<std::shared_ptr<BPtype>> const & ends,
        std::shared_ptr<BPtype> const & end) {

    auto open_chains = std::vector<std::shared_ptr<Chaintype>>();
    auto chains = s.get_chains();
    for(auto const & c : chains) {
    if(c->first()->uuid() == end->res1_uuid() || c->first()->uuid() == end->res2_uuid()) {
            open_chains.push_back(c);
            break;
        }
    }

    if(open_chains.size() == 0) {
        throw std::runtime_error("could not find chain to start with");
    }

    auto seen_res    = std::map<std::shared_ptr<Restype>, int>();
    auto seen_bps    = std::map<std::shared_ptr<BPtype>, int>();
    auto seen_chains = std::map<std::shared_ptr<Chaintype>, int>();
    seen_chains[open_chains[0]] = 1;

    auto saved_bp = std::shared_ptr<BPtype>(nullptr);
    auto bp = std::shared_ptr<BPtype>(nullptr);
    auto ss_chains = std::vector<Strings>();
    auto seq = String("");
    auto ss = String("");
    auto dot_bracket = ' ';

    auto best_chains = std::vector<std::shared_ptr<Chaintype>>();
    auto best_chain = std::shared_ptr<Chaintype>();
    auto c = std::shared_ptr<Chaintype>();
    auto best_score = 0;
    auto score = 0;
    auto pos = 0;

    while( open_chains.size() > 0) {
        c = open_chains[0];
        open_chains.erase(open_chains.begin());

        for(auto const & r: *c) {
            dot_bracket = '.';
            bp = get_res_wc_or_gu_basepair<BPtype, Restype>(bps, ends, *r);
            if(bp != nullptr && bp->bp_type() != BasepairType::NC) {
                saved_bp = bp;
                auto partner_res_uuid = bp->partner(r->uuid());
                auto partner_res = s.get_residue(partner_res_uuid);
                if(seen_bps.find(bp) == seen_bps.end() &&
                   seen_res.find(r) == seen_res.end() &&
                   seen_res.find(partner_res) == seen_res.end()) {
                    seen_res[r] = 1;
                    dot_bracket = '(';
                }
                else if(seen_res.find(partner_res) != seen_res.end()) {
                    if(seen_res[partner_res] > 1)  { dot_bracket = '.'; }
                    else {
                        dot_bracket = ')';
                        seen_res[r] = 1;
                        seen_res[partner_res] += 1;
                    }
                }
            }
            ss += dot_bracket;
            seq += r->name();
            if(saved_bp != nullptr ) { seen_bps[saved_bp] = 1; }
        }

        ss_chains.push_back(Strings{seq, ss});
        ss = "";
        seq = "";
        best_score = -1;
        best_chains = std::vector<std::shared_ptr<Chaintype>>();
        for(auto const & c : chains) {
            if(seen_chains.find(c) != seen_chains.end()) { continue; }
            score = 0;
            for(auto const & r : *c) {
                bp = get_res_wc_or_gu_basepair<BPtype, Restype>(bps, ends, *r);
                if(bp != nullptr && seen_bps.find(bp) == seen_bps.end()) { score += 1; }
            }
            if(score > best_score) { best_score = score; }
        }

        for(auto const & c: chains) {
            if(seen_chains.find(c) != seen_chains.end()) { continue; }
            score = 0;
            for(auto const & r : *c) {
                bp = get_res_wc_or_gu_basepair<BPtype, Restype>(bps, ends, *r);
                if(bp != nullptr && seen_bps.find(bp) == seen_bps.end()) { score += 1; }
            }
            if(score == best_score) { best_chains.push_back(c);  }
        }

        best_chain = nullptr;
        best_score = 1000;
        for(auto const & c: best_chains) {
            auto i = -1;
            pos = 1000;
            for(auto const & r : *c) {
                i++;
                bp = get_res_wc_or_gu_basepair<BPtype, Restype>(bps, ends, *r);
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
                throw RNAStructureException("unexpected symbol in dot bracket notation: " + std::to_string(e));
            }
        }
        if(i != ss_chain.size()-1) { ss_id += "_"; }
        i++;
    }

    return ss_id;
}

Strings
end_id_to_seq_and_db(String const & ss_id) {
    auto ss = String("");
    auto seq = String("");
    auto spl = split_str_by_delimiter(ss_id, "_");
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

    return Strings({seq, ss});
}


}

#endif //PRIMITIVES_RNA_STRUCTURE_H
