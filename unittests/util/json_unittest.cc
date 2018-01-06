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

/*
 * for speed tests json is quite a bit faster of simple objects
 */
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










}