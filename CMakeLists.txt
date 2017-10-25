cmake_minimum_required(VERSION 2.8.12)
project(rnamake_new)

set(CMAKE_BUILD_TYPE Release)
# forcing  -fPIC so we can make a shared object
set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ON)

include(/Users/jyesselm/projects/RNAMake.New/cmake/build/compiler.cmake)

# Include path for Python header files
include_directories(/usr/include/python2.7)

# Include paths for RNAMake src
include_directories(/Users/jyesselm/projects/RNAMake.New/src/)
include_directories(/Users/jyesselm/projects/RNAMake.New/unittests/)

# pybind11 setup 
find_library(PYBIND11 pybind11)
set(PYBIND11_CPP_STANDARD -std=c++11)
# requires pybind11 to be in local directory need to find workaround!
add_subdirectory(pybind11/)

find_package(Boost)

if (Boost_FOUND)
        set(Boost_USE_MULTITHREADED true)
        set(Boost_USE_STATIC_LIBS true)
        message(STATUS "Boost is ${BOOST_ROOT}")
        message(STATUS "Boost directories found at ${Boost_INCLUDE_DIR}")
        message(STATUS "Boost libraries found at ${Boost_LIBRARY_DIR}")
        set(Boost_LIBRARY_DIR "${BOOST_ROOT}/stage64/lib")
        include_directories(${Boost_INCLUDE_DIR})
        elseif()
        message("Boost NOT Found!")
    endif()###########################################################################
# base
###########################################################################

set(base_files
	/Users/jyesselm/projects/RNAMake.New/src/base/base.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/file_io.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/simple_string.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/string.cc
)

add_library(base_lib ${base_files})
target_link_libraries(base_lib pybind11::module)

add_library(base MODULE ${base_files})
target_link_libraries(base pybind11::module)

###########################################################################
# math
###########################################################################

set(math_files
	/Users/jyesselm/projects/RNAMake.New/src/math/math.cc
	/Users/jyesselm/projects/RNAMake.New/src/math/numerical.cc
	/Users/jyesselm/projects/RNAMake.New/src/math/xyz_matrix.cc
	/Users/jyesselm/projects/RNAMake.New/src/math/xyz_vector.cc
)

add_library(math_lib ${math_files})
target_link_libraries(math_lib base_lib  pybind11::module)

add_library(math MODULE ${math_files})
target_link_libraries(math base_lib  pybind11::module)

add_executable(xyz_matrix_unittest /Users/jyesselm/projects/RNAMake.New/unittests/math/xyz_matrix_unittest.cc)
target_link_libraries(xyz_matrix_unittest math_lib)

###########################################################################
# util
###########################################################################

set(util_files
	/Users/jyesselm/projects/RNAMake.New/src/util/util.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/uuid.cc
)

add_library(util_lib ${util_files})
target_link_libraries(util_lib math_lib  pybind11::module)

add_library(util MODULE ${util_files})
target_link_libraries(util math_lib  pybind11::module)

###########################################################################
# primitives
###########################################################################

set(primitives_files
	/Users/jyesselm/projects/RNAMake.New/src/primitives/basepair.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/chain.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/primitives.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/residue.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/rna_structure.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/structure.cc
)

add_library(primitives_lib ${primitives_files})
target_link_libraries(primitives_lib util_lib  pybind11::module)

add_library(primitives MODULE ${primitives_files})
target_link_libraries(primitives util_lib  pybind11::module)

add_executable(iterator_adaptor_unittest /Users/jyesselm/projects/RNAMake.New/unittests/primitives/iterator_adaptor_unittest.cc)
target_link_libraries(iterator_adaptor_unittest primitives_lib)

add_executable(primitive_memory_leak_unittest /Users/jyesselm/projects/RNAMake.New/unittests/primitives/primitive_memory_leak_unittest.cc)
target_link_libraries(primitive_memory_leak_unittest primitives_lib)

add_executable(primitives_unittest /Users/jyesselm/projects/RNAMake.New/unittests/primitives/primitives_unittest.cc)
target_link_libraries(primitives_unittest primitives_lib)

###########################################################################
# data_structures
###########################################################################

set(data_structures_files
	/Users/jyesselm/projects/RNAMake.New/src/data_structures/data_structures.cc
	/Users/jyesselm/projects/RNAMake.New/src/data_structures/graph.cc
)

add_library(data_structures_lib ${data_structures_files})
target_link_libraries(data_structures_lib util_lib  pybind11::module)

add_library(data_structures MODULE ${data_structures_files})
target_link_libraries(data_structures util_lib  pybind11::module)

add_executable(graph_unittest /Users/jyesselm/projects/RNAMake.New/unittests/data_structures/graph_unittest.cc)
target_link_libraries(graph_unittest data_structures_lib)

