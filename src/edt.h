#ifndef __EDT_H__
#define __EDT_H__

#include "log.h"
#include <ctime>
#include <curl/curl.h>
#include <curl/easy.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <functional>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

struct Event {
    std::tm starting_time, ending_time;
    std::string summary, desc, location;
};

typedef std::vector<Event> Calendar;

std::vector<Event> get_timetable(std::string, std::tm, std::tm);

void clamp_calandar(std::vector<Event>&, std::tm, std::tm);



#endif
