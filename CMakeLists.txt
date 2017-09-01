cmake_minimum_required(VERSION 2.8.12)
project(example)

set(CMAKE_BUILD_TYPE Release)
# forcing  -fPIC so we can make a shared object
set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ON)

include(compiler.cmake)
# Include path for Python header files

include_directories(/usr/include/python2.7)

include_directories(src/)
include_directories(unittests/)
include_directories(pybind11/include)

find_library(PYBIND11 pybind11)
set(PYBIND11_CPP_STANDARD -std=c++11)
add_subdirectory(pybind11)

############################################################################
# Base
############################################################################
set(base_files
        ./src/base/string.cc
        ./src/base/file_io.cc
        ./src/base/base.cc
        ./src/base/simple_string.cc
        )

add_library(base_lib ${base_files})
target_link_libraries(base_lib pybind11::module)

add_library(base_util MODULE ${base_files})
target_link_libraries(base_util PRIVATE pybind11::module)

############################################################################
# Math
############################################################################

set(math_files
        ./src/math/numerical.cc
        ./src/math/xyz_vector.cc
        ./src/math/xyz_matrix.cc
        ./src/math/math.cc
        )

add_library(math_util MODULE ${math_files})
target_link_libraries(math_util PRIVATE base_lib pybind11::module)

add_library(math_lib ${math_files})
target_link_libraries(math_lib base_lib pybind11::module)

add_executable(xyz_matrix_unittest unittests/xyz_matrix_unittest.cpp)
target_link_libraries(xyz_matrix_unittest math_lib)

############################################################################
# Util
############################################################################

set(util_files
        ./src/util/util.cc
        ./src/util/uuid.cc
        )

add_library(util MODULE ${util_files})
target_link_libraries(util PRIVATE math_lib pybind11::module )

add_library(util_lib ${util_files})
target_link_libraries(util_lib math_lib pybind11::module)

############################################################################
# Primitives
############################################################################

set(primitives_files
        ./src/primitives/residue.cc
        ./src/primitives/chain.cc
        ./src/primitives/structure.cc
        ./src/primitives/basepair.cc
        ./src/primitives/rna_structure.cc
        ./src/primitives/primitives.cc
        )

add_library(primitives MODULE ${primitives_files})
target_link_libraries(primitives PRIVATE util_lib pybind11::module )

add_library(primitives_lib ${primitives_files})
target_link_libraries(primitives_lib util_lib pybind11::module)

############################################################################
# All Atom
############################################################################

set(all_atom_files
        ./src/all_atom/atom.cc
        ./src/all_atom/all_atom.cc
        )

add_library(all_atom MODULE ${all_atom_files})
target_link_libraries(all_atom math_lib pybind11::module)

add_library(all_atom_lib ${all_atom_files})
target_link_libraries(all_atom_lib math_lib pybind11::module)




#pybind11_add_module(example)