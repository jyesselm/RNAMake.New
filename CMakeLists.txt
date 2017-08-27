cmake_minimum_required(VERSION 2.8.12)
project(example)

set(CMAKE_BUILD_TYPE Release)
# forcing  -fPIC so we can make a shared object
set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ON)

include(compiler.cmake)
# Include path for Python header files

include_directories(/usr/include/python2.7)

include_directories(src/)
include_directories(pybind11/include)

find_library(PYBIND11 pybind11)

set(base_files
        ./src/base/string.cc
        )

add_library(base_lib SHARED ${base_files})
target_link_libraries(base_lib )

set(math_files
        ./src/math/math.cc
        )

set(PYBIND11_CPP_STANDARD -std=c++11)
add_subdirectory(pybind11)

pybind11_add_module(math_util MODULE ${math_files})

add_library(math_lib ${math_files})
target_link_libraries(math_lib base_lib pybind11::module)


set(all_atom_files
        ./src/all_atom/atom.cc
        ./src/all_atom/all_atom.cc
        )

add_library(all_atom_lib ${all_atom_files})
pybind11_add_module(all_atom MODULE ${all_atom_files})


#pybind11_add_module(example)