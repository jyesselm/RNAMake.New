//
// Created by Joseph Yesselman on 1/6/18.
//

#include <base/log.h>

namespace base {

void
init_logging(
        LogLevel log_level) {
    static plog::ColorConsoleAppender<plog::CustomFormatter> consoleAppender;
    plog::init((plog::Severity)log_level, &consoleAppender);
}

}
