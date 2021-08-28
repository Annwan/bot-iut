#include <dpp/dpp.h>
#include <fstream>
#include <iostream>

#include "config.h"
#include "log.h"

using namespace dpp;

void register_slash_commands(cluster& bot) {
    logging::note("Registering slash commands");
    slashcommand ping_command;
    ping_command.set_name("ping")
        .set_description("Pings the bot to see if it is awake")
        .set_application_id(bot.me.id);

    bot.global_command_create(ping_command);
    logging::note("Registered /ping command");

    slashcommand group_command;
    command_option group_role_option(co_string, "role",
                                     "the group to be added to", true);
    for (std::string role : group_role_names()) {
        group_role_option.add_choice(command_option_choice(role, role));
    }
    group_command.set_name("group")
        .set_description("Manage groups")
        .set_application_id(bot.me.id)
        .add_option(group_role_option);
    bot.global_command_create(group_command);

    logging::note("Registered /group command");
    logging::note("Done registering slash commands");
}

int main() {

    std::ifstream token_file("./token.secret");
    std::string token;
    if (token_file.is_open()) {
        token_file >> token;
    } else {
        logging::fatal("Failed to read token file, aborting");
    }

    logging::note("Token loaded");

    dpp::cluster bot(token);

    logging::note("Created bot object");

    bot.on_ready([&bot](const dpp::ready_t& event) {
        register_slash_commands(bot);
        std::cerr << "[NOTICE] Started with session ID: " << event.session_id
                  << std::endl;
        std::cerr << "[NOTICE] Logged in as " << bot.me.username << std::endl;
    });

    std::cerr << "[NOTICE] Registered `on_ready' event handler" << std::endl;

    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        if (event.msg->content == "%ping") {
            bot.message_create(dpp::message(event.msg->channel_id, "Pong!"));
        }
    });

    bot.on_interaction_create([](const dpp::interaction_create_t& event) {
        if (event.command.type == dpp::it_application_command) {
            dpp::command_interaction cmd_data =
                std::get<dpp::command_interaction>(event.command.data);
            if (cmd_data.name == "ping") {
                event.reply(dpp::ir_channel_message_with_source, "Pong!");
            } else if (cmd_data.name == "group") {
            }
        }
    });

    std::cerr << "[NOTICE] Registered `on_message_create' event handler"
              << std::endl;

    std::cerr << "[NOTICE] Starting bot" << std::endl;

    bot.start(false);
    return 0;
}
