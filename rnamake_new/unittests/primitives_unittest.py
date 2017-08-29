import unittest
import numpy as np

from rnamake_new.util import *
from rnamake_new.primitives import *


class NewRes(Residue):
    def __init__(self, name, num, chain_id, i_code, uuid):
        super(self.__class__, self).__init__(name, num, chain_id,
                                             i_code, uuid)

    def unique(self):
        return 1

class ResidueUnittest(unittest.TestCase):
    def setUp(self):
        pass

    def test_creation(self):
        Residue('A', 1, 'A', ' ', Uuid())

    def test_gets(self):
        r = Residue('A', 1, 'A', ' ', Uuid())
        self.failUnless(r.get_name() == 'A')
        self.failUnless(r.get_num() == 1)

    def test_inherit(self):
        new_r = NewRes('A', 1, 'A', ' ', Uuid())
        self.failUnless(new_r.unique() == 1)

class ChainUnittest(unittest.TestCase):

    def setUp(self):
        self.r1 = Residue('A', 1, 'A', ' ', Uuid())
        self.r2 = Residue('A', 2, 'A', ' ', Uuid())
        self.r3 = Residue('A', 2, 'A', ' ', Uuid())
        self.c = Chain([self.r1, self.r2, self.r3])

    def test_first_and_last(self):
        self.failUnless(self.c.get_first() == self.r1)
        self.failUnless(self.c.get_last()  == self.r3)

        c = Chain([])

        with self.assertRaises(ChainException):
            c.get_first()


    def test_inherit(self):
        r1 = NewRes('A', 1, 'A', ' ', Uuid())
        r2 = NewRes('A', 2, 'A', ' ', Uuid())

        new_c = Chain([r1, r2])
        self.failUnless(new_c.get_residue(0).unique() == 1)

    def test_iterator(self):
        for r in self.c:
            print r


def main():
    unittest.main()

if __name__ == '__main__':
    main()
