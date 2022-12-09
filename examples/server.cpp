#include "mqttsoa/sync_service.hpp"

#include <csignal>

std::atomic<bool> is_running{true};
void signalHandler(int signum)
{
    is_running.store(false);
}

using SS =sync::Service<common::Role::Server>;

int main(int argc, char* argv[])
{
    SS server("sample", "sub");
    signal(SIGINT, signalHandler);

    Option<std::string> res;

    while (is_running) {
        res = server.receive();
        if (res.is_some()) {
            std::cout << res.unwrap() << std::endl;
            server.send("Server Respose to " + res.unwrap() + " : Rust.");
        }
    }

    return 0;
}