#!/usr/bin/env python

import glob
import subprocess
import os
import argparse

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-tests_until', default="segment_data_structures")
    args = parser.parse_args()

    return args

def run_tests(tests):
    for test in tests:

        try:
            lines = subprocess.check_output("./" + test, shell=True).split("\n")
        except subprocess.CalledProcessError as e:
            print e.output
            continue

        # everything passed
        if len(lines) < 6:
            print test, lines[1] + "\n",
            continue
        elif lines[-3][0:3] == "All":
            print test, lines[-3] + "\n",
            continue
        else:
            print test, ": FAIL"
            subprocess.call(test, shell=True)

libs = "base math util primitives data_structures secondary_structure all_atom resource_management segment_data_structures".split()

unittests = {
    'base' : [
        'option_unittest'],

    'math' : [
        'xyz_matrix_unittest'],

    'util' : [
        'json_unittest',
        'x3dna_unittest',
        'sqlite3_connection_unittest'],

    'primitives' : [
        'primitives_unittest'],

    'data_structures' : [
        'graph_adjacency_list_unittest',
        'graph_iter_list_unittest',
        'graph_unittest'],

    'secondary_structure' : [
        'parser_unittest'],

    'all_atom' : [
        'all_atom_basepair_unittest',
        'all_atom_structure_unittest',
        'all_atom_basic_unittest',
        'all_atom_pose_unittest',
        'pdb_parser_unittest'],

    'resource_management' : [
        'resource_manager_unittest',
        'sqlite_library_unittest'
    ],

    'segment_data_structures' : [
        'segment_graph_unittest',
        'segment_merger_unittest'
    ]
}

args = parse_args()
tests_util = args.tests_until
for lib in libs:
    tests = unittests[lib]
    run_tests(tests)

    if lib == tests_util:
        break



exit()

