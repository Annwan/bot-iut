#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <iostream>
#include <string>

namespace logging {
void note(std::string message);
void warn(std::string message);
void err(std::string message);
void fatal(std::string message);
} // namespace log

#endif // __LOG_HPP__
