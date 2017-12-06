//
// Created by Joseph Yesselman on 11/22/17.
//

#ifndef RNAMAKE_NEW_LOGGER_H
#define RNAMAKE_NEW_LOGGER_H

#include <iostream>
#include <base/types.h>

enum class Loglevel {
    INFO,
    WARNING,
    ERROR,
    NONE
};

static Loglevel LOG_LEVEL = Loglevel::INFO;

static inline void
SET_LOG_LEVEL(
        Loglevel new_level) {
    LOG_LEVEL = new_level;
}

using namespace std;
#define LOG_INFO(name, msg)  { \
    if(LOG_LEVEL <= Loglevel::INFO) { cout << "[INFO]    " << name << ":" << __FUNCTION__ << ": " << msg << std::endl; } \
}

#define LOG_WARNING(name, msg)  { \
    if(LOG_LEVEL <= Loglevel::WARNING) { cout << "[WARNING] " << name << ":" << __FUNCTION__ << ": " << msg << std::endl; } \
}

#define LOG_ERROR(name, msg)  { \
    if(LOG_LEVEL <= Loglevel::ERROR) { cout << "[ERROR]   " << name << ":" << __FUNCTION__ << ": " << msg  << std::endl; } \
}





#endif //RNAMAKE_NEW_LOGGER_H
