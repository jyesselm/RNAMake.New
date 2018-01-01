//
// Created by Joseph Yesselman on 12/7/17.
//

#include <iostream>
#include <zlib.h>
#include "../common.hpp"

#include <base/paths.h>
#include <base/json.h>
#include <base/file_io.h>
#include <math/xyz_vector.h>
#include <util/random_number_generator.h>

void
with_stringify(
        util::RandomNumberGenerator & rng,
        std::vector<std::vector<double>> const & values) {

    auto p = math::Point();
    auto p2 = math::Point();
    auto s = String();
    auto d = 0;
    auto pos = 0;
    for(int i = 0; i < 10000000; i++) {
        pos = rng.randrange(100);
        p.set_x(values[pos][0]);
        p.set_x(values[pos][1]);
        p.set_x(values[pos][1]);

        p2 = math::Point(p.get_str());

        d = p.distance(p2);

    }


}

void
with_json(
        util::RandomNumberGenerator & rng,
        std::vector<std::vector<double>> const & values) {

    auto p = math::Point();
    auto p2 = math::Point();
    auto j = json::JSON();
    auto d = 0;
    auto pos = 0;
    for(int i = 0; i < 10000000; i++) {
        pos = rng.randrange(100);
        p.set_x(values[pos][0]);
        p.set_x(values[pos][1]);
        p.set_x(values[pos][1]);

        j = p.get_json();
        p2 = math::Point(j);

        d = p.distance(p2);

    }


}

TEST_CASE( "Test JSON ", "[JSON]" ) {

    auto rng = util::RandomNumberGenerator();
    std::vector<std::vector<double>> values;
    for(int i = 0; i < 100; i++) {
        values.push_back(std::vector<double>{rng.rand(), rng.rand(), rng.rand()});

    }

    //with_stringify(rng, values);
    //with_json(rng, values);
    auto j = json::Array();
    for(auto const & v : values) {
        auto p = math::Point(v[0], v[1], v[2]);
        j.append(p.get_json());
    }
    /*for(int i = 0; i < j.size(); i++) {
        std::cout << j[i] << std::endl;
    }*/

    auto s = j.dump();
    std::cout << s << std::endl;

    std::cout << s.size() << std::endl;
    char b[1500];
    char c[10000];

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup "a" as the input and "b" as the compressed output
    defstream.avail_in = (uInt)s.size()+1; // size of input, string + terminator
    defstream.next_in = (Bytef *)s.c_str(); // input char array
    defstream.avail_out = (uInt)sizeof(b); // size of output
    defstream.next_out = (Bytef *)b; // output char array

    // the actual compression work.
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);


    std::ofstream out ("test.bin");
    out.write(b, sizeof(b));
    out.close();

    auto all_l = String("");
    auto lines = base::get_lines_from_file("test.bin");
    for(auto const & l : lines) {
        all_l += l;
    }
    std::cout << all_l.size() << std::endl;
    std::cout << (uInt)((char*)defstream.next_out - b) << std::endl;
    exit(0);

    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    // setup "b" as the input and "c" as the compressed output
    //infstream.avail_in = (uInt)(all_l.size()+1); // size of input
    //infstream.next_in = (Bytef *)all_l.c_str(); // input char array
    infstream.avail_in = (uInt)((char*)defstream.next_out - b); // size of input
    infstream.next_in = (Bytef *)b; // input char array
    infstream.avail_out = (uInt)sizeof(c); // size of output
    infstream.next_out = (Bytef *)c; // output char array

    // the actual DE-compression work.
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);

    std::cout << c << std::endl;







}