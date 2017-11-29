//
//  residue.h
//  RNAMake
//
//  Created by Joseph Yesselman on 1/25/15.
//  Copyright (c) 2015 Joseph Yesselman. All rights reserved.
//

#ifndef __RNAMake__all_atom_residue__
#define __RNAMake__all_atom_residue__

#include <stdio.h>

//RNAMake Headers
#include <base/types.h>
#include <util/uuid.h>
#include <primitives/residue.h>
#include <all_atom/atom.h>
#include <all_atom/residue_type.h>
#include <all_atom/residue_type_set.h>
#include <util/bead.h>


namespace all_atom {

math::Point
center(
        Atoms const &);


/**
 * Store residue information from pdb file, stores all Atom objects that
 * belong to residue. Implementation is designed to be extremely lightweight.
 *
 * To get an example instance of Residue please include:\n
 * #include "instances/structure_instances.hpp"
 *
 * Example of Common Usage:
 *
 * @code
 *  //grabbing example instance
 *  #include "instances/structure_instances.hpp"
 *  auto r = instances::residue();
 *  //simply getting name of residue, etc: A,G,C or U
 *  std::cout << r->name() << std::endl; //OUTPUT: "G"
 *
 *  //getting a specific atom from residue
 *  auto a = r->get_atom("C1'");
 *  std::cout << a->coords() << std::endl; //OUTPUT: "-23.806 -50.289  86.732"
 *
 *  auto beads = r->get_beads();
 *  std::cout << beads[0]->btype() << " " << beads[0]->center() << std::endl;
 *  //OUTPUT: "1 24.027 -48.5001111111 86.368"
 *
 *  //get PDB formatted String (can also write to file with r->to_pdb("test.pdb") )
 *  std::cout << r->to_pdb_str() << std::endl; //OUTPUT -->
 *  ATOM      1 O5'  G   A 103     -26.469 -47.756  84.669  1.00  0.00
 *  ATOM      2 C5'  G   A 103     -25.050 -47.579  84.775  1.00  0.00
 *  ATOM      3 C4'  G   A 103     -24.521 -48.156  86.068  1.00  0.00
 *  ATOM      4 O4'  G   A 103     -24.861 -49.568  86.118  1.00  0.00
 *  ATOM      5 C3'  G   A 103     -23.009 -48.119  86.281  1.00  0.00
 *  ATOM      6 O3'  G   A 103     -22.548 -46.872  86.808  1.00  0.00
 *  .
 *  .
 *  .
 * @endcode
 */
class Residue : public primitives::Residue {
public:

    /**
     * Standard constructor for Residue object.
     * @param   name    name of residue i.e. "G", "C", etc
     * @param   num     residue num 
     * @param   chain_id    the id of the chain i.e. "A", "B", only one character
     * @param   i_code  residue insertion code, usually nothing ("")
     * @param   rtype   residue type, dictates residue topology and information
     *
     */
    Residue(
            char name,
            int num,
            char chain_id,
            char i_code,
            ResidueTypeCOP res_type,
            Atoms const & atoms,
            util::Uuid const & uuid):
            primitives::Residue(name, num, chain_id, i_code, uuid),
            res_type_(res_type),
            atoms_(atoms) {
        _build_beads();
    }

    /**
     * Copy constructor
     * @param   r   Residue object copying from
     */
    Residue(
            Residue const & r):
            primitives::Residue(r.name_, r.num_, r.chain_id_, r.i_code_, r.uuid_),
            res_type_(r.res_type_),
            atoms_(r.atoms_),
            beads_(r.beads_) {}

    /**
     * Construction from String, used in reading data from files
     * @param   s   string generated from to_str()
     * @see to_str()
     */
    Residue(
            String const & s,
            ResidueTypeSet const & rts):
            primitives::Residue() {

        auto spl  = base::split_str_by_delimiter(s, ",");
        res_type_ = rts.get_residue_type(spl[0]);
        name_     = spl[1][0];
        num_      = std::stoi(spl[2]);
        chain_id_ = spl[3][0];
        i_code_   = spl[4][0];
        uuid_     = util::Uuid();
        atoms_    = Atoms();
        int i = 5;
        while (i < spl.size()) {
            atoms_.push_back(Atom(spl[i]));
            i++;
        }
        _build_beads();
    }


    /**
     * Empty deconstructor
     */
    ~Residue() {}

public: //iterator stuff
    typedef Atoms::const_iterator const_iterator;

    const_iterator begin() const noexcept { return atoms_.begin(); }
    const_iterator end() const noexcept   { return atoms_.end(); }

public:

    inline
    bool
    operator == (
            Residue const & r) const {
        if(name_ != r.name_) { return false; }
        if(num_ != r.num_) { return false; }
        if(chain_id_ != r.chain_id_) { return false; }
        if(i_code_ != r.i_code_) { return false; }
        if(uuid_ != r.uuid_) { return false; }
        for(int i = 0; i < atoms_.size(); i++) {
            if(atoms_[i] != r.atoms_[i]) { return false;}
        }
        return true;
    }

public:
    inline
    bool
    is_equal(
            Residue const & r,
            bool check_uuid = true) const {
        if(check_uuid && uuid_ != r.uuid_) { return false; }
        if(name_ != r.name_) { return false; }
        if(num_ != r.num_) { return false; }
        if(chain_id_ != r.chain_id_) { return false; }
        if(i_code_ != r.i_code_) { return false; }
        for(int i = 0; i < atoms_.size(); i++) {
            if(atoms_[i] != r.atoms_[i]) { return false;}
        }
        return true;
    }


public: // getters

    /**
     * get atom object by its name
     * @param   name   name of atom
     *
     * examples:
     * @code 
     *  auto a = r->get_atom("C1'");
     *  std::cout << a->coords() << std::endl;
     *  //OUTPUT -23.806 -50.289  86.732
     * @endcode
     */
    inline
    Atom const &
    get_atom(
            String const & name) const {
        int index = res_type_->get_atom_index(name);
        if (index == -1) {
            throw ResidueException("atom name: " + name + " does not exist in this residue");
        }
        return atoms_[index];
    }

    inline
    Atom const &
    get_atom(
            Index index) const {
        return atoms_[index];
    }

    inline
    math::Point const &
    get_coords(
            String const & name) const {
        return get_atom(name).get_coords();
    }

    inline
    util::Bead const &
    get_bead(
            util::BeadType bead_type) const {
        for(auto const & b : beads_) {
            if(b.get_type() == bead_type) { return b; }
        }
        throw ResidueException(
                "bead type: " + std::to_string((int)bead_type) + " does not exist in this residue");
    }

    inline
    math::Point
    get_center() const { return center(atoms_); }

    inline
    String const &
    get_res_name() { return res_type_->get_name(); }

    /**
     * Determine if another residue is connected to this residue, returns 0
     * if res is not connected to self, returns 1 if connection is going
     *  from 5' to 3' and returns -1 if connection is going from 3' to 5'
     * @param   res another residue
     * @param   cutoff  distance to be considered connected, default: 3 Angstroms
     */
    /*inline
    int
    connected_to(
            Residue const & res,
            float cutoff = 3.0) const {
        String o3 = "O3'", p = "P";

        // 5' to 3'
        AtomOP o3_atom = get_atom(o3), p_atom = res.get_atom(p);
        if (o3_atom != nullptr && p_atom != nullptr) {
            if (o3_atom->coords().distance(p_atom->coords()) < cutoff) {
                return 1;
            }
        }

        // 3' to 5'
        o3_atom = res.get_atom(o3);
        p_atom = get_atom(p);
        if (o3_atom != nullptr && p_atom != nullptr) {
            if (o3_atom->coords().distance(p_atom->coords()) < cutoff) {
                return -1;
            }
        }

        return 0;
    }
    */

    /**
     * stringifes residue object
     * @code 
     *  #include "instances/structure_instances.hpp"
     *  auto r = instances::residue();
     *  std::cout << r->to_str() << std::endl;
     *  //OUTPUT 
     *  GUA,G,103,A,,N,N,N,O5' -26.469 -47.756 84.669,C5' -25.05 -47.579 84.775,C4' -24.521 -48.156 
     *  86.068,O4' -24.861 -49.568 86.118,C3' -23.009 -48.119 86.281,O3' -22.548 -46.872 86.808,C1'
     *  -23.806 -50.289 86.732,C2' -22.812 -49.259 87.269,O2' -23.167 -48.903 88.592,N1 -19.538 -52.485 
     *  85.025,C2 -19.717 -51.643 86.097,N2 -18.624 -51.354 86.809,N3 -20.884 -51.124 86.445,C4 -21.881
     *  -51.521 85.623,C5 -21.811 -52.356 84.527,C6 -20.546 -52.91 84.164,O6 -20.273 -53.677 83.228,N7 
     *  -23.063 -52.513 83.947,C8 -23.858 -51.786 84.686,N9 -23.21 -51.159 85.722,
     * @endcode
     */
    String
    get_str() const;

    /**
     * wrapper for to_pdb_str(int &, int, String const &) when one does not care about
     * renumbering atoms and residue
     */

    inline
    String
    get_pdb_str(
            int acount) {
        return get_pdb_str(acount, num_, chain_id_);
    }

    /**
     * returns pdb formatted string of residue's coordinate information
     * @param   acount  current atom index, default: 1
     * @param   rnum    starting residue number
     * @param   chain_id    the chain id of the chain, i.e. "A", "B" etc
     * @code 
     *  #include "instances/structure_instances.hpp"
     *  auto r = instances::residue();
     *  //get PDB formatted String (can also write to file with r->to_pdb("test.pdb") )
     *  std::cout << r->to_pdb_str() << std::endl; //OUTPUT -->
     *  ATOM      1 O5'  G   A 103     -26.469 -47.756  84.669  1.00  0.00
     *  ATOM      2 C5'  G   A 103     -25.050 -47.579  84.775  1.00  0.00
     *  ATOM      3 C4'  G   A 103     -24.521 -48.156  86.068  1.00  0.00
     *  ATOM      4 O4'  G   A 103     -24.861 -49.568  86.118  1.00  0.00
     *  ATOM      5 C3'  G   A 103     -23.009 -48.119  86.281  1.00  0.00
     *  ATOM      6 O3'  G   A 103     -22.548 -46.872  86.808  1.00  0.00
     *  .
     *  .
     *  .
     * @endcode
     */
    String
    get_pdb_str(
            int,
            int,
            char) const;

    /**
     * writes a PDB string formmated verision of this Residue object to file
     * @param  filename of output PDB file
     */
    void
    write_pdb(String const);

public:

    inline
    void
    move(
            math::Point const & p) {
        for(auto & a : atoms_) { a.move(p); }
        for(auto & b : beads_) { b.move(p); }
    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t,
            math::Point & dummy) {
        for(auto & a : atoms_) { a.transform(r, t, dummy); }
        for(auto & b : beads_) { b.transform(r, t, dummy); }
    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t) {
        auto dummy = math::Point();
        for(auto & a : atoms_) { a.transform(r, t, dummy); }
        for(auto & b : beads_) { b.transform(r, t, dummy); }
    }

    /**
     * equal operator checks whether the unique indentifier is the same 
     * @param   r   another residue to check if its the same
     */
    /*bool
    operator==(Residue const & r) const {
        return uuid_ == r.uuid_;
    }*/

public: // getters

    /**
     * getter for the one letter residue type
     */
    /*inline
    String
    short_name() const { return res_type_->short_name(); }*/

private:
    void
    _build_beads();

private:
    /**
     * residue type object which explains what atoms in belong in this residue.
     */
    ResidueTypeCOP res_type_;

    /**
     * vector of the atom objects that belong to this residue
     */
    Atoms atoms_;

    /**
     * vector of bead objects for sterics
     */
    util::Beads beads_;

    /**
     * unique residue indentifier so each residue can be be found in larger structures
     */
    util::Uuid uuid_;

};

/**
 * Shared pointer typedef for Residue. Only use shared pointers!
 */
typedef std::shared_ptr<Residue> ResidueOP;
typedef std::shared_ptr<Residue const> ResidueCOP;

/**
 * Typedef of a vector of shared pointer vectors, only used this.
 */
typedef std::vector<ResidueOP> ResidueOPs;


}


#endif /* defined(__RNAMake__all_atom_residue__) */
