//
//  Numeric_Test.h
//  REDESIGNC
//
//  Created by Joseph Yesselman on 10/4/14.
//  Copyright (c) 2014 Joseph Yesselman. All rights reserved.
//

#ifndef __REDESIGNC__Numeric_Test__
#define __REDESIGNC__Numeric_Test__

#include <iostream>

//RNAMake Headers
#include <math/xyz_vector.h>
#include <math/xyz_matrix.h>

namespace math {

int
are_floats_equal(
        double const a,
        double const b,
        double tol = 0.001);

int
are_points_equal(
        Point const & p1,
        Point const & p2,
        float tol = 0.001);

int
are_matrices_equal(
        Matrix const &,
        Matrix const &);

Matrix
get_random_rotation_matrix();

}

#endif /* defined(__REDESIGNC__Numeric_Test__) */
