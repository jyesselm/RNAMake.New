
#include <iostream>
#include "../common.hpp"

#include <base/file_io.h>
#include <math/xyz_matrix.h>
#include <math/numerical.h>
#include <math/quaternion.h>


TEST_CASE( "Test Matrix math ", "[XYZMatrix]" ) {
    
    SECTION("Test Stringify Matrices") {
        auto m = math::Matrix(5.0);
        auto s = m.get_str();
        auto m2 = math::Matrix(s);
        
        REQUIRE(math::are_matrices_equal(m, m2));
    }
    

    SECTION("Single known test of unitarize compared to python") {
    
        auto path ="/Users/jyesselm/projects/RNAMake.New/rnamake_new/unittests/resources/math/test_unitarize.dat";
        auto lines = base::get_lines_from_file(path);
        auto org_m = math::Matrix(lines[0]);
        
        auto m = math::Matrix(1.0, 2.0, 3.0,
                              4.0, 5.0, 6.0,
                              1.0, 1.0, 1.0);
    
        auto unit = m.get_unitarize();
        REQUIRE(math::are_matrices_equal(org_m, unit));
        
    }

    SECTION("test random quaterion") {
        auto q = math::get_random_quaternion();
    }

    SECTION("test random matrix") {
        auto m = math::get_random_rotation_matrix();
        std::cout << m.get_str_readable() << std::endl;
    }

    /*SECTION("Test unitarize in batch with 1000 matrices") {
        auto path = unittest_resource_dir() + "/math/test_unitarize_multi.dat";
        auto lines = get_lines_from_file(path);
        
        int fail = 0;
        for(auto const & l : lines) {
            if(l.length() < 10) { break; }
            auto spl = split_str_by_delimiter(l, "|");
            auto org_m = Matrix(spl[0]);
            auto final_m = Matrix(spl[1]);
            
            auto unit = org_m.get_unitarize();
            
            if(! are_xyzMatrix_equal(final_m, unit)) {
                fail = 1;
                break;
            }
            
            org_m.unitarize();
            
            if(! are_xyzMatrix_equal(final_m, org_m)) {
                fail = 1;
                break;
            }
            
            
        }

        REQUIRE(fail == 0);

    }*/
    
}