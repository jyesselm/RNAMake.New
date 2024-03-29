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
include_directories(/Users/jyesselm/projects/RNAMake.New/src/plog/)
include_directories(/Users/jyesselm/projects/RNAMake.New/unittests/)

# pybind11 setup 
find_library(PYBIND11 pybind11)
set(PYBIND11_CPP_STANDARD -std=c++11)
# requires pybind11 to be in local directory need to find workaround!
add_subdirectory(pybind11/)

# sqlite libraries
find_library(SQLITE3_LIBRARY sqlite3)

find_package(ZLIB)

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
	/Users/jyesselm/projects/RNAMake.New/src/base/file_io.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/string.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/backtrace.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/json.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/base.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/simple_string.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/paths.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/log.cc
	/Users/jyesselm/projects/RNAMake.New/src/base/option.cc
)

add_library(base_lib ${base_files})
target_link_libraries(base_lib ${ZLIB_LIBRARY} pybind11::module)

add_library(base MODULE ${base_files})
target_link_libraries(base ${ZLIB_LIBRARY} pybind11::module)

add_executable(logger_unittest /Users/jyesselm/projects/RNAMake.New/unittests/base/logger_unittest.cc)
target_link_libraries(logger_unittest base_lib)

add_executable(compression_unittest /Users/jyesselm/projects/RNAMake.New/unittests/base/compression_unittest.cc)
target_link_libraries(compression_unittest base_lib)

add_executable(option_unittest /Users/jyesselm/projects/RNAMake.New/unittests/base/option_unittest.cc)
target_link_libraries(option_unittest base_lib)

###########################################################################
# math
###########################################################################

set(math_files
	/Users/jyesselm/projects/RNAMake.New/src/math/math.cc
	/Users/jyesselm/projects/RNAMake.New/src/math/quaternion.cc
	/Users/jyesselm/projects/RNAMake.New/src/math/xyz_vector.cc
	/Users/jyesselm/projects/RNAMake.New/src/math/xyz_matrix.cc
	/Users/jyesselm/projects/RNAMake.New/src/math/numerical.cc
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
	/Users/jyesselm/projects/RNAMake.New/src/util/segment_type.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/uuid.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/util.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/bead.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/x3dna.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/random_number_generator.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/sqlite/connection.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/sqlite/database.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/sqlite/table_details.cc
	/Users/jyesselm/projects/RNAMake.New/src/util/sqlite/field.cc
)

add_library(util_lib ${util_files})
target_link_libraries(util_lib math_lib  ${SQLITE3_LIBRARY} pybind11::module)

add_library(util MODULE ${util_files})
target_link_libraries(util math_lib  ${SQLITE3_LIBRARY} pybind11::module)

add_executable(x3dna_on_all_pdbs_unittest /Users/jyesselm/projects/RNAMake.New/unittests/util/x3dna_on_all_pdbs_unittest.cc)
target_link_libraries(x3dna_on_all_pdbs_unittest util_lib)

add_executable(x3dna_unittest /Users/jyesselm/projects/RNAMake.New/unittests/util/x3dna_unittest.cc)
target_link_libraries(x3dna_unittest util_lib)

add_executable(sqlite3_connection_unittest /Users/jyesselm/projects/RNAMake.New/unittests/util/sqlite3_connection_unittest.cc)
target_link_libraries(sqlite3_connection_unittest util_lib)

add_executable(json_unittest /Users/jyesselm/projects/RNAMake.New/unittests/util/json_unittest.cc)
target_link_libraries(json_unittest util_lib)

###########################################################################
# primitives
###########################################################################

set(primitives_files
	/Users/jyesselm/projects/RNAMake.New/src/primitives/segment.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/residue.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/basepair.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/structure.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/pose.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/chain.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/primitives.cc
	/Users/jyesselm/projects/RNAMake.New/src/primitives/aligner.cc
)

add_library(primitives_lib ${primitives_files})
target_link_libraries(primitives_lib util_lib  pybind11::module)

add_library(primitives MODULE ${primitives_files})
target_link_libraries(primitives util_lib  pybind11::module)

add_executable(primitives_unittest /Users/jyesselm/projects/RNAMake.New/unittests/primitives/primitives_unittest.cc)
target_link_libraries(primitives_unittest primitives_lib)

add_executable(primitive_memory_leak_unittest /Users/jyesselm/projects/RNAMake.New/unittests/primitives/primitive_memory_leak_unittest.cc)
target_link_libraries(primitive_memory_leak_unittest primitives_lib)

###########################################################################
# data_structures
###########################################################################

set(data_structures_files
	/Users/jyesselm/projects/RNAMake.New/src/data_structures/data_structures.cc
	/Users/jyesselm/projects/RNAMake.New/src/data_structures/graph.cc
	/Users/jyesselm/projects/RNAMake.New/src/data_structures/graph_adjacency_list.cc
	/Users/jyesselm/projects/RNAMake.New/src/data_structures/graph_base.cc
	/Users/jyesselm/projects/RNAMake.New/src/data_structures/graph_iter_list.cc
)

add_library(data_structures_lib ${data_structures_files})
target_link_libraries(data_structures_lib util_lib  pybind11::module)

add_library(data_structures MODULE ${data_structures_files})
target_link_libraries(data_structures util_lib  pybind11::module)

add_executable(graph_adjacency_list_unittest /Users/jyesselm/projects/RNAMake.New/unittests/data_structures/graph_adjacency_list_unittest.cc)
target_link_libraries(graph_adjacency_list_unittest data_structures_lib)

add_executable(graph_unittest /Users/jyesselm/projects/RNAMake.New/unittests/data_structures/graph_unittest.cc)
target_link_libraries(graph_unittest data_structures_lib)

add_executable(graph_iter_list_unittest /Users/jyesselm/projects/RNAMake.New/unittests/data_structures/graph_iter_list_unittest.cc)
target_link_libraries(graph_iter_list_unittest data_structures_lib)

###########################################################################
# secondary_structure
###########################################################################

set(secondary_structure_files
	/Users/jyesselm/projects/RNAMake.New/src/secondary_structure/segment.cc
	/Users/jyesselm/projects/RNAMake.New/src/secondary_structure/residue.cc
	/Users/jyesselm/projects/RNAMake.New/src/secondary_structure/parser.cc
	/Users/jyesselm/projects/RNAMake.New/src/secondary_structure/basepair.cc
	/Users/jyesselm/projects/RNAMake.New/src/secondary_structure/structure.cc
	/Users/jyesselm/projects/RNAMake.New/src/secondary_structure/pose.cc
	/Users/jyesselm/projects/RNAMake.New/src/secondary_structure/chain.cc
	/Users/jyesselm/projects/RNAMake.New/src/secondary_structure/secondary_structure.cc
	/Users/jyesselm/projects/RNAMake.New/src/secondary_structure/aligner.cc
)

add_library(secondary_structure_lib ${secondary_structure_files})
target_link_libraries(secondary_structure_lib primitives_lib  pybind11::module)

add_library(secondary_structure MODULE ${secondary_structure_files})
target_link_libraries(secondary_structure primitives_lib  pybind11::module)

add_executable(parser_unittest /Users/jyesselm/projects/RNAMake.New/unittests/secondary_structure/parser_unittest.cc)
target_link_libraries(parser_unittest secondary_structure_lib)

###########################################################################
# all_atom
###########################################################################

set(all_atom_files
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/segment.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/segment_factory.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/atom.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/residue_type_set.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/residue.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/basepair.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/structure.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/pdb_parser.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/pose.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/chain.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/aligner.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/residue_type.cc
	/Users/jyesselm/projects/RNAMake.New/src/all_atom/all_atom.cc
)

add_library(all_atom_lib ${all_atom_files})
target_link_libraries(all_atom_lib secondary_structure_lib  pybind11::module)

add_library(all_atom MODULE ${all_atom_files})
target_link_libraries(all_atom secondary_structure_lib  pybind11::module)

add_executable(all_atom_generate_all_segments /Users/jyesselm/projects/RNAMake.New/unittests/all_atom/all_atom_generate_all_segments.cc)
target_link_libraries(all_atom_generate_all_segments all_atom_lib)

add_executable(pdb_parser_unittest /Users/jyesselm/projects/RNAMake.New/unittests/all_atom/pdb_parser_unittest.cc)
target_link_libraries(pdb_parser_unittest all_atom_lib)

add_executable(all_atom_basepair_unittest /Users/jyesselm/projects/RNAMake.New/unittests/all_atom/all_atom_basepair_unittest.cc)
target_link_libraries(all_atom_basepair_unittest all_atom_lib)

add_executable(parse_all_pdbs_unittest /Users/jyesselm/projects/RNAMake.New/unittests/all_atom/parse_all_pdbs_unittest.cc)
target_link_libraries(parse_all_pdbs_unittest all_atom_lib)

add_executable(all_pdbs_to_poses_unittest /Users/jyesselm/projects/RNAMake.New/unittests/all_atom/all_pdbs_to_poses_unittest.cc)
target_link_libraries(all_pdbs_to_poses_unittest all_atom_lib)

add_executable(all_atom_segment_unittest /Users/jyesselm/projects/RNAMake.New/unittests/all_atom/all_atom_segment_unittest.cc)
target_link_libraries(all_atom_segment_unittest all_atom_lib)

add_executable(all_atom_structure_unittest /Users/jyesselm/projects/RNAMake.New/unittests/all_atom/all_atom_structure_unittest.cc)
target_link_libraries(all_atom_structure_unittest all_atom_lib)

add_executable(all_atom_pose_unittest /Users/jyesselm/projects/RNAMake.New/unittests/all_atom/all_atom_pose_unittest.cc)
target_link_libraries(all_atom_pose_unittest all_atom_lib)

add_executable(all_atom_basic_unittest /Users/jyesselm/projects/RNAMake.New/unittests/all_atom/all_atom_basic_unittest.cc)
target_link_libraries(all_atom_basic_unittest all_atom_lib)

###########################################################################
# resource_management
###########################################################################

set(resource_management_files
	/Users/jyesselm/projects/RNAMake.New/src/resource_management/segment_sqlite_library.cc
	/Users/jyesselm/projects/RNAMake.New/src/resource_management/sqlite_library.cc
	/Users/jyesselm/projects/RNAMake.New/src/resource_management/resource_manager.cc
	/Users/jyesselm/projects/RNAMake.New/src/resource_management/resource_builder.cc
)

add_library(resource_management_lib ${resource_management_files})
target_link_libraries(resource_management_lib all_atom_lib  pybind11::module)

add_library(resource_management MODULE ${resource_management_files})
target_link_libraries(resource_management all_atom_lib  pybind11::module)

add_executable(resource_manager_unittest /Users/jyesselm/projects/RNAMake.New/unittests/resource_management/resource_manager_unittest.cc)
target_link_libraries(resource_manager_unittest resource_management_lib)

add_executable(resource_builder_unittest /Users/jyesselm/projects/RNAMake.New/unittests/resource_management/resource_builder_unittest.cc)
target_link_libraries(resource_builder_unittest resource_management_lib)

add_executable(sqlite_library_unittest /Users/jyesselm/projects/RNAMake.New/unittests/resource_management/sqlite_library_unittest.cc)
target_link_libraries(sqlite_library_unittest resource_management_lib)

###########################################################################
# segment_data_structures
###########################################################################

set(segment_data_structures_files
	/Users/jyesselm/projects/RNAMake.New/src/segment_data_structures/sequence_updater.cc
	/Users/jyesselm/projects/RNAMake.New/src/segment_data_structures/segment_graph.cc
	/Users/jyesselm/projects/RNAMake.New/src/segment_data_structures/segment_merger.cc
)

add_library(segment_data_structures_lib ${segment_data_structures_files})
target_link_libraries(segment_data_structures_lib resource_management_lib  pybind11::module)

add_library(segment_data_structures MODULE ${segment_data_structures_files})
target_link_libraries(segment_data_structures resource_management_lib  pybind11::module)

add_executable(segment_merger_unittest /Users/jyesselm/projects/RNAMake.New/unittests/segment_data_structures/segment_merger_unittest.cc)
target_link_libraries(segment_merger_unittest segment_data_structures_lib)

add_executable(sequence_updater_unittest /Users/jyesselm/projects/RNAMake.New/unittests/segment_data_structures/sequence_updater_unittest.cc)
target_link_libraries(sequence_updater_unittest segment_data_structures_lib)

add_executable(segment_graph_unittest /Users/jyesselm/projects/RNAMake.New/unittests/segment_data_structures/segment_graph_unittest.cc)
target_link_libraries(segment_graph_unittest segment_data_structures_lib)

