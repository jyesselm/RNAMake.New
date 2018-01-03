
#include <iostream>
#include <bitset>

#include "../common.hpp"

#include <base/file_io.h>
#include <math/xyz_vector.h>
#include <math/xyz_matrix.h>
#include <math/numerical.h>
#include <math/quaternion.h>


TEST_CASE( "Test Matrix math ", "[XYZMatrix]" ) {

    union FloatValue {
        float f;
        struct {
            uint8_t ut1, ut2, ut3, ut4;
        } bytes;
    };

    union DoubleValue {
        double d;
        struct {
            uint8_t ut1, ut2, ut3, ut4, ut5, ut6, ut7, ut8;
        } bytes;
    };

    auto p = math::Point(1, 2, 3);

    /*float d = 2.54;
    double d1 = 2.64;
    unsigned char *array;
    array = reinterpret_cast<unsigned char *>(&d);
    std::cout << array[0] << " " << array[1] << " " << array[2] << " " << array[3] << std::endl;

    //std::cout << reinterpret_cast<float>(&array) << std::endl;
    std::cout << sizeof(d) << " " << sizeof(d1) << std::endl;*/

    SECTION("test float") {

        FloatValue v;
        v.f = 1.1f;
        //std::cout << v.bytes.ut1 << v.bytes.ut2 << v.bytes.ut3 << v.bytes.ut4 << std::endl;

        FloatValue v2;
        v2.bytes = v.bytes;
        REQUIRE(v.f == v2.f);
    }

    SECTION("test double") {
        DoubleValue v, v2;
        v.d = 0.403966;

        v2.bytes = v.bytes;

        std::ofstream out ("test.out" , std::ios::binary);
        out.write(reinterpret_cast<char*>(&v.d), sizeof(v.d));
        out << std::endl;
        //out << v.bytes.ut1 << v.bytes.ut2 << v.bytes.ut3 << v.bytes.ut4 << v.bytes.ut5 << v.bytes.ut6 << v.bytes.ut7 << v.bytes.ut8 << std::endl;
        out.close();

    }

}