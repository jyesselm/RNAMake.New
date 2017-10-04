import os
import fnmatch
import argparse

depends = {
    'base' : [],
    'math' : ['base'],
    'util' : ['math'],
    'primitives' : ['util']
}

libs = "base math util primitives"
all_lib_paths = libs.split()


file_path = os.path.realpath(__file__)
spl = file_path.split("/")
base_dir = "/".join(spl[:-2])


def parse_args():
    parser = argparse.ArgumentParser()

    args = parser.parse_args()

    return args


def get_lib_paths(args):
    return all_lib_paths


def get_cmake_lists_header():
    s =  "cmake_minimum_required(VERSION 2.8.12)\n"
    s += "project(rnamake_new)\n\n"
    s += "set(CMAKE_BUILD_TYPE Release)\n"
    s += "# forcing  -fPIC so we can make a shared object\n"
    s += "set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ON)\n\n"
    s += "include(%s)\n\n" % (base_dir + "/cmake/build/compiler.cmake")
    s += "# Include path for Python header files\n"
    s += "include_directories(/usr/include/python2.7)\n\n"
    s += "# Include paths for RNAMake src\n"
    s += "include_directories(%s)\n" % (base_dir + "/src/")
    s += "include_directories(%s)\n\n" % (base_dir + "/unittests/")
    s += "# pybind11 setup \n"
    s += "find_library(PYBIND11 pybind11)\n"
    s += "set(PYBIND11_CPP_STANDARD -std=c++11)\n"
    s += "# requires pybind11 to be in local directory need to find workaround!\n"
    s += "add_subdirectory(pybind11/)\n\n"
    return s


def get_pretty_lib_name(name):
    s = ""
    for i in range(75):
        s += "#"
    s += "\n"
    s += "# " + name + "\n"
    for i in range(75):
        s += "#"
    s += "\n\n"
    return s


def get_lib_file_declaration(lib):
    abs_path = base_dir + "/src/" + lib + "/"
    cc_files = get_cc_files_in_dir(abs_path)
    s  = "set(%s_files\n" % (lib)
    for cc_file in cc_files:
        s += "\t" + abs_path + cc_file + "\n"
    s += ")\n\n"
    return s


def get_build_library_declaration(lib):
    s  = "add_library(%s ${%s})\n" % (lib + "_lib", lib + "_files")
    s += "target_link_libraries(%s" % (lib + "_lib")
    for depend in depends[lib]:
        s += " " + depend + "_lib "
    s += " pybind11::module)\n\n"
    return s

def get_build_module_declaration(lib):
    s  = "add_library(%s MODULE ${%s})\n" % (lib, lib + "_files")
    s += "target_link_libraries(%s" % (lib)
    for depend in depends[lib]:
        s += " " + depend + "_lib "
    s += " pybind11::module)\n\n"

    """s += "add_custom_command(\n"
    s += "\t TARGET " + lib + "_copy POST_BUILD COMMAND " + \
         "bash -c \"cp lib" + lib + ".so " +\
         base_dir + "/rnamake_new/" + lib + ".so\"\n"
    s += ")\n\n"
    """
    return s


def write_cmake_lists(path, args):
    f = open(path+"/CMakeLists.txt", "w")
    f.write(get_cmake_lists_header())

    lib_paths = get_lib_paths(args)
    for lib in lib_paths:
        f.write(get_pretty_lib_name(lib))
        f.write(get_lib_file_declaration(lib))
        f.write(get_build_library_declaration(lib))
        f.write(get_build_module_declaration(lib))

    f.close()


def get_cc_files_in_dir(path):
    cpp_files = []
    for root, dirnames, filenames in os.walk(path):
            for filename in filenames:
                if filename[-2:] == 'cc' or filename[-3:] == 'cpp':
                    cpp_files.append(filename)
    return cpp_files


def write_application_cmake_file():

    fsum = open(CMAKE_PATH+"apps.cmake", "w")

    f = open(CMAKE_PATH+"apps.txt")
    lines = f.readlines()
    f.close()
    for l in lines:
        if l[0] == "#":
         continue

        spl = l.split()
        cc_path = util.base_dir(spl[1])
        app_dir = cc_path
        cpp_files = get_cc_files_in_dir(app_dir)

        symlink = spl[0]+"_symlink"
        fsum.write("add_executable("+spl[0] + " ")
        for f in cpp_files:
            fsum.write(app_dir + "/" + f + " ")
        fsum.write(")\n")
        fsum.write("target_link_libraries("+spl[0]+" all_libs)\n")
        fsum.write("add_custom_target("+symlink+" ALL)\n")
        fsum.write("add_custom_command(TARGET "+symlink+" POST_BUILD COMMAND cmake -E create_symlink "+BIN_PATH+spl[0]+" "+ spl[0]+")\n")
        fsum.write("\n\n")

    fsum.close()


def get_unittests_for_dir(d_name):
    unittest_apps = []

    if not os.path.isdir('../../unittests/'+d_name):
        return []

    for root, dirnames, filenames in os.walk('../../unittests/'+d_name):
        for filename in fnmatch.filter(filenames, '*.c*'):
            path = os.path.join(root, filename)

            f = open(path)
            fail = 0
            for l in f.readlines():
                if len(l) < 8:
                    continue
                if l[:9] == 'TEST_CASE':
                    unittest_apps.append((os.path.join(root, filename)))
                    fail = 1
                    break

    return unittest_apps


if __name__ == '__main__':
    args = parse_args()
    write_cmake_lists("./", args)



"""
c_dir = settings.LIB_PATH + "/lib/RNAMake/"




f = open("build.cmake", "w")

for p in lib_paths:

    f.write("set("+p+"_files\n")
    for root, dirnames, filenames in os.walk('../../src/'+p):
        for filename in filenames:
            if filename[-2:] == 'cc' or filename[-3:] == 'cpp':
                f.write("\t"+os.path.join(root, filename)+"\n")
    f.write(")\n")

    f.write("add_library(%s SHARED ${%s_files})\n" % (p, p))
    f.write("target_link_libraries(%s %s)\n\n" % (p, depends[p]))

    unittest_apps = get_unittests_for_dir(p)
    for unit_app in unittest_apps:
        fname = util.filename(unit_app)
        spl = fname.split(".")
        prog_name = spl[0]

        symlink = prog_name+"_symlink"
        f.write("add_executable("+ prog_name + " " + unit_app +")\n")
        f.write("target_link_libraries("+prog_name+" %s)\n" % p)
        f.write("add_custom_target("+symlink+" ALL)\n")
        f.write("add_custom_command(TARGET "+symlink+" POST_BUILD COMMAND ./symlink.py "+prog_name + " " + BIN_PATH + "unittests/"+prog_name+")\n")
        f.write("\n\n")

    integration_apps = get_integration_tests_for_dir(p)
    for inte_app in integration_apps:
        fname = util.filename(inte_app)
        spl = fname.split(".")
        prog_name = spl[0]

        symlink = prog_name+"_symlink"
        f.write("add_executable("+ prog_name + " " + inte_app +")\n")
        f.write("target_link_libraries("+prog_name+" %s)\n" % p)
        f.write("add_custom_target("+symlink+" ALL)\n")
        f.write("add_custom_command(TARGET "+symlink+" POST_BUILD COMMAND ./symlink.py "+prog_name + " " + BIN_PATH +  "integration/"+prog_name+")\n")
        f.write("\n\n")

    #compile unittests for this lib

    #f.write("target_link_libraries ( " + )

f.write("add_library(all_libs SHARED ../../src/main.cpp)\n")
f.write("target_link_libraries(all_libs %s)\n\n" % (depends['all_libs']))

for p in ['apps']:

    unittest_apps = get_unittests_for_dir(p)
    for unit_app in unittest_apps:
        fname = util.filename(unit_app)
        spl = fname.split(".")
        prog_name = spl[0]
        spl2 = prog_name.split("_")
        spl2.pop()
        act_name = "_".join(spl2)
        act_prog_dir = "../../apps/"+act_name+"/"
        #print act_prog_dir
        cc_files = get_cc_files_in_dir(act_prog_dir)
        final_cc_files = []
        for cc_file in cc_files:
            f_cc = open(act_prog_dir+cc_file)
            lines = f_cc.readlines()
            f_cc.close()

            found = 0
            for l in lines:
                if l[0:8] == "int main":
                    found = 1
                    break
            if not found:
                final_cc_files.append(cc_file)

        symlink = prog_name+"_symlink"
        f.write("add_executable("+prog_name + " " + unit_app + " ")
        for fname in final_cc_files:
            f.write(act_prog_dir + "/" + fname + " ")
        f.write(")\n")

        f.write("target_link_libraries("+prog_name+" %s)\n" % 'all_libs')
        f.write("add_custom_target("+symlink+" ALL)\n")
        f.write("add_custom_command(TARGET "+symlink+" POST_BUILD COMMAND ./symlink.py "+prog_name + " " + BIN_PATH + "unittests/"+prog_name+")\n")
        f.write("\n\n")

f.close()

write_application_cmake_file()
"""