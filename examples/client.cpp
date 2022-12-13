#include "sync_service.hpp"

#include <csignal>

std::atomic<bool> is_running{true};
void signalHandler(int signum)
{
    is_running.store(false);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, signalHandler);
    sync::Client cli("sample", "sub");

    while (is_running) {
        cli.send("Request { Favirote language? }");
        auto res = cli.receive();
        if (res.is_some()) {
            std::cout << res.unwrap().message_id << "---" << res.unwrap().client_id << " ---- "
                      << res.unwrap().message << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}