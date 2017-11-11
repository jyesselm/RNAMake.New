import shutil
import os

file_path = os.path.realpath(__file__)
spl = file_path.split("/")
base_dir = "/".join(spl[:-2])

shutil.copy("libutil.so", base_dir+"/rnamake_new/util.so")
shutil.copy("libprimitives.so",  base_dir+"/rnamake_new/primitives.so")
shutil.copy("libbase.so",  base_dir+"/rnamake_new/base.so")
shutil.copy("libmath.so",  base_dir+"/rnamake_new/math_util.so")
shutil.copy("libsecondary_structure.so", base_dir+"/rnamake_new/secondary_structure.so")
shutil.copy("libdata_structures.so", base_dir+"/rnamake_new/data_structures.so")
shutil.copy("liball_atom.so", base_dir+"/rnamake_new/all_atom.so")