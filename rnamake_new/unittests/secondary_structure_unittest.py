import unittest
import numpy as np

from rnamake_new.util import *
from rnamake_new.secondary_structure import *
from rnamake_new.base import *


class NewRes(Residue):
    def __init__(self, name, dot_bracket, num, chain_id, i_code, uuid):
        super(self.__class__, self).__init__(name, dot_bracket, num,
                                             chain_id, i_code, uuid)

    def unique(self):
        return 1


class ResidueUnittest(unittest.TestCase):

    def setUp(self):
        pass

    def test_creation(self):
        Residue('A', '(', 1, 'A', ' ', Uuid())

    def test_gets(self):
        r = Residue('A', '(', 1, 'A', ' ', Uuid())
        self.failUnless(r.get_name() == 'A')
        self.failUnless(r.get_num() == 1)

    def test_inherit(self):
        new_r = NewRes('A', '(', 1, 'A', ' ', Uuid())
        self.failUnless(new_r.unique() == 1)



def main():
    unittest.main()

if __name__ == '__main__':
    main()






