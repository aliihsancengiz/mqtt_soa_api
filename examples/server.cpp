#include "service.hpp"

#include <csignal>

std::atomic<bool> is_running{true};
void signalHandler(int signum)
{
    is_running.store(false);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, signalHandler);
    service::Server server("sample", "sub");

    while (is_running) {
        auto res = server.receive();
        if (res.is_some()) {
            std::cout << res.unwrap().message_id << "---" << res.unwrap().client_id << " ---- "
                      << res.unwrap().message << std::endl;
            server.respond(res.unwrap(), "Server Respose to " + res.unwrap().message + " : Rust.");
        }
    }

    return 0;
}