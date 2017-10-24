//
// Created by Joseph Yesselman on 2/15/17.
//

#include "rna_structure.h"

namespace primitives {
/*
PrimitiveBasepairCOP
get_res_wc_or_gu_basepair(
        PrimitiveBasepairCOPs const & basepairs,
        PrimitiveBasepairCOPs const & ends,
        PrimitiveResidueCOP r) {

    for(auto const & bp : basepairs) {
        if((bp->get_res1_uuid() == r->get_uuid() ||
            bp->get_res2_uuid() == r->get_uuid()) &&
           bp->get_bp_type() != BasepairType::NC) {
            return bp;
        }
    }

    for(auto const & bp : ends) {
        if((bp->get_res1_uuid() == r->get_uuid() ||
            bp->get_res2_uuid() == r->get_uuid()) &&
           bp->get_bp_type() != BasepairType::NC) {
            return bp;
        }
    }
    return nullptr;
}

String
generate_end_id(
        PrimitiveStructureCOP s,
        PrimitiveBasepairCOPs const & bps,
        PrimitiveBasepairCOPs const & ends,
        PrimitiveBasepairCOP  end) {

    auto open_chains = PrimitiveChainCOPs();
    auto chains = s->get_chains();
    for(auto const & c : chains) {
        if(c->get_first()->get_uuid() == end->get_res1_uuid() ||
           c->get_first()->get_uuid() == end->get_res2_uuid()) {
            open_chains.push_back(c);
            break;
        }
    }

    if(open_chains.size() == 0) {
        throw std::runtime_error("could not find chain to start with");
    }

    auto seen_res    = std::map<PrimitiveResidueCOP, int>();
    auto seen_bps    = std::map<PrimitiveBasepairCOP, int>();
    auto seen_chains = std::map<PrimitiveChainCOP, int>();
    seen_chains[open_chains[0]] = 1;

    auto saved_bp = PrimitiveBasepairCOP();
    auto bp = PrimitiveBasepairCOP();
    auto ss_chains = std::vector<Strings>();
    auto seq = String("");
    auto ss = String("");
    auto dot_bracket = ' ';

    auto best_chains = PrimitiveChainCOPs();
    auto best_chain = PrimitiveChainCOP();
    auto c = PrimitiveChainCOP();
    auto r = PrimitiveResidueCOP();
    auto best_score = 0;
    auto score = 0;
    auto pos = 0;

    while( open_chains.size() > 0) {
        c = open_chains[0];
        open_chains.erase(open_chains.begin());

        for(auto i = 0; i < c->get_length(); i++) {
            r = c->get_residue(i);
            dot_bracket = '.';
            bp = get_res_wc_or_gu_basepair(bps, ends, r);
            if(bp != nullptr && bp->get_bp_type() != BasepairType::NC) {
                saved_bp = bp;
                auto partner_res_uuid = bp->get_partner(r->get_uuid());
                auto partner_res = s->get_residue(partner_res_uuid);
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
            seq += r->get_name();
            if(saved_bp != nullptr ) { seen_bps[saved_bp] = 1; }
        }

        auto dummy_str = Strings(2);
        dummy_str[0] = seq; dummy_str[1] = ss;
        ss_chains.push_back(dummy_str);
        ss = "";
        seq = "";
        best_score = -1;
        best_chains = PrimitiveChainCOPs();
        for(auto const & c : chains) {
            if(seen_chains.find(c) != seen_chains.end()) { continue; }
            score = 0;
            for(auto const & r : *c) {
                bp = get_res_wc_or_gu_basepair(bps, ends, r);
                if(bp != nullptr && seen_bps.find(bp) == seen_bps.end()) { score += 1; }
            }
            if(score > best_score) { best_score = score; }
        }

        for(auto const & c: chains) {
            if(seen_chains.find(c) != seen_chains.end()) { continue; }
            score = 0;
            for(auto const & r : *c) {
                bp = get_res_wc_or_gu_basepair(bps, ends, r);
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
                throw RNAStructureException(
                        "unexpected symbol in dot bracket notation: " + std::to_string(e));
            }
        }
        if(i != ss_chain.size()-1) { ss_id += "_"; }
        i++;
    }

    return String("");
    //return ss_id;
}*/

}

