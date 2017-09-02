//
//  Numeric_Test.cpp
//  REDESIGNC
//
//  Created by Joseph Yesselman on 10/4/14.
//  Copyright (c) 2014 Joseph Yesselman. All rights reserved.
//

//RNAMake Headers
#include <base/types.h>
#include <math/numerical.h>

namespace math {

int
are_floats_equal(
		double const a,
		double const b,
		double tol) {
	if (fabs(a - b) < tol) { return 1; }
	else { return 0; }
}

int
are_points_equal(
		Point const & p1,
		Point const & p2,
		float tol) {

	if (are_floats_equal(p1.get_x(), p2.get_x(), tol) &&
		are_floats_equal(p1.get_y(), p2.get_y(), tol) &&
		are_floats_equal(p1.get_z(), p2.get_z(), tol)) {
		return 1;
	} else {
		return 0;
	}
}

int
are_matrices_equal(
		Matrix const & m,
		Matrix const & mc) {

	if (!are_floats_equal(m.get_xx(), mc.get_xx()) ||
		!are_floats_equal(m.get_xy(), mc.get_xy()) ||
		!are_floats_equal(m.get_xz(), mc.get_xz()) ||
		!are_floats_equal(m.get_yx(), mc.get_yx()) ||
		!are_floats_equal(m.get_yy(), mc.get_yy()) ||
		!are_floats_equal(m.get_yz(), mc.get_yz()) ||
		!are_floats_equal(m.get_zx(), mc.get_zx()) ||
		!are_floats_equal(m.get_zy(), mc.get_zy()) ||
		!are_floats_equal(m.get_zz(), mc.get_zz())) {
		return 0;
	}

	return 1;

}

}


