//
// Created by Joseph Yesselman on 2/11/19.
//

#ifndef RNAMAKE_NEW_STATE_SEGMENT_H
#define RNAMAKE_NEW_STATE_SEGMENT_H

#include <util/segment_type.h>
#include <primitives/segment.h>
#include <secondary_structure/segment.h>
#include <state/structure.h>
#include <state/basepair.h>

namespace state {

class Segment : public primitives::Segment<Basepair, Structure, Chain, Residue>  {
public:
    typedef primitives::Segment<Basepair, Structure, Chain, Residue> BaseClass;

public:
    Segment(
            Structure const & structure,
            Basepairs const & basepairs,
            Indexes const & end_indexes,
            base::SimpleStringCOPs const & end_ids,
            base::SimpleStringCOP name,
            Structure const & proteins,
            Structure const & small_molecules,
            base::SimpleStringCOP dot_bracket,
            util::SegmentType segment_type,
            Index aligned_end_index,
            util::Uuid const & uuid ):
            BaseClass(structure, basepairs, end_indexes, end_ids, name, segment_type, aligned_end_index, uuid),
            proteins_(proteins),
            small_molecules_(small_molecules),
            dot_bracket_(dot_bracket) {}

    Segment(
            Segment const & seg):
            BaseClass(seg.structure_, seg.basepairs_, seg.end_indexes_, seg.end_ids_, seg.name_,
                      seg.segment_type_, seg.aligned_end_index_, seg.uuid_),
            proteins_(seg.proteins_),
            small_molecules_(seg.small_molecules_),
            dot_bracket_(seg.dot_bracket_) {}

    Segment(
            json::JSON & j):
            BaseClass(),
            proteins_(Structure(j["proteins"])),
            small_molecules_(Structure(j["small_molecules"])) {

        uuid_ = util::Uuid();
        structure_ = Structure(j["structure"]);
        name_ = std::make_shared<base::SimpleString const>(j["name"].ToString());
        dot_bracket_ = std::make_shared<base::SimpleString const>(j["dot_bracket"].ToString());
        segment_type_ = static_cast<util::SegmentType>(j["segment_type"].ToInt());
        aligned_end_index_ = j["aligned_end_index"].ToInt();
        basepairs_ = Basepairs();
        end_indexes_ = Indexes();
        end_ids_ = base::SimpleStringCOPs();

        auto & j_bps = j["basepairs"];
        auto & j_end_indexes = j["end_indexes"];
        auto & j_end_ids = j["end_ids"];

        for(int i = 0; i < j_bps.size(); i++) {
            auto & r1 = get_residue(j_bps[i][1].ToInt(), (char)j_bps[i][2].ToInt(), (char)j_bps[i][3].ToInt());
            auto & r2 = get_residue(j_bps[i][4].ToInt(), (char)j_bps[i][5].ToInt(), (char)j_bps[i][6].ToInt());
            basepairs_.push_back(Basepair(j_bps[i][0], r1.get_uuid(), r2.get_uuid(), util::Uuid()));
        }

        for(int i = 0; i < j_end_indexes.size(); i++) {
            end_indexes_.push_back(j_end_indexes[i].ToInt());
        }

        for(int i = 0; i < j_end_ids.size(); i++) {
            end_ids_.push_back(std::make_shared<base::SimpleString const>(j_end_ids[i].ToString()));
        }
    }

public:

    const_iterator protein_begin() const { return proteins_.begin(); }
    const_iterator protein_end()   const { return proteins_.end(); }

    inline
    bool
    is_protein_residue_start_of_chain(
            Residue const & r) const {
        return proteins_.is_residue_start_of_chain(r);
    }

    const_iterator small_molecules_begin() const { return small_molecules_.begin(); }
    const_iterator small_molecules_end()   const { return small_molecules_.end(); }


public:
    inline
    bool
    operator ==(Segment const & p) const {
        return is_equal(p);
    }

    inline
    bool
    operator !=(Segment const & p) const  {
        return !is_equal(p);
    }

public:
    bool
    is_equal(
            Segment const & s,
            CheckUUID check_uuid = CheckUUID::YES) const {
        if(segment_type_ != s.segment_type_) { return false; }
        if(aligned_end_index_ != s.aligned_end_index_) { return false; }
        if(basepairs_.size() != s.basepairs_.size()) { return false; }
        if(end_indexes_.size() != s.end_indexes_.size()) { return false; }
        if(*name_ != *s.name_) { return false; }
        if(*dot_bracket_ != *s.dot_bracket_) { return false; }
        for(int i = 0; i < basepairs_.size(); i++) {
            if(!basepairs_[i].is_equal(s.basepairs_[i], check_uuid)) { return false; }
        }
        for(int i = 0; i < end_indexes_.size(); i++) {
            if(end_indexes_[i] != s.end_indexes_[i]) { return false; }
        }
        if(! structure_.is_equal(s.structure_, check_uuid)) { return false; }
        if(! proteins_.is_equal(s.proteins_, check_uuid)) { return false; }
        if(! small_molecules_.is_equal(s.small_molecules_, check_uuid)) { return false; }
        return true;
    }

public: // non const methods
    void
    move(
            math::Point const & p) {
        structure_.move(p);
        proteins_.move(p);
        small_molecules_.move(p);
        for(auto & bp : basepairs_) { bp.move(p); }

    }

    void
    transform(
            math::Matrix const & r,
            math::Vector const & t,
            math::Point & dummy) {
        structure_.transform(r, t, dummy);
        proteins_.transform(r, t, dummy);
        small_molecules_.transform(r, t, dummy);
        for(auto & bp : basepairs_) { bp.transform(r, t, dummy); }
    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t) {
        auto dummy = math::Point();
        transform(r, t, dummy);
    }

public: //sterics
    bool
    steric_clash(
            Segment const & s) {

        for(auto const & r1 : *this) {
            // RNA/RNA clashes
            for(auto const & r2 : s) {
                if(residue_steric_clash_RNA(r1, r2)) { return true; }
            }
            // RNA/protein clashes
            for(auto const & r2 : s.proteins_) {
                if(residue_steric_clash_RNA(r1, r2)) { return true; }
            }
            // RNA/small molecule clashes
            for(auto const & r2 : s.small_molecules_) {
                if(residue_steric_clash_RNA(r1, r2)) { return true; }
            }
        }

        for(auto const & r1 : proteins_) {
            // protein/RNA clashes
            for(auto const & r2 : s) {
                if(residue_steric_clash_RNA(r1, r2)) { return true; }
            }
            // protein/protein clashes
            for(auto const & r2 : s.proteins_) {
                if(residue_steric_clash(r1, r2)) { return true; }
            }
            // protein/small molecule clashes
            for(auto const & r2 : s.small_molecules_) {
                if(residue_steric_clash(r1, r2)) { return true; }
            }
        }

        for(auto const & r1 : small_molecules_) {
            // small_molecule/RNA clashes
            for(auto const & r2 : s) {
                if(residue_steric_clash_RNA(r1, r2)) { return true; }
            }
            // small_molecule/protein clashes
            for(auto const & r2 : s.proteins_) {
                if(residue_steric_clash(r1, r2)) { return true; }
            }
            // small_molecule/small molecule clashes
            for(auto const & r2 : s.small_molecules_) {
                if(residue_steric_clash(r1, r2)) { return true; }
            }
        }
        return false;
    }

    int
    get_num_steric_clashes(
            Segment const & s) {
        int steric_clash_count = 0;

        for(auto const & r1 : *this) {
            // RNA/RNA clashes
            for(auto const & r2 : s) {
                if(residue_steric_clash_RNA(r1, r2)) { steric_clash_count += 1; }
            }
            // RNA/protein clashes
            for(auto const & r2 : s.proteins_) {
                if(residue_steric_clash_RNA(r1, r2)) { steric_clash_count += 1;  }
            }
            // RNA/small molecule clashes
            for(auto const & r2 : s.small_molecules_) {
                if(residue_steric_clash_RNA(r1, r2)) { steric_clash_count += 1;  }
            }
        }

        for(auto const & r1 : proteins_) {
            // protein/RNA clashes
            for(auto const & r2 : s) {
                if(residue_steric_clash_RNA(r1, r2)) { steric_clash_count += 1;  }
            }
            // protein/protein clashes
            for(auto const & r2 : s.proteins_) {
                if(residue_steric_clash(r1, r2)) { steric_clash_count += 1;  }
            }
            // protein/small molecule clashes
            for(auto const & r2 : s.small_molecules_) {
                if(residue_steric_clash(r1, r2)) { steric_clash_count += 1;  }
            }
        }

        for(auto const & r1 : small_molecules_) {
            // small_molecule/RNA clashes
            for(auto const & r2 : s) {
                if(residue_steric_clash_RNA(r1, r2)) { steric_clash_count += 1;  }
            }
            // small_molecule/protein clashes
            for(auto const & r2 : s.proteins_) {
                if(residue_steric_clash(r1, r2)) { steric_clash_count += 1;  }
            }
            // small_molecule/small molecule clashes
            for(auto const & r2 : s.small_molecules_) {
                if(residue_steric_clash(r1, r2)) { steric_clash_count += 1;  }
            }
        }

        return steric_clash_count;
    }

public: // outputing functions

    json::JSON
    get_json() const {
        auto j_bps = json::Array();
        auto j_end_indexes = json::Array();
        auto j_end_ids = json::Array();

        for(auto const & bp : basepairs_) {
            auto bp_res = get_bp_res(bp);
            j_bps.append(json::Array(bp.get_json(), bp_res->at(0).get_num(), bp_res->at(0).get_chain_id(),
                                     bp_res->at(0).get_i_code(), bp_res->at(1).get_num(),
                                     bp_res->at(1).get_chain_id(), bp_res->at(1).get_i_code()));
        }

        for(auto const & end_id : end_ids_) { j_end_ids.append(end_id->get_str()); }
        for(auto const & ei : end_indexes_) { j_end_indexes.append(ei); }

        return json::JSON{
                "structure", structure_.get_json(),
                "basepairs", j_bps,
                "end_indexes", j_end_indexes,
                "end_ids", j_end_ids,
                "name", name_->get_str(),
                "proteins", proteins_.get_json(),
                "small_molecules", small_molecules_.get_json(),
                "dot_bracket", dot_bracket_->get_str(),
                "segment_type", (int)segment_type_,
                "aligned_end_index", aligned_end_index_};
    }

    secondary_structure::SegmentOP
    get_secondary_structure() const {
        auto dot_bracket_spl = base::split_str_by_delimiter(dot_bracket_->get_str(), "&");
        auto dot_bracket_str = base::join_by_delimiter(dot_bracket_spl, "");

        auto i = 0 ;
        auto residues = secondary_structure::Residues();
        auto cutpoints = structure_.get_cutpoints();
        for(auto const & r : structure_) {
            auto ss_r = secondary_structure::Residue(r.get_name(), dot_bracket_str[i], r.get_num(),
                                                     r.get_chain_id(), r.get_i_code(), r.get_uuid());
            residues.push_back(ss_r);
            i += 1;
        }

        auto ss_structure = secondary_structure::Structure(residues, cutpoints);
        auto ss_bps = secondary_structure::Basepairs();
        for(auto const & bp : basepairs_) {
            auto ss_bp = secondary_structure::Basepair(
                    bp.get_res1_uuid(), bp.get_res2_uuid(), bp.get_uuid(), bp.get_bp_type(), bp.get_name());

            ss_bps.push_back(ss_bp);
        }

        return std::make_shared<secondary_structure::Segment>(
                ss_structure, ss_bps, end_indexes_, end_ids_, name_, segment_type_, aligned_end_index_, uuid_);
    }

public:
    inline
    String
    get_dot_bracket() { return dot_bracket_->get_str(); }

    inline
    Basepair const &
    get_aligned_end() { return basepairs_[end_indexes_[aligned_end_index_]]; }

protected:
    Structure proteins_;
    Structure small_molecules_;
    base::SimpleStringCOP dot_bracket_;
};

typedef std::shared_ptr<Segment> SegmentOP;
typedef std::vector<SegmentOP>   SegmentOPs;


}


#endif //RNAMAKE_NEW_STATE_SEGMENT_H
