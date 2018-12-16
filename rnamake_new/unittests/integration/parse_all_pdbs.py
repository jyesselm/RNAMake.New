import glob
from rnamake_new.all_atom import *

if __name__ == '__main__':
    rts = ResidueTypeSet()
    pdb_parser = PDBParser(rts)

    path = '/Users/jyesselm/projects/REDESIGN/resources/non-redundant-rnas'
    dirs = glob.glob(path+"/*")
    count = 0
    for d in dirs:
        spl = d.split("/")
        if len(spl[-1]) != 4:
            continue
        count += 1
        residues = pdb_parser.parse(d + "/" + spl[-1] + ".pdb")
