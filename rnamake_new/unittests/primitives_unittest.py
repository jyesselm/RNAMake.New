import unittest
import numpy as np

from rnamake_new.util import *
from rnamake_new.primitives import *
from rnamake_new.base import *


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

    def test_creation(self):
        # must have at least one residue
        with self.assertRaises(ChainException):
            Chain([])

    def test_copy(self):
        Chain(self.c)

    def test_first_and_last(self):
        self.failUnless(self.c.get_first() == self.r1)
        self.failUnless(self.c.get_last()  == self.r3)


    """def test_inherit(self):
        r1 = NewRes('A', 1, 'A', ' ', Uuid())
        r2 = NewRes('A', 2, 'A', ' ', Uuid())

        new_c = Chain([r1, r2])
        self.failUnless(new_c.get_residue(0).unique() == 1)"""

    def test_iterator(self):
        count = 0
        for r in self.c:
            count += 1
        self.failUnless(count == 3)


class StructureUnittest(unittest.TestCase):

    def setUp(self):
        self.r1 = Residue('A', 1, 'A', ' ', Uuid())
        self.r2 = Residue('A', 2, 'A', ' ', Uuid())
        self.r3 = Residue('A', 2, 'A', ' ', Uuid())
        self.s = Structure([self.r1, self.r2, self.r3], [3])

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

    def test_getters(self):
        self.failUnless(self.s.get_num_chains() == 1)
        self.failUnless(self.s.get_num_residues() == 3)
        self.failUnless(self.s.get_sequence() == "AAA")
        self.failUnless(self.s.get_res_index(self.r1) == 0)

    def test_multi_chains(self):
        r1 = Residue('A', 1, 'A', ' ', Uuid())
        r2 = Residue('U', 2, 'A', ' ', Uuid())
        r3 = Residue('A', 3, 'B', ' ', Uuid())
        r4 = Residue('U', 4, 'B', ' ', Uuid())
        s = Structure([r1, r2, r3, r4], [2, 4])

        self.failUnless(s.get_num_chains() == 2)
        chains = s.get_chains()
        self.failUnless(len(chains) == 2)
        self.failUnless(chains[1].get_first() == r3)


class BasepairUnittest(unittest.TestCase):

    def setUp(self):
        self.r1 = Residue('A', 1, 'A', ' ', Uuid())
        self.r2 = Residue('U', 2, 'A', ' ', Uuid())
        name = generate_bp_name(self.r1, self.r2)
        self.bp = Basepair(self.r1.get_uuid(), self.r2.get_uuid(), Uuid(),
                           BasepairType.WC, SimpleString(name))

    def test_getters(self):
        self.failUnless(self.r1.get_uuid() == self.bp.get_res1_uuid())
        self.failUnless(self.r2.get_uuid() == self.bp.get_res2_uuid())
        self.failUnless(self.bp.get_bp_type() == BasepairType.WC)

    def test_generate_bp_name(self):
        r1 = Residue('A', 1, 'A', ' ', Uuid())
        r2 = Residue('U', 2, 'A', ' ', Uuid())
        name = generate_bp_name(r1, r2)
        self.failUnless(name == 'A1-A2')

        r1 = Residue('A', 2, 'A', ' ', Uuid())
        r2 = Residue('U', 1, 'B', ' ', Uuid())
        name = generate_bp_name(r1, r2)
        self.failUnless(name == 'A2-B1'
                        )
        # not order dependent
        name = generate_bp_name(r2, r1)
        self.failUnless(name == 'A2-B1')

        r1 = Residue('A', 99, 'A', ' ', Uuid())
        r2 = Residue('U', 100, 'A', ' ', Uuid())
        name = generate_bp_name(r1, r2)
        self.failUnless(name == 'A99-A100')

        r1 = Residue('A', 2, 'A', 'C', Uuid())
        r2 = Residue('U', 1, 'B', ' ', Uuid())
        name = generate_bp_name(r1, r2)
        self.failUnless(name == 'A2C-B1')

        # is dervived classes still okay?
        r1 = NewRes('A', 2, 'A', 'C', Uuid())
        r2 = NewRes('U', 1, 'B', ' ', Uuid())
        name = generate_bp_name(r1, r2)
        self.failUnless(name == 'A2C-B1')


class RNAStructureUnittest(unittest.TestCase):

    def setUp(self):
        self.r1 = Residue('A', 1, 'A', ' ', Uuid()) # 0
        self.r2 = Residue('G', 2, 'A', ' ', Uuid()) # 1
        self.r3 = Residue('U', 3, 'A', ' ', Uuid()) # 2
        self.r4 = Residue('A', 4, 'B', ' ', Uuid()) # 3
        self.r5 = Residue('C', 5, 'B', ' ', Uuid()) # 4
        self.r6 = Residue('U', 6, 'B', ' ', Uuid()) # 5

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
        self.all_bps = [self.bp1, self.bp2, self.bp3]
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

    def test_generate_end_id(self):
        bps = []
        ends = [self.bp1, self.bp2]
        end_ids = []
        for end in ends:
            end_ids.append(generate_end_id(self.s, bps, ends, end))

        self.failUnless(end_ids[0] == 'AGU_LUL_ACU_RUR')

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

    def test_ends_from_basepairs(self):
        ends = get_ends_from_basepairs(self.s, self.all_bps)
        ends = ends.get_data()



def main():
    unittest.main()

if __name__ == '__main__':
    main()













































