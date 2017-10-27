//
// Created by Joseph Yesselman on 10/26/17.
//

#ifndef RNAMAKE_NEW_BASEPAIR_H
#define RNAMAKE_NEW_BASEPAIR_H

#include <base/assertions.h>
#include <base/string.h>
#include <primitives/basepair.h>

namespace secondary_structure {

class Basepair : public primitives::Basepair {
public:
    inline
    Basepair(
            util::Uuid const & res1_uuid,
            util::Uuid const & res2_uuid,
            util::Uuid const & uuid,
            primitives::BasepairType const & bp_type,
            base::SimpleStringCOP const & name):
            primitives::Basepair(res1_uuid, res2_uuid, uuid, bp_type, name) {}

    inline
    Basepair(
            Basepair const & bp): primitives::Basepair(bp) {}

    inline
    Basepair(
            String const & s,
            util::Uuid const & res1_uuid,
            util::Uuid const & res2_uuid,
            util::Uuid const & uuid):
            primitives::Basepair(res1_uuid, res2_uuid, uuid) {
        auto spl = base::split_str_by_delimiter(s, " ");
        expects<BasepairException>(
                spl.size() == 2,
                "basepair string requires two elements");
        bp_type_ = static_cast<primitives::BasepairType>(std::stoi(spl[0]));
        name_    = std::make_shared<base::SimpleString>(spl[1]);
    }

    virtual
    ~Basepair() {}

public:
    inline
    bool
    operator == (
            Basepair const & bp) const {
        if(res1_uuid_ != bp.res1_uuid_) { return false; }
        if(res2_uuid_ != bp.res2_uuid_) { return false; }
        if(uuid_ != bp.uuid_) { return false; }
        if(*name_ != *bp.name_) { return false; }
        if(bp_type_ != bp.bp_type_) { return false; }
        return true;
    }

    inline
    bool
    operator != (
            Basepair const & bp) const {
        return !(*this == bp);
    }

public:

    inline
    bool
    is_equal(
            Basepair const & bp,
            bool check_uuid = true) const {
        if(check_uuid && res1_uuid_ != bp.res1_uuid_) { return false; }
        if(check_uuid && res2_uuid_ != bp.res2_uuid_) { return false; }
        if(check_uuid && uuid_ != bp.uuid_) { return false; }
        if(*name_ != *bp.name_) { return false; }
        if(bp_type_ != bp.bp_type_) { return false; }
        return true;
    }

public:
    String
    get_str() const {
        return std::to_string(bp_type_) + " " + name_->get_str();
    }

};

typedef std::vector<Basepair> Basepairs;

}

#endif //RNAMAKE_NEW_BASEPAIR_H
