import unittest
import numpy as np

from rnamake_new.util import *
from rnamake_new.secondary_structure import *
from rnamake_new.primitives import BasepairType
from rnamake_new.base import SimpleString

class NewRes(Residue):
    def __init__(self, name, dot_bracket, num, chain_id, i_code, uuid):
        super(self.__class__, self).__init__(name, dot_bracket, num,
                                             chain_id, i_code, uuid)

    def unique(self):
        return 1


class ResidueUnittest(unittest.TestCase):

    def setUp(self):
        self.r =  Residue('A', '(', 1, 'A', ' ', Uuid())

    def test_creation(self):
        Residue('A', '(', 1, 'A', ' ', Uuid())

        with self.assertRaises(ResidueException):
            Residue('X', '(', 1, 'A', ' ', Uuid())

    def test_copy(self):
        r2 = self.r
        self.failUnless(self.r == r2)

        r2 = Residue(self.r)
        self.failUnless(self.r == r2)

        r2.set_name("G")
        self.failUnless(self.r != r2)

    def test_get_str(self):
        s = self.r.get_str()
        r2 = Residue(s)
        self.failUnless(self.r.is_equal(r2, False))

        # not a proper string of a residue
        with self.assertRaises(ResidueException):
            Residue("A")

    def test_gets(self):
        r = Residue('A', '(', 1, 'A', ' ', Uuid())
        self.failUnless(r.get_name() == 'A')
        self.failUnless(r.get_num() == 1)

    def test_inherit(self):
        new_r = NewRes('A', '(', 1, 'A', ' ', Uuid())
        self.failUnless(new_r.unique() == 1)


class ChainUnittest(unittest.TestCase):

    def setUp(self):
        self.r1 = Residue('A', '(', 1, 'A', ' ', Uuid())
        self.r2 = Residue('A', '.', 2, 'A', ' ', Uuid())
        self.r3 = Residue('A', '(', 2, 'A', ' ', Uuid())
        self.c = Chain([self.r1, self.r2, self.r3])

    def test_creation(self):
        self.failUnless(self.c.get_length() == 3)

    def test_first_and_last(self):
        self.failUnless(self.c.get_first() == self.r1)
        self.failUnless(self.c.get_last()  == self.r3)

    def test_iterator(self):
        count = 0
        for r in self.c:
            count += 1
        self.failUnless(count == 3)

    def test_copy(self):
        c2 = Chain(self.c)
        self.failUnless(self.c == c2)

        c2.set_residue_name(0, "G")
        self.failUnless(self.c !=  c2)

    def test_get_str(self):
        s = self.c.get_str()
        c2 = Chain(s)
        self.failUnless(self.c.is_equal(c2, False))


class StructureUnittest(unittest.TestCase):

    def setUp(self):
        self.r1 = Residue('A', '(', 1, 'A', ' ', Uuid())
        self.r2 = Residue('A', '.', 2, 'A', ' ', Uuid())
        self.r3 = Residue('A', '(', 2, 'A', ' ', Uuid())
        self.s = Structure([self.r1, self.r2, self.r3], [3])

    def test_creation(self):
        self.failUnless(self.s.get_num_residues() == 3)
        self.failUnless(self.s.get_sequence() == "AAA")

    def test_get_str(self):
        s = self.s.get_str()
        s2 = Structure(s)

    def test_get_residue(self):
        r = self.s.get_residue(1, 'A', ' ')
        self.failUnless(self.r1 == r)

        r1 = self.s.get_residue(r.get_uuid())
        self.failUnless(self.r1 == r1)

        # by internal position
        r2 = self.s.get_residue(0)
        self.failUnless(self.r1 == r2)

        # all these should raise errors, no longer returning None if failed search
        with self.assertRaises(StructureException):
            self.s.get_residue(10)

        with self.assertRaises(StructureException):
            self.s.get_residue(Uuid())

        with self.assertRaises(StructureException):
            self.s.get_residue(1, 'A', 'B')

        with self.assertRaises(StructureException):
            self.s.get_residue(1, 'B', ' ')


    def test_structure_from_ss(self):
        s = get_structure_from_secondary_structure("GAG&CUC", "(((&)))")

        self.failUnless(s.get_num_residues() == 6)
        self.failUnless(s.get_num_chains() == 2)
        self.failUnless(s.get_sequence() == "GAG&CUC")

        # differnt lengths
        with self.assertRaises(StructureException):
            get_structure_from_secondary_structure("GAG&CUC", "(((&))")

        # invalid characters
        with self.assertRaises(StructureException):
            get_structure_from_secondary_structure("KAG&CUC", "(((&)))")


class BasepairUnittest(unittest.TestCase):

    def setUp(self):
        self.r1 = Residue('A', '(', 1, 'A', ' ', Uuid())
        self.r2 = Residue('U', '(', 2, 'A', ' ', Uuid())
        name = generate_bp_name(self.r1, self.r2)
        self.bp = Basepair(self.r1.get_uuid(), self.r2.get_uuid(), Uuid(),
                           BasepairType.WC, SimpleString(name))

    def test_getters(self):
        self.failUnless(self.r1.get_uuid() == self.bp.get_res1_uuid())
        self.failUnless(self.r2.get_uuid() == self.bp.get_res2_uuid())
        self.failUnless(self.bp.get_bp_type() == BasepairType.WC)


class RNAStructureUnittest(unittest.TestCase):

    def setUp(self):
        self.r1 = Residue('A', '(', 1, 'A', ' ', Uuid())
        self.r2 = Residue('G', '(', 2, 'A', ' ', Uuid())
        self.r3 = Residue('U', '(', 3, 'A', ' ', Uuid())
        self.r4 = Residue('A', ')', 4, 'B', ' ', Uuid())
        self.r5 = Residue('C', ')', 5, 'B', ' ', Uuid())
        self.r6 = Residue('U', ')', 6, 'B', ' ', Uuid())

        name1 = generate_bp_name(self.r1, self.r6)
        name2 = generate_bp_name(self.r3, self.r4)
        name3 = generate_bp_name(self.r2, self.r5)

        self.bp1 = Basepair(self.r1.get_uuid(), self.r6.get_uuid(), Uuid(),
                           BasepairType.WC, SimpleString(name1))
        self.bp2 = Basepair(self.r3.get_uuid(), self.r4.get_uuid(), Uuid(),
                           BasepairType.WC, SimpleString(name2))
        self.bp3 = Basepair(self.r2.get_uuid(), self.r5.get_uuid(), Uuid(),
                           BasepairType.WC, SimpleString(name3))

        self.s = Structure([self.r1, self.r2, self.r3,
                            self.r4, self.r5, self.r6], [3, 6])
        bps = [self.bp3]
        ends = [self.bp1, self.bp2]
        self.end_ids = []
        for end in ends:
            self.end_ids.append(SimpleString(generate_end_id(self.s, bps, ends, end)))


        self.rs = RNAStructure(self.s, bps, ends, self.end_ids, SimpleString("test"))

    def test_improper_init(self):
        bps = []
        ends = [self.bp1, self.bp2]

        # need same number of ends and end_ids
        with self.assertRaises(RNAStructureException):
            RNAStructure(self.s, bps, ends, [], SimpleString("test"))

    def test_creation(self):
        self.failUnless(self.rs.get_num_chains() == 2)

    def test_get_residue(self):
        r = self.rs.get_residue(1, 'A', ' ')
        self.failUnless(self.r1 == r)

        r1 = self.rs.get_residue(r.get_uuid())
        self.failUnless(self.r1 == r1)

        # by internal position
        r2 = self.rs.get_residue(0)
        self.failUnless(self.r1 == r2)

    def test_get_end(self):

        # get by index
        end = self.rs.get_end(0)
        self.failUnless(end == self.bp1)

        # triyng to get an end that does not exist
        with self.assertRaises(RNAStructureException):
            self.rs.get_end(2)

        # get by name
        name = self.bp1.get_name()
        end = self.rs.get_end(name)
        self.failUnless(end == self.bp1)

        # trying to get with fake name
        with self.assertRaises(RNAStructureException):
            self.rs.get_end("FAKE")

        # by uuid
        end = self.rs.get_end(self.bp1.get_uuid())
        self.failUnless(end == self.bp1)

        with self.assertRaises(RNAStructureException):
            self.failUnless(self.rs.get_end(Uuid()))

        # by res ids
        end = self.rs.get_end(self.r1.get_uuid(), self.r6.get_uuid())
        self.failUnless(end == self.bp1)

        with self.assertRaises(RNAStructureException):
            self.failUnless(self.rs.get_end(Uuid(), Uuid()))

    def test_get_end_by_id(self):
        # by str
        self.rs.get_end_by_id(self.end_ids[0].get_str())

        # by simple str
        self.rs.get_end_by_id(self.end_ids[0])

    def test_get_basepairs(self):
        # get by uuid
        bps = self.rs.get_basepairs(self.bp3.get_uuid())
        self.failUnless(bps[0] == self.bp3)
        self.failUnless(len(bps) == 1)

        with self.assertRaises(RNAStructureException):
            self.failUnless(self.rs.get_basepairs(Uuid()))

        # by res ids
        bps = self.rs.get_basepairs(self.r2.get_uuid(), self.r5.get_uuid())
        self.failUnless(bps[0] == self.bp3)

        with self.assertRaises(RNAStructureException):
            self.failUnless(self.rs.get_basepairs(Uuid(), Uuid()))

        # get by name
        name = self.bp3.get_name()
        bps = self.rs.get_basepairs(name.get_str())
        self.failUnless(bps[0] == self.bp3)

    def test_get_basepair(self):
        # get by index
        bp = self.rs.get_basepair(0)
        self.failUnless(bp == self.bp3)

        # triyng to get an end that does not exist
        with self.assertRaises(RNAStructureException):
            self.rs.get_basepair(2)

        # get by name
        name = self.bp3.get_name().get_str()
        bp = self.rs.get_basepair(name)
        self.failUnless(bp == self.bp3)

        # trying to get with fake name
        with self.assertRaises(RNAStructureException):
            self.rs.get_basepair("FAKE")

        # by uuid
        bp = self.rs.get_basepair(self.bp3.get_uuid())
        self.failUnless(bp == self.bp3)

        with self.assertRaises(RNAStructureException):
            self.failUnless(self.rs.get_basepair(Uuid()))

        # by res ids
            bp = self.rs.get_basepair(self.r2.get_uuid(), self.r5.get_uuid())
        self.failUnless(bp == self.bp3)

        with self.assertRaises(RNAStructureException):
            self.failUnless(self.rs.get_basepair(Uuid(), Uuid()))

    def test_get_end_id(self):
        end_id = self.rs.get_end_id(0).get_str()
        self.failUnless(end_id == 'AGU_LLL_ACU_RRR')

        with self.assertRaises(RNAStructureException):
            self.rs.get_end_id(3)



def main():
    unittest.main()

if __name__ == '__main__':
    main()






