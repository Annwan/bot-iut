#include "config.h"
#include "log.h"
#include <algorithm>
#include <cctype>
#include <string>

Config::Config(std::string grp_config) : config_path(grp_config) {
    this->reload_config();
}

Config::Config() : Config("groups.cfg") {}

void Config::reload_config() {
    NOTE("Starting config reload");
    this->groups.clear();
    NOTE("<Reload> Groups clear");
    std::ifstream config(this->config_path);
    if (config) {
        std::string entry;
        while (std::getline(config, entry)) {
            if (std::all_of(entry.begin(), entry.end(),
                            [](char p) { return std::isspace(p); }))
                break;
            std::stringstream line(entry);
            std::string n;
            dpp::snowflake gid;
            dpp::snowflake cid;
            std::string eid;
            line >> n;
            line >> gid;
            line >> cid;
            line >> eid;
            NOTE(fmt::format("<Reload> Adding group {} ({},{},{})", n, gid, cid,
                             eid));
            this->groups.push_back({n, gid, cid, eid});
        }
    }
    config.close();
    NOTE("Config reload complete");
}

void Config::save_config() {
    std::ofstream config(this->config_path);
    for (Group g : this->groups) {
        config << g.name << " " << g.group_id << " " << g.channel_id << " "
               << g.ade_group << "\n";
    }
    config.close();
}
