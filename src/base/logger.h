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

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */

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
    if(LOG_LEVEL <= Loglevel::WARNING) { cout << GREEN << "[WARNING] " << name << ":" << __FUNCTION__ << ": " << msg << RESET << std::endl; } \
}

#define LOG_ERROR(name, msg)  { \
    if(LOG_LEVEL <= Loglevel::ERROR) { cout << RED << "[ERROR]   " << name << ":" << __FUNCTION__ << ": " << msg << RESET << std::endl; } \
}





#endif //RNAMAKE_NEW_LOGGER_H
