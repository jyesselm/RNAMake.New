//
// Created by Joseph Yesselman on 12/20/17.
//

#ifndef RNAMAKE_NEW_ALL_ATOM_SEGMENT_H
#define RNAMAKE_NEW_ALL_ATOM_SEGMENT_H

#include <util/segment_type.h>
#include <primitives/segment.h>
#include <all_atom/structure.h>
#include <all_atom/basepair.h>

namespace all_atom {

class Segment : public primitives::Segment<Basepair, Structure, Chain, Residue>  {
public:
    typedef primitives::Segment<Basepair, Structure, Chain, Residue> BaseClass;

public:
    friend class SegmentFactory;

public:
    Segment(
            Structure const & structure,
            Basepairs const & basepairs,
            Basepairs const & ends,
            base::SimpleStringCOPs const & end_ids,
            base::SimpleStringCOP name,
            Structure const & proteins,
            Structure const & small_molecules,
            base::SimpleStringCOP dot_bracket,
            util::SegmentType segment_type,
            Index aligned_end):
            BaseClass(structure, basepairs, ends, end_ids, name, segment_type, aligned_end),
            proteins_(proteins),
            small_molecules_(small_molecules),
            dot_bracket_(dot_bracket) {}

    Segment(
            Segment const & seg):
            BaseClass(seg.structure_, seg.basepairs_, seg.ends_, seg.end_ids_, seg.name_,
                      seg.segment_type_, seg.aligned_end_),
            proteins_(seg.proteins_),
            small_molecules_(seg.small_molecules_),
            dot_bracket_(seg.dot_bracket_) {}

public: // non const methods
    void
    move(
            math::Point const & p) {
        structure_.move(p);
        proteins_.move(p);
        small_molecules_.move(p);
        for(auto & bp : basepairs_) { bp.move(p); }
        for(auto & bp : ends_) { bp.move(p); }

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
        for(auto & bp : ends_) { bp.transform(r, t, dummy); }
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
            String const &);

    void
    write_steric_beads_to_pdb(
            String const &);

public:
    inline
    String
    dot_bracket() { return dot_bracket_->get_str(); }

    inline
    Basepair const &
    get_aligned_end() { return ends_[aligned_end_]; }

protected:
    Structure proteins_;
    Structure small_molecules_;
    base::SimpleStringCOP dot_bracket_;


};

typedef std::shared_ptr<Segment> SegmentOP;
typedef std::vector<SegmentOP>   SegmentOPs;


}


#endif //RNAMAKE_NEW_ALL_ATOM_SEGMENT_H
