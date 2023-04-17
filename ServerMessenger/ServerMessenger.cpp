#include "ServerMessenger.h"

int main() {
    auto logger = std::make_shared<spdlog::logger>(
        "main",
        spdlog::sinks_init_list
            {
                std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
                std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/main_log.txt", 1024 * 1024 * 10, 2)
            });
    spdlog::register_logger(logger);
    spdlog::get("main")->info("hello");
}
