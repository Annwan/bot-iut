#include "log.h"
#include <string>

namespace logging {
void note(const std::string message) {
    std::cerr << "[NOTICE] " << message << std::endl;
}
void err(const std::string message) {
    std::cerr << "[ERROR] " << message << std::endl;
}
void warn(const std::string message) {
    std::cerr << "[WARNING] " << message << std::endl;
}
void fatal(const std::string message) {
    std::cerr << "[FATAL] " << message << std::endl;
    //TODO abort here
}
} // namespace logging
