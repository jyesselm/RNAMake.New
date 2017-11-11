import unittest
import numpy as np

from rnamake_new.util import *
from rnamake_new.all_atom import *
from rnamake_new.base import *
from rnamake_new.math_util import *

class AtomUnittest(unittest.TestCase):

    def test_creation(self):
        Atom(SimpleString("P"), Point(0, 0, 0))

    def test_getters(self):
        a = Atom(SimpleString("P"), Point(0, 0, 0))
        self.failUnless(a.get_name().get_str() == "P")
        self.failUnless(a.get_coords().distance(Point(0, 0, 0)) < 0.1)

    def test_copy(self):
        a = Atom(SimpleString("P"), Point(0, 0, 0))
        a2 = Atom(a)
        self.failUnless(a == a2)

    def test_to_string(self):
        a = Atom(SimpleString("P"), Point(0, 0, 0))
        s = a.get_str()
        a2 = Atom(s)
        self.failUnless(a == a2)

    def test_transform(self):
        f = open("resources/all_atom/test_transform_atoms.dat")
        lines = f.readlines()
        f.close()

        r = Matrix(lines[0])
        d = Point(lines[1])

        r_T = r.get_transpose()

        name = SimpleString("P")
        atoms = []
        final_atoms = []
        spl = lines[2].split()
        for i in range(0, len(spl), 3):
            p = Point(float(spl[i]), float(spl[i+1]), float(spl[i+2]))
            a = Atom(name, p)
            atoms.append(a)

        spl = lines[3].split()
        for i in range(0, len(spl), 3):
            p = Point(float(spl[i]), float(spl[i + 1]), float(spl[i + 2]))
            a = Atom(name, p)
            final_atoms.append(a)

        for i, a in enumerate(atoms):
            a.transform(r_T, d)
            self.failUnless(a == final_atoms[i])





def main():
    unittest.main()

if __name__ == '__main__':
    main()



