#include "log.h"

void logging::__logger(std::string fi, int ln, std::string fn, std::string lv,
              std::string mg) {
    std::cerr << fmt::format("{}:{}({}) [{}] {}", fi, fn, ln, lv, mg)
              << std::endl;
}
