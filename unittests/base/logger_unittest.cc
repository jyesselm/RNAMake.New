//
// Created by Joseph Yesselman on 11/22/17.
//


#include <iostream>
#include "../common.hpp"

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
//#include <base/logger.h>

namespace plog
{
class CustomFormatter
{
public:
    static util::nstring header()
    {
        return util::nstring();
    }

    static util::nstring format(const Record& record)
    {
        tm t;
        util::localtime_s(&t, &record.getTime().time);

        util::nostringstream ss;
        ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec <<  PLOG_NSTR(" ");
        ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << severityToString(record.getSeverity()) << PLOG_NSTR(" ");
        ss << PLOG_NSTR("[") << record.getTid() << PLOG_NSTR("] ");
        ss << PLOG_NSTR("[") << record.getFunc() << PLOG_NSTR("@") << record.getLine() << PLOG_NSTR("] ");
        ss << record.getMessage() << PLOG_NSTR("\n");

        return ss.str();
    }
};
}


TEST_CASE( "Test logging functions", "[Logger]" ) {
    /*SET_LOG_LEVEL(Loglevel::INFO);
    LOG_INFO("PDBParserUnittest", "test");
    LOG_WARNING("PDBParserUnittest", "test");
    LOG_ERROR("PDBParserUnittest", "test");
    SET_LOG_LEVEL(Loglevel::WARNING);
    LOG_INFO("PDBParserUnittest", "test");*/

    static plog::ColorConsoleAppender<plog::CustomFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);

    LOGI << "Hello log!"; // short macro
    LOG_DEBUG << "Hello log!"; // long macro
    LOG(plog::debug) << "Hello log!"; // function-style macro

}