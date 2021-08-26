#include <dpp/dpp.h>
#include <iostream>
#include <fstream>

int main()
{
  std::ifstream token_file ("./token.secret");
  std::string token;
  if (token_file.is_open()) {
    token_file >> token;
  } else {
    std::cerr << "Could not read token form ./token.secret";
    return 1;
  }

  dpp::cluster bot (token);

  bot.on_ready([&bot](const dpp::ready_t & event){
    std::cout << "Bot started: " << event.from->ready << "\n";
    std::cout << "Logged in as " << bot.me.username << std::endl;
  });

  bot.on_message_create([&bot](const dpp::message_create_t & event){
    if (event.msg->content == "%ping") {
      bot.message_create(dpp::message(event.msg->channel_id, "Pong!"));
    }
  });
  
  return 0;
}
