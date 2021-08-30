#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <dpp/discord.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "log.h"

struct Group {
    std::string name;
    dpp::snowflake group_id;
    dpp::snowflake channel_id;
    int ade_group;
};

struct Remind {
    dpp::user user;
    dpp::snowflake channel_id;
    std::string message;
};

class State {
public:
    State(std::string grp_config);
    State();
    
    void reload_config();
    void save_config();
    std::string config_path;
    std::vector<Group> groups;
    std::vector<Remind> reminds;
    
};

#endif // __CONFIG_HPP__3
