#include "config.h"

Config::Config(std::string grp_config) : config_path(grp_config) {
    this->reload_config();
}

Config::Config() : Config("groups.cfg") {}

void Config::reload_config() {
    this->groups.clear();
    std::ifstream config(this->config_path);
    if (config) {
        std::string entry;
        while (std::getline(config, entry)) {
            std::stringstream line(entry);
            std::string n;
            dpp::snowflake gid;
            dpp::snowflake cid;
            int eid;
            line >> n >> gid >> cid >> eid;
            this->groups.push_back({n, gid, cid, eid});
        }
    }
    config.close();
}

void Config::save_config() {
    std::ofstream config(this->config_path);
    for (Group g : this->groups) {
        config << g.name << " " << g.group_id << " " << g.channel_id << " "
               << g.ade_group << "\n";
    }
    config.close();
}
