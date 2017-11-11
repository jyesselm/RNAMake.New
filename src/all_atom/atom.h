
#ifndef __RNAMake__atom__
#define __RNAMake__atom__

#include <stdio.h>
//#include <math/transform.h>

//RNAMake Headers
#include <base/types.h>
#include <base/simple_string.h>
#include <math/xyz_vector.h>
#include <math/xyz_matrix.h>
#include <math/numerical.h>

namespace all_atom {

/**
 * Stores atomic information from pdb file, design is to be extremely
 * lightweight only storing the atom name and coordinates.
 *
 * Example Usage:
 *
 * @code
 *  // creation
 *  auto a = Atom("P", Point(0, 1, 2));
 *
 *  //copy
 *  auto a2 = Atom(a);
 * @endcode
 */

class Atom {
public:

    /**
     * Standard constructor for Atom object.
     * @param   name    name of atom
     * @param   coords  3d coordinates of atom's position
     */
    inline
    Atom(
            base::SimpleStringCOP name,
            math::Point const & coords) :
            name_(name),
            coords_(coords) {}

    /**
     * Construction from String, used in reading data from files
     * @param   s   string generated from to_str()
     * @see to_str()
     * 
     * Example Usage:
     * @code
     *  auto a = Atom("P", Point(0, 1, 2));
     *  auto s = a.to_str();
     *  auto a2 = Atom(s);
     * @endcode
     */
    inline
    Atom(
            String const & s) {

        auto spl = base::split_str_by_delimiter(s, " ");
        name_ = std::make_shared<base::SimpleString const>(spl[0]);
        coords_ = math::Point(std::stof(spl[1]), std::stof(spl[2]), std::stof(spl[3]));
    }

    /**
     * Copy constructor
     * @param   a   atom object to from
     */
    inline
    Atom(
            Atom const & a) :
            name_(a.name_),
            coords_(a.coords_) {}

public:
    inline
    bool
    operator == (
            Atom const & a) const {
        if(*name_ != *a.name_) { return false; }
        if(!math::are_points_equal(coords_, a.coords_)) { return false; }
        return true;
    }

    inline
    bool
    operator != (
            Atom const & a) const {
        return !(*this == a);
    }


public:

    /**
     * Strigifies atom object
     * @code
     *  auto a = Atom("P", Point(0, 1, 2));
     *  std::cout << a.to_str() << std::endl;
     *  //EXPECTED OUTPUT
     *  "H1 0.0 1.0 2.0"
     * @endcode
     */
    String get_str();

    /**
     * Strigifies atom into PDB format
     * @param   acount  the number of the atom, default=1
     *
     * @code
     *  auto a = Atom("P", Point(0, 1, 2));
     *  std::cout << a.to_pdb_str() << std::endl;
     *  //EXPECTED OUTPUT
     *  "ATOM      1  P   C   A   1       1.000   2.000   3.000  1.00 62.18           P
     * @endcode
     */
    String get_pdb_str(int);

    /**
     * @param p xyz coords to move atom by
     *
     * @code
     * @endcode
     */
    inline
    void
    move(math::Point const & p) {
        coords_ = coords_ + p;

    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t,
            math::Point & dummy) {
        math::dot_vector(r, coords_, dummy);
        coords_ = dummy + t;
    }

    inline
    void
    transform(
            math::Matrix const & r,
            math::Vector const & t) {
        auto dummy = math::dot_vector(r, coords_);
        coords_ = dummy + t;
    }

public: //accessors

    /**
     * Accessor for name_
     */
    inline
    base::SimpleStringCOP
    get_name() const { return name_; }

    /**
     * Accessor for coords_
     */
    inline
    math::Point const
    get_coords() const { return coords_; }

private:
    /**
     * private variable of name of atom
     */
    base::SimpleStringCOP name_;

    /**
     * private variable of 3D coordinates of atom
     */
    math::Point coords_;

};

/**
 * Shared pointer typedef for Atom. Only use shared pointers!
 */
typedef std::shared_ptr<Atom> AtomOP;

/**
 * Typedef of a vector of shared pointer atoms, only used this.
 */
typedef std::vector<AtomOP> AtomOPs;

bool
are_atoms_equal(
        AtomOP const &,
        AtomOP const &,
        float tol = 0.001);

bool
are_atom_vectors_equal(
        AtomOPs const &,
        AtomOPs const &,
        float tol = 0.001);


}

#endif /* defined(__RNAMake__atom__) */
