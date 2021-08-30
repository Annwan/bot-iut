#include <cstdlib>
#include <dpp/dpp.h>
#include <fmt/core.h>
#include <fstream>
#include <iostream>

#include "config.h"
#include "log.h"

using namespace dpp;

void register_slash_commands(cluster& bot) {
    NOTE("Registering slash commands");
    slashcommand ping_command;
    ping_command.set_name("ping")
        .set_description("Pings the bot to see if it is awake")
        .set_application_id(bot.me.id);

    bot.global_command_create(ping_command);
    NOTE("Registered /ping command");

    slashcommand group_command;
    command_option group_role_option(co_string, "role",
                                     "the group to be added to", true);
    for (auto role : group_role_names()) {
        group_role_option.add_choice(command_option_choice(role, role));
    }
    group_command.set_name("group")
        .set_description("Manage groups")
        .set_application_id(bot.me.id)
        .add_option(group_role_option);
    bot.global_command_create(group_command);

    NOTE("Registered /group command");
    NOTE("Done registering slash commands");
}

int main() {

    std::ifstream token_file("./token.secret");
    std::string token;
    if (token_file.is_open()) {
        token_file >> token;
    } else {
        FATAL("Failed to read token file, aborting");
        std::abort();
    }

    setup_database();

    NOTE("Token loaded");

    dpp::cluster bot(token);

    NOTE("Created bot object");

    bot.on_ready([&bot](const dpp::ready_t& event) {
        register_slash_commands(bot);
        NOTE(fmt::format("Started with session ID {}", event.session_id));
        NOTE(fmt::format("Logged in as `{}`", bot.me.username));
    });

    NOTE("Registered `on_ready' event handler");

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

    NOTE("Registered `on_message_create' event handler");

    NOTE("Starting bot");

    bot.start(false);
    return 0;
}
