#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <fmt/core.h>
#include <iostream>
#include <string>

namespace logging {
void __logger(std::string fi, int ln, std::string fn, std::string lv,
              std::string mg);
}
#define NOTE(message)                                                          \
    logging::__logger(__FILE__, __LINE__, __FUNCTION__, "NOTICE", message)
#define WARN(message)                                                          \
    logging::__logger(__FILE__, __LINE__, __FUNCTION__, "WARNING", message)
#define ERR(message)                                                           \
    logging::__logger(__FILE__, __LINE__, __FUNCTION__, "ERROR", message)
#define FATAL(message)                                                         \
    logging::__logger(__FILE__, __LINE__, __FUNCTION__, "FATAL", message)

#endif // __LOG_HPP__
