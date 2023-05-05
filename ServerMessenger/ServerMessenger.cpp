#include "ServerMessenger.h"
#include "Server.h"

int main()
{
    spdlog::register_logger(std::make_shared<spdlog::logger>("server", spdlog::sinks_init_list{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>(), std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/server.log", 1024 * 1024 * 10, 5) }));
    
    std::jthread serverJThread(Server::Start);
    
    atexit(Server::Stop);

    int funnyNumber;
    std::cin >> funnyNumber;
}
