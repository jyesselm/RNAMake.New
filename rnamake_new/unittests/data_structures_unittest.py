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
        g = ResidueGraph()

    def test_add_node(self):
        g = ResidueGraph()
        r  = primitives.Residue('A', 1, 'A', ' ', Uuid())
        r2 = primitives.Residue('A', 2, 'A', ' ', Uuid())
        self.failUnless(g.add_node(r) == 0)

        r_same = g.get_node(0)
        self.failUnless(r == r_same)
        self.failUnless(g.get_num_nodes() == 1)

        g.add_node(r2, 0)
        self.failUnless(g.get_num_nodes() == 2)

    def test_iter_basic(self):
        g = ResidueGraph()
        r  = primitives.Residue('A', 1, 'A', ' ', Uuid())
        r2 = primitives.Residue('A', 2, 'A', ' ', Uuid())
        g.add_node(r)
        g.add_node(r2, 0)

        res = []
        for n in g:
            res.append(n.data)

        self.failUnless(len(res) == len(g))
        self.failUnless(res[0] == r)

        g.change_transverse_start(1)

        res = []
        for n in g:
            res.append(n.data)
        #self.failUnless(len(res) == len(g))
        #self.failUnless(res[0] == r2)


    """
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
        pass"""

class StaticEdgedGraphUnittest(unittest.TestCase):
    def setUp(self):
        pass

    #def test_creation(self):
    #    g = StaticEdgedGraph()

class DirectedGraphUnittest(unittest.TestCase):
    def setUp(self):
        self.r1 = primitives.Residue('A', 1, 'A', ' ', Uuid())
        self.r2 = primitives.Residue('A', 2, 'A', ' ', Uuid())
        self.r3 = primitives.Residue('A', 3, 'A', ' ', Uuid())
        self.r4 = primitives.Residue('A', 4, 'A', ' ', Uuid())

    def test_creation(self):
        dg = ResidueDirectedGraph()
        dg.add_node(self.r1, 2)
        dg.add_node(self.r2, 2, 0, 0, 0)
        dg.add_node(self.r3, 2, 0, 1, 0)
        dg.add_node(self.r4, 2)

        self.failUnless(dg.has_parent(0) == False)
        self.failUnless(dg.get_parent_index(1) == 0)
        self.failUnless(dg.get_parent_index(2) == 0)

        self.failUnless(len(dg.get_all_edges()) == 2)

        self.failUnless(dg.are_nodes_connected(0, 1) == True)
        self.failUnless(dg.are_nodes_connected(1, 2) == False)

        expected_indexes = [0, 1, 2, 3]
        indexes = []
        for n in dg:
            indexes.append(n.index)
        self.failUnless(expected_indexes == indexes)

        #self.failUnless(len(dg.get_roots()) == 2)



def main():
    unittest.main()

if __name__ == '__main__':
    main()








































