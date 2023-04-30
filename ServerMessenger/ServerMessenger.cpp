#include "ServerMessenger.h"

int main() {
    spdlog::register_logger(std::make_shared<spdlog::logger>(
        "main",
        spdlog::sinks_init_list
    {
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/main_log.txt", 1024 * 1024 * 10, 2)
    }));
    spdlog::get("main")->info("hello");
}
