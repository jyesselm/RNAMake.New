#ifndef __common
#define __common

#pragma once

#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif

#include "catch.hpp"
#include <base/log.h>

// very nice modern implemention of singleton
// from: https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template
template<typename T>
class UnitestSingleton {
public:
    static T& get() {
        static const std::unique_ptr<T> instance{ new T() };
        return *instance;
    }

    static T& get(
            base::LogLevel log_level) {
        static const std::unique_ptr<T> instance{ new T(log_level) };
        return *instance;
    }

    UnitestSingleton(const UnitestSingleton&) = delete;
    UnitestSingleton& operator= (const UnitestSingleton) = delete;

protected:
    UnitestSingleton() {}
};

struct BasicUnitestData {
    BasicUnitestData(
            base::LogLevel log_level = base::LogLevel::INFO) {
        // always start logging
        base::init_logging(log_level);
    }
};

void
init_unittest_safe_logging(
        base::LogLevel log_level = base::LogLevel::INFO) { UnitestSingleton<BasicUnitestData>::get(log_level); }


#endif























