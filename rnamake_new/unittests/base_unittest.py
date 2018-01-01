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

        s5 = SimpleString(s1.get_str())
        self.failUnless(s1 == s5)


    def test_get_str(self):
        s1 = SimpleString("test")
        self.failUnless(s1.get_str() == "test")

class JSONUnittest(unittest.TestCase):
    def test_creation(self):
        j = JSON()
        j.append(True)
        #print j

    def test_load(self):
        s = """
        {"coords" : [-26.422439, -49.199253, 82.788377], "name" : "P" }

        """
        j2 = Load(s)
        print j2
        print j2.dump_minimized()
        #self.failUnless(j2["name"].ToString() == "P")

def main():
    unittest.main()

if __name__ == '__main__':
    main()
