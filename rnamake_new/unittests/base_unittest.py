import unittest
import numpy as np

from rnamake_new.base import *


class SimpleStringUnittest(unittest.TestCase):
    def setUp(self):
        pass

    def test_creation(self):
        SimpleString("test")

    def test_comparision(self):
        s1 = SimpleString("test")
        s2 = SimpleString("test")
        s3 = SimpleString("test_2")
        s4 = SimpleString("fail")

        self.failUnless(s1 == s2)
        self.failUnless(s1 != s3)
        self.failUnless(s1 != s4)

    def test_get_str(self):
        s1 = SimpleString("test")
        self.failUnless(s1.get_str() == "test")

def main():
    unittest.main()

if __name__ == '__main__':
    main()
