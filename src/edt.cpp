#include "edt.h"

static std::string download_ade(std::string url) {
    CURL* curl;
    CURLcode rc;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    std::string ical_text;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                         [](void* buffer, size_t size, void* data) {
                             std::string* str = (std::string*)data;
                             str->append((char*)buffer, size);
                             return size;
                         });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ical_text);
        rc = curl_easy_perform(curl);
        if (rc != CURLE_OK) {
            ERR(fmt::format("cURL failed: {}", rc));
            return "";
        }
    } else {
        ERR("Couldnt get a handle on cURL");
    }

    return ical_text;
}

static int weekday(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }
    return (day + ((13 * (month + 1)) / 5) + (year % 100) + ((year % 100) / 4) +
            ((year / 100) / 4) - (2 * (year / 100)) + 1);
}

static int next_sunday(int wday, int mday) { return mday + 7 - wday - 1; }

static bool is_dst(int day, int month, int year) {
    switch (month) {
    case 1:  // Jan
    case 2:  // Feb
    case 11: // Nov
    case 12: // Dec
        return false;
    case 4: // Apr
    case 5: // May
    case 6: // Jun
    case 7: // Jul
    case 8: // Aug
    case 9:
        return true;
    case 3: // Mar
        return (next_sunday(weekday(day, month, year), day) > 31);
    case 10: // Oct
        return !(next_sunday(weekday(day, month, year), day) > 31);
    default:
        return false; // should never happen, but compiler wants it
    }
}

static std::tm parse_date(std::string str) {
    std::tm date_time{};
    date_time.tm_year = std::stoi(str.substr(0, 4)) - 1900;
    date_time.tm_mon = std::stoi(str.substr(4, 2)) - 1;
    date_time.tm_mday = std::stoi(str.substr(6, 2));
    if (str[8] == 'T') {
        date_time.tm_hour = std::stoi(str.substr(9, 2)) +
                            (is_dst(date_time.tm_mday, date_time.tm_mon + 1,
                                    date_time.tm_year + 1900)
                                 ? 2
                                 : 1);
        date_time.tm_min = std::stoi(str.substr(11, 2));
    }
    return date_time;
}

static Calendar extract_events(std::string ical_text) {
    std::vector<Event> events;
    std::stringstream ical_stream(ical_text);
    bool in_event = false;
    Event current_event;
    std::string line;
    while (std::getline(ical_stream, line)) {
        if (line == "BEGIN:VEVENT" && !in_event) {
            current_event = {};
            in_event = true;
            continue;
        }
        if (line == "END:VEVENT" && in_event) {
            events.push_back(Event(current_event));
            in_event = false;
            continue;
        }
        if (in_event) {
            if (line.substr(0, 8) == "DTSTART:") {
                current_event.starting_time = parse_date(line.substr(8));
                continue;
            }
            if (line.substr(0, 6) == "DTEND:") {
                current_event.ending_time = parse_date(line.substr(6));
                continue;
            }
            if (line.substr(0, 9) == "LOCATION:") {
                current_event.location = line.substr(9);
                continue;
            }
            if (line.substr(0, 8) == "SUMMARY:") {
                current_event.summary = line.substr(8);
                continue;
            }
            if (line.substr(0, 12) == "DESCRIPTION:") {
                current_event.desc = line.substr(12);
                continue;
            }
        }
    }
    std::sort(events.begin(), events.end(), [](Event a, Event b) {
        return std::mktime(&a.starting_time) < std::mktime(&b.starting_time);
    });
    return events;
}

Calendar get_timetable(std::string ade_url) {
    std::string ical_text = download_ade(ade_url);
    Calendar v = extract_events(ical_text);
    return v;
}

void clamp_calandar(Calendar& es, std::tm st, std::tm et) {
    es.erase(std::remove_if(es.begin(), es.end(), [&](Event e) {
        return std::mktime(&e.starting_time) < std::mktime(&st);
    }));
    es.erase(std::remove_if(es.begin(), es.end(), [&](Event e) {
        return std::mktime(&e.starting_time) > std::mktime(&et);
    }));
}

std::string format_calandar(Calendar c) {
    auto formatDate = [](std::tm date) {
        return fmt::format("{:%d/%m/%y}", date);
    };
    std::string rv = "";
    std::string lastDate = "";
    for (Event e : c) {
        std::string eDate = formatDate(e.starting_time);
        if (lastDate != eDate) {
            std::string teacher = std::regex_replace(e.desc, std::regex("^\\n\\n.+\\n(.+)\\n"), "$1");
            rv += fmt::format("**{}**\n", eDate);
            rv += fmt::format("{:%H:%M} → {:%H:%M} : **{}** en {} avec {}",
                              e.starting_time,
                              e.ending_time,
                              e.summary,
                              e.location,
                              (teacher == "") ? "Unknown" : teacher);
            
        }
    }
    return rv;
}
