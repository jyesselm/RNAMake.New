//
// Created by Joseph Yesselman on 12/20/17.
//

#ifndef RNAMAKE_NEW_ALL_ATOM_SEGMENT_H
#define RNAMAKE_NEW_ALL_ATOM_SEGMENT_H

#include <util/segment_type.h>
#include <primitives/segment.h>
#include <all_atom/structure.h>
#include <all_atom/basepair.h>

// forward declaration to allow SegmentSqliteLibraryt to call new_uuids()
namespace resource_management {
    class SegmentSqliteLibrary;
}

namespace all_atom {

class Segment : public primitives::Segment<Basepair, Structure, Chain, Residue>  {
public:
    typedef primitives::Segment<Basepair, Structure, Chain, Residue> BaseClass;

public:
    friend class SegmentFactory;
    friend class resource_management::SegmentSqliteLibrary;

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
            json::JSON & j,
            ResidueTypeSet const & rts):
            BaseClass(),
            proteins_(Structure(j["proteins"], rts)),
            small_molecules_(Structure(j["small_molecules"], rts)) {

        uuid_ = util::Uuid();
        structure_ = Structure(j["structure"], rts);
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
            basepairs_.push_back(Basepair(j_bps[0], r1.get_uuid(), r2.get_uuid(), util::Uuid()));
        }

        for(int i = 0; i < j_end_indexes.size(); i++) {
            end_indexes_.push_back(j_end_indexes[i].ToInt());
        }

        for(int i = 0; i < j_end_ids.size(); i++) {
            end_ids_.push_back(std::make_shared<base::SimpleString const>(j_end_ids[i].ToString()));
        }
    }

public:
    bool
    is_equal(
            Segment const & s,
            bool check_uuid = true) const {
        if(segment_type_ != s.segment_type_) { return false; }
        if(aligned_end_index_ != s.aligned_end_index_) { return false; }
        if(basepairs_.size() != s.basepairs_.size()) { return false; }
        if(end_indexes_.size() != s.end_indexes_.size()) { return false; }
        if(*name_ != *s.name_) { return false; }
        if(*dot_bracket_ != *s.dot_bracket_) { return false; }
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

public:
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

public:
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

    String
    get_pdb_str(
            int &,
            int &,
            char &);

    inline
    String
    get_pdb_str(
            int acount = 0) {
        auto num = structure_.get_residue(0).get_num();
        auto chain_id = structure_.get_residue(0).get_chain_id();
        return get_pdb_str(acount, num, chain_id);
    }

    void
    write_pdb(
            String const &) const;

    void
    write_steric_beads_to_pdb(
            String const &);

public:
    inline
    String
    dot_bracket() { return dot_bracket_->get_str(); }

    inline
    Basepair const &
    get_aligned_end() { return basepairs_[end_indexes_[aligned_end_index_]]; }

protected:
    void
    new_uuids() {
        uuid_ = util::Uuid();
        auto uuid_map = std::map<util::Uuid, int>();
        int i = 0;
        for(auto const & r : structure_) {
            uuid_map[r.get_uuid()] = i;
            i++;
        }
        structure_.new_uuids();

        for(auto & bp : basepairs_) {
            auto r1_pos = uuid_map[bp.get_res1_uuid()];
            auto r2_pos = uuid_map[bp.get_res2_uuid()];
            auto & r1 = structure_.get_residue(r1_pos);
            auto & r2 = structure_.get_residue(r2_pos);
            bp.new_uuids(r1.get_uuid(), r2.get_uuid());
        }

        proteins_.new_uuids();
        small_molecules_.new_uuids();
    }

protected:
    Structure proteins_;
    Structure small_molecules_;
    base::SimpleStringCOP dot_bracket_;


};

typedef std::shared_ptr<Segment> SegmentOP;
typedef std::vector<SegmentOP>   SegmentOPs;


}


#endif //RNAMAKE_NEW_ALL_ATOM_SEGMENT_H
