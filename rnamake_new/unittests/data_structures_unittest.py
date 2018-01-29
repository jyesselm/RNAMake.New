import unittest
import numpy as np

from rnamake_new.data_structures import *
from rnamake_new.util import *

class StringstoGraph(object):
    def __init__(self):
        self._v_connect = [(1, 0), (-1, 0)]
        self._h_connect = [(0, 1), (0, -1)]
        self._connects = [(1, 0), (-1, 0), (0, 1), (0, -1)]
        self._h_pairs = [('-', '-'), ('<', '-'), ('<', '>'), ('-', '>')]
        self._v_pairs = [('|', '|'), ('^', '|'), ('v', '|'), ('^', 'v'), ('|', '-'), ('-', '|')]
        self._arrows = ['<', '>', '^', 'v']
        self._connection_paths = []
        self._connections = []
        self._nodes = []

    def _find_nodes(self, strs):
        self._node_pos = []
        for i in range(0, len(strs)):
            self._node_pos.append([-1 for x in range(0, len(strs[i]))])

        num = ""
        pos = []
        nodes = []
        for i, s in enumerate(strs):
            for j, e in enumerate(s):
                if e.isdigit():
                    num += e
                    pos.append((i, j))
                else:
                    for i,j in pos:
                        self._node_pos[i][j] = int(num)
                        nodes.append(int(num))
                    num = ""
                    pos = []
        nodes.sort()
        self._nodes = nodes

    def _step_path(self, i, j, path, seen, strs):
        seen.append((i, j))
        path.append((i, j))

        for di, dj in self._v_connect:
            if i+di >= len(strs):
                continue
            if j+dj >= len(strs[i+di]):
                continue

            if strs[i+di][j+dj] == ' ':
                continue
            if self._node_pos[i+di][j+dj] != -1:
                continue
            if (i+di, j+dj) in seen:
                continue
            if (strs[i][j], strs[i+di][j+dj]) in self._v_pairs:
                return self._step_path(i+di, j+dj, path, seen, strs)

        for di, dj in self._h_connect:
            if i + di >= len(strs):
                continue
            if j + dj >= len(strs[i + di]):
                continue

            if strs[i + di][j + dj] == ' ':
                continue
            if self._node_pos[i + di][j + dj] != -1:
                continue
            if (i + di, j + dj) in seen:
                continue
            if (strs[i][j], strs[i + di][j + dj]) in self._h_pairs:
                return self._step_path(i + di, j + dj, path, seen, strs)

    def _find_connections(self, strs):
        connections = []
        for i, s in enumerate(strs):
            for j, e in enumerate(s):
                if e == ' ':
                    continue
                if self._node_pos[i][j] != -1:
                    continue
                #if (i, j) in seen:
                #    continue
                path = []
                seen = []
                self._step_path(i, j, path, seen, strs)
                connections.append(path)

        saved_connections = []
        connections.sort(key=lambda x: len(x))
        for i, c1 in enumerate(connections):
            keep = 1
            for j, c2 in enumerate(connections):
                if i >= j:
                    continue
                for p in c1:
                    if p in c2:
                        keep = 0
                        break
            if keep:
                saved_connections.append(c1)

        self._connection_paths = saved_connections

    def parse(self, strs):
        self._find_nodes(strs)
        self._find_connections(strs)

        for c in self._connection_paths:
            start_p = c[0]
            end_p = c[-1]

            start_n = -1
            end_n = -1

            for di, dj in self._connects:
                try:
                    if self._node_pos[start_p[0]+di][start_p[1]+dj] != -1:
                        start_n =  self._node_pos[start_p[0]+di][start_p[1]+dj]
                except IndexError:
                    pass

            for di, dj in self._connects:
                try:
                    if self._node_pos[end_p[0]+di][end_p[1]+dj] != -1:
                        end_n = self._node_pos[end_p[0]+di][end_p[1]+dj]
                except IndexError:
                    pass
            if start_n == -1 or end_n == -1:
                raise ValueError("cannot find nodes in connection")

            connection = [start_n, end_n]
            if strs[start_p[0]][start_p[1]] in self._arrows:
                connection.append(1)
            else:
                connection.append(0)

            if strs[end_p[0]][end_p[1]] in self._arrows:
                connection.append(1)
            else:
                connection.append(0)
            self._connections.append(connection)

        return self._build_directed_graph()

    def _get_node_directed_connections(self, n):
        node_connections = []
        for c in self._connections:
            if c[0] == n and c[2] == 1:
                node_connections.append(c)
            if c[1] == n and c[3] == 1:
                node_connections.append(c)

        return node_connections

    def _get_next_edge_index(self, g, ni):
        for i in range(0, 10):
            if not g.edge_index_empty(ni, i):
                continue
            return i

    def _build_directed_graph(self):
        g = FixedEdgeDirectedGraphInt()
        last = None
        for n in self._nodes:
            if last is None:
                g.add_node(n, 10)
                last = n
                continue

            if n - last != 1:
                raise ValueError("nodes must be sequential")

            connections = self._get_node_directed_connections(n)
            parent = None
            for c in connections:
                other = None
                if n == c[0]:
                    other = c[1]
                else:
                    other = c[0]
                try:
                    node = g.get_node(other)
                except:
                    raise ValueError("cannot find parent in graph")
                parent = other
                break

            if parent is None:
                g.add_node(n, 10)
            else:
                pei = -1
                for i in range(0, 10):
                    if not g.edge_index_empty(parent, i):
                        continue
                    pei = i
                    break
                g.add_node(n, 10, 0, NodeIndexandEdge(parent, pei))

            last = n

        for c in self._connections:
            if c[2] == 0 and c[3] == 0:
                e1 = self._get_next_edge_index(g, c[0])
                e2 = self._get_next_edge_index(g, c[1])
                g.add_edge(NodeIndexandEdge(c[0], e1), NodeIndexandEdge(c[1], e2))


        return g


class StringstoGraphUnittest(unittest.TestCase):
    def test_creation(self):
        parser = StringstoGraph()

    def test_connections(self):
        s = """
        0 ------->1
        | |
        | |
        ---
        """

        lines = s.split("\n")
        parser = StringstoGraph()
        g = parser.parse(lines)
        self.failUnless(len(g) == 2)
        self.failUnless(g.get_num_edges() == 1)
        self.failUnless(g.edge_between_nodes(0, 1))

        s = """
              1<-----
        0-----------|
                   ||
                   --
        """
        lines = s.split("\n")
        parser = StringstoGraph()
        g = parser.parse(lines)
        self.failUnless(len(g) == 2)
        self.failUnless(g.get_num_edges() == 1)
        self.failUnless(g.edge_between_nodes(0, 1))

        s = """
        0->1->2->3->4->5
        """
        lines = s.split("\n")
        parser = StringstoGraph()
        g = parser.parse(lines)
        self.failUnless(len(g) == 6)
        self.failUnless(g.get_num_edges() == 5)



class DirectedGraphUnittest(unittest.TestCase):
    def test_creation(self):
        g = FixedEdgeDirectedGraphInt()

    def test_add(self):
        g = FixedEdgeDirectedGraphInt()
        g.add_node(0, 5)
        g.add_node(1, 5, 0, NodeIndexandEdge(0, 0))
        g.setup_path_transversal(0, 1)
        target = [0, 1]
        path = []
        for n in g:
            path.append(n.index)
        self.failUnless(path == target)

    def test_parse(self):
        s = """
           5------
           ^     |
           |     |
        2->3->4  |
        ^        |
        |        |
        0->1------
        """

        parser = StringstoGraph()
        lines = s.split("\n")
        g = parser.parse(lines)
        self.failUnless(g.edge_between_nodes(1, 5))
        g.setup_path_transversal(0, 5)
        path = []
        target = [0, 2, 3, 5]
        for n in g:
            path.append(n.index)
        self.failUnless(path == target)

        g.setup_sub_graph_transversal(0, 5)
        path = []
        target = [0, 2, 3, 5, 1, 4]
        for n in g:
            path.append(int(n.index))
        self.failUnless(path == target)

    def test_sub_ring(self):
        s = """
                  ----------
                  |        |
        0----->1->2->3  4->5
        |      |     |  ^
        |      --->6--  |
        -----------------           
        """
        parser = StringstoGraph()
        lines = s.split("\n")
        g = parser.parse(lines)
        g.setup_path_transversal(0, 5)
        path = []
        target = [0, 4, 5]
        for n in g:
            path.append(int(n.index))
        self.failUnless(path == target)

        s = """
                     ----------
                     |        |
           0----->1->2->3  4->5
           |      |     |  |
           |      --->6--  |
           -----------------           
        """

        parser = StringstoGraph()
        lines = s.split("\n")
        g = parser.parse(lines)
        g.setup_sub_graph_transversal(0, 3)
        path = []
        target = [0, 1, 2, 3, 6]
        for n in g:
            path.append(int(n.index))
        self.failUnless(path == target)

    def test_sub_graphs(self):
        s = """
        0->1->2
        
        3->4->5
        
        6->7->8
        """

        parser = StringstoGraph()
        lines = s.split("\n")
        g = parser.parse(lines)
        g.setup_transversal(0)
        path = []
        target = [0, 1, 2, 3, 4, 5, 6, 7, 8]
        for n in g:
            path.append(int(n.index))
        self.failUnless(g.edge_between_nodes(2, 3) == False)
        self.failUnless(path == target)


def main():
    unittest.main()

if __name__ == '__main__':
    main()








































