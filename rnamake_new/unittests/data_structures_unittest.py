import unittest
import numpy as np

from rnamake_new.data_structures import *
from rnamake_new.util import *
from rnamake_new import primitives


class GraphUnittest(unittest.TestCase):
    def setUp(self):
        pass

    def test_edge(self):
        e = Edge(0, 0, 0, 0)

        with self.assertRaises(AttributeError):
            e.node_i = 10

    def test_creation(self):
        g = Graph()

    def test_add_node(self):
        g = Graph()
        r  = primitives.Residue('A', 1, 'A', ' ', Uuid())
        r2 = primitives.Residue('A', 2, 'A', ' ', Uuid())
        self.failUnless(g.add_node(r) == 0)

        r_same = g.get_node(0)
        self.failUnless(r == r_same)
        self.failUnless(g.get_num_nodes() == 1)

        g.add_node(r2, 0)
        self.failUnless(g.get_num_nodes() == 2)

    def test_iter_basic(self):
        g = Graph()
        r = primitives.Residue('A', 1, 'A', ' ', Uuid())
        r2 = primitives.Residue('A', 2, 'A', ' ', Uuid())
        g.add_node(r)
        g.add_node(r2, 0)

        res = []
        for n in g:
            res.append(n)

        self.failUnless(len(res) == len(g))
        self.failUnless(res[0] == r)

        g.change_transverse_start(1)

        res = []
        for n in g:
            res.append(n)
        self.failUnless(len(res) == len(g))
        self.failUnless(res[0] == r2)

    def test_iter_2(self):
        g = Graph()
        r1 = primitives.Residue('A', 1, 'A', ' ', Uuid())
        r2 = primitives.Residue('A', 2, 'A', ' ', Uuid())
        r3 = primitives.Residue('A', 3, 'A', ' ', Uuid())
        r4 = primitives.Residue('A', 4, 'A', ' ', Uuid())

        g.add_node(r1)
        g.add_node(r2, 0)
        g.add_node(r3, 0)
        g.add_node(r4, 0)

        res = []
        for n in g:
            res.append(n)

        self.failUnless(len(res) == 4)
        self.failUnless(res[0] == r1)

        g = Graph()
        g.add_node(r1)
        g.add_node(r2, 0)
        g.add_node(r3, 1)
        g.add_node(r4, 2)

        res = []
        for n in g:
            res.append(n)
        self.failUnless(len(res) == 4)
        self.failUnless(res[0] == r1)
        self.failUnless(res[3] == r4)

    def test_are_nodes_connected(self):
        g = Graph()
        r = primitives.Residue('A', 1, 'A', ' ', Uuid())
        r2 = primitives.Residue('A', 2, 'A', ' ', Uuid())
        r3 = primitives.Residue('A', 3, 'A', ' ', Uuid())
        g.add_node(r)
        g.add_node(r2, 0)
        g.add_node(r3)

        self.failUnless(g.are_nodes_connected(0, 1))
        self.failUnless(g.are_nodes_connected(2, 1) == 0)

    def test_second_binding(self):
        pass

class StaticEdgedGraphUnittest(unittest.TestCase):
    def setUp(self):
        pass

    #def test_creation(self):
    #    g = StaticEdgedGraph()

def main():
    unittest.main()

if __name__ == '__main__':
    main()
