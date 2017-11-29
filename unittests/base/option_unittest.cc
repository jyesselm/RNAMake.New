//
// Created by Joseph Yesselman on 11/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <base/option.h>

TEST_CASE( "Test Options", "[Options]" ) {

    SECTION("test int options") {
        auto opt = base::IntOption("1");
        REQUIRE(opt.get_value() == 1);
        opt.set_value(5);
        REQUIRE(opt.get_value() == 5);
        REQUIRE_THROWS_AS(base::IntOption("f"), base::OptionException);
    }

    SECTION("test bool options") {
        auto opt = base::BoolOption("true");
        REQUIRE(opt.get_value() == true);
    }

    SECTION("test options class") {
        auto opt_info = base::OptInfos {
                base::OPT_INFO_STRING("string_opt", "test_1"),
                base::OPT_INFO_INT("int_opt", "1"),
                base::OPT_INFO_DOUBLE("double_opt", "1.0"),
                base::OPT_INFO_BOOL("bool_opt", "false")};

        auto options = base::Options(opt_info);
        //options.add_option("test", "test_1", base::OptionType::STRING);
        REQUIRE(options.get_string_value("string_opt") == "test_1");
        options.set_string_value("string_opt", "test_2");
        REQUIRE(options.get_string_value("string_opt") == "test_2");
        REQUIRE(options.get_int_value("int_opt") == 1);

    }


}