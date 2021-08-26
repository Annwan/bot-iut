#include <dpp/dpp.h>
#include <iostream>
#include <fstream>

void register_slash_commands(dpp::cluster &bot)
{
  dpp::slashcommand ping_command;
  ping_command.set_name("ping")
    .set_description("Pings the bot to see if it is awake")
    .set_application_id(bot.me.id);

  bot.global_command_create(ping_command);
  std::cerr << "Created /ping command" << std::endl;

  dpp::slashcommand group_command;
  group_command.set_name("group")
    .set_description("Manage groups")
    .set_application_id(bot.me.id)
    .add_option(dpp::command_option(dpp::command_option_type::co_sub_command, "get", ))
}

int main()
{
  std::ifstream token_file ("./token.secret");
  std::string token;
  if (token_file.is_open()) {
    token_file >> token;
  } else {
    std::cerr << "Could not read token form ./token.secret" << std::endl;
    return 1;
  }

  std::cerr << "[Notice] Token read" << std::endl;
  
  dpp::cluster bot (token);

  std::cerr << "[Notice] Created bot object" << std::endl;

  bot.on_ready([&bot](const dpp::ready_t & event){
    register_slash_commands(bot);
    std::cerr << "[Notice] Started with session ID: " << event.session_id << std::endl;
    std::cerr << "[Notice] Logged in as " << bot.me.username << std::endl;
  });

  std::cerr << "[Notice] Registered `on_ready' event handler" << std::endl;

  bot.on_message_create([&bot](const dpp::message_create_t & event){
    if (event.msg->content == "%ping") {
      bot.message_create(dpp::message(event.msg->channel_id, "Pong!"));
    }
  });

  bot.on_interaction_create([](const dpp::interaction_create_t & event){
    if (event.command.type == dpp::it_application_command) {
      dpp::command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);
      if (cmd_data.name == "ping") {
        event.reply(dpp::ir_channel_message_with_source, "Pong!");
      }
    }
  });

  std::cerr << "[Notice] Registered `on_message_create' event handler" << std::endl;

  std::cerr << "[Notice] Starting bot" << std::endl;
  
  bot.start(false);
  return 0;
}
