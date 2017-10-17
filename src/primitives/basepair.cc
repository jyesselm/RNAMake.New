//
// Created by Joseph Yesselman on 1/29/17.
//


#include <primitives/basepair.h>

namespace primitives {

util::Uuid const &
Basepair::get_partner(util::Uuid const & r_uuid) {
    if     (r_uuid == res1_uuid_) { return res2_uuid_; }
    else if(r_uuid == res2_uuid_) { return res1_uuid_; }
    else {
        throw BasepairException("called partner with uuid that does not exist in basepair");
    }
}

String
generate_bp_name(
        ResidueOP const & res1,
        ResidueOP const & res2) {

    auto res1_name = String("");
    auto res2_name = String("");

    if(res1->get_i_code() == ' ') {
        res1_name = res1->get_chain_id()+std::to_string(res1->get_num());
    }
    else {
        res1_name = res1->get_chain_id()+std::to_string(res1->get_num())+res1->get_i_code();

    }

    if(res2->get_i_code() == ' ') {
        res2_name = res2->get_chain_id()+std::to_string(res2->get_num());
    }
    else {
        res2_name = res2->get_chain_id()+std::to_string(res2->get_num())+res2->get_i_code();
    }

    if(res1->get_chain_id() < res2->get_chain_id()) { return res1_name+"-"+res2_name; }
    if(res2->get_chain_id() < res1->get_chain_id()) { return res2_name+"-"+res1_name; }

    if(res1->get_num() < res2->get_num()) { return res1_name+"-"+res2_name; }
    else                                  { return res2_name+"-"+res1_name; }

}

}