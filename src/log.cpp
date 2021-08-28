#include "log.hpp"

namespace log {
void note(const std::string message) {
  std::cerr << "[NOTICE] " << message << std::endl;
}
} // namespace log
