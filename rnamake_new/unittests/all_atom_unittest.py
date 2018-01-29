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


class ResidueTypeSetUnittest(unittest.TestCase):

    def setUp(self):
        self.rts = ResidueTypeSet()

    def test_get_residue_type(self):
        rtype = self.rts.get_residue_type("GUA")

        self.failUnless(rtype.get_name() == "GUA")


class PDBParserUnittest(unittest.TestCase):
    def setUp(self):
        self.rts = ResidueTypeSet()
        self.pdb_parser = PDBParser(self.rts)

    def test_ideal_res(self):
        path = resources_path() + "/ideal_residues/GUA.pdb"
        residues = self.pdb_parser.parse(path)
        r1 = residues.RNA_residues[0]
        self.failUnless(len(residues.RNA_residues) == 1)

        path = unittest_resources_path() + "/all_atom/GUA_incomplete.pdb"
        residues_2 = self.pdb_parser.parse(path)
        r2 = residues_2.RNA_residues[0]
        self.failUnless(r1.is_equal(r2, False))

    def test_p4p6(self):
        path = unittest_resources_path() + "/all_atom/p4p6.pdb"
        residues = self.pdb_parser.parse(path)
        self.failUnless(len(residues.RNA_residues) == 157)

    def test_parsing_proteins(self):
        path = unittest_resources_path() + "/all_atom/2lxe.pdb"
        residues = self.pdb_parser.parse(path)
        self.failUnless(len(residues.protein_residues) == 109)

    def test_parsing_small_molecules(self):
        path = unittest_resources_path() + "/all_atom/1am0.pdb"
        residues = self.pdb_parser.parse(path)
        self.failUnless(len(residues.small_molecule_residues) == 1)
        amp = residues.small_molecule_residues[0]
        self.failUnless(amp.get_res_name() == "AMP")

class StructureUnittest(unittest.TestCase):
    def setUp(self):
        self.rts = ResidueTypeSet()
        self.pdb_parser = PDBParser(self.rts)

    def test_create(self):
        path = unittest_resources_path() + "/all_atom/p4p6.pdb"
        residues = self.pdb_parser.parse(path)
        s = get_structure_from_residues(residues.RNA_residues)
        r = s.get_residue(0)



def main():
    unittest.main()

if __name__ == '__main__':
    main()




