import unittest
import numpy as np

from rnamake_new.util import *


class UtilUnittest(unittest.TestCase):
    def setUp(self):
        pass

    def test_creation(self):
        Uuid()

    def test_comparision(self):
        u1 = Uuid()
        u2 = Uuid()

        u3 = u1
        self.failUnless(u1 == u3)




def main():
    unittest.main()

if __name__ == '__main__':
    main()
