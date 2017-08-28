import unittest
import numpy as np

from rnamake_new.math_util import *
from rnamake_new import settings, py_util

class PointUnittest(unittest.TestCase):

    def setUp(self):
        pass

    def test_creation(self):
        p = Point(0, 0, 0)

    def test_get_str(self):
        p = Point(10, 10, 10)
        s = p.get_str()

        p_new = Point(s)
        self.failUnless(are_points_equal(p, p_new))

    def test_zero(self):
        p = Point(10, 10, 10)
        p.zero()

        p1 = Point(0, 0, 0)

        self.failUnless(are_points_equal(p, p1))

    def test_negate(self):
        p1 = Point(10, 10, 10)
        p1.negate()

        p2 = Point(-10, -10, -10)
        self.failUnless(are_points_equal(p1, p2))

    def test_distance(self):
        p1 = Point(10, 10, 10)
        p2 = Point(5, 5, 5)

        d = p1.distance(p2)
        expected = 8.66025403784
        self.failUnless(are_floats_equal(d, expected))

    def test_dot(self):
        p1 = Point(10, 10, 10)
        p2 = Point(5, 5, 5)

        d = p1.dot(p2)
        expected = 150.0
        self.failUnless(are_floats_equal(d, expected))

    def test_cross(self):
        p1 = Point(10, 10, 10)
        p2 = Point(5, 5, 5)

        p3 = p1.cross(p2)
        p_expected = Point(0, 0, 0)

        self.failUnless(are_points_equal(p3, p_expected))


    def test_add(self):
        p1 = Point(10, 10, 10)
        p2 = Point(5, 5, 5)
        p = p1 + p2

        p3 = Point(15, 15, 15)
        self.failUnless(are_points_equal(p, p3))


class MatrixUnittest(unittest.TestCase):

    def setUp(self):
        self.m = Matrix(0)

    def test_creation(self):
        Matrix(0)

    def test_get_str(self):
        m = Matrix(5.0)
        s = m.get_str()
        m2 = Matrix(s)

        self.failUnless(are_matrices_equal(m, m2))

    def test_unitarize(self):
        path = settings.UNITTEST_PATH + "resources/math/test_unitarize.dat"
        lines = py_util.get_lines_from_file(path)
        org_m = Matrix(lines[0])


        m = Matrix(1.0, 2.0, 3.0,
                   4.0, 5.0, 6.0,
                   1.0, 1.0, 1.0)

        unit = m.get_unitarize()
        self.failUnless(are_matrices_equal(org_m, unit))

    def test_all_unitarize(self):
        path = settings.UNITTEST_PATH + "resources/math/test_unitarize_multi.dat"
        lines = py_util.get_lines_from_file(path)

        for l in lines:
            spl = l.split("|")
            org_m = Matrix(spl[0])
            final_m = Matrix(spl[1])
            unit = org_m.get_unitarize()

            self.failUnless(are_matrices_equal(final_m, unit))
            org_m.unitarize()
            self.failUnless(are_matrices_equal(final_m, org_m))

        

def main():
    unittest.main()

if __name__ == '__main__':
    main()
