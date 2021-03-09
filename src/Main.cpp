#include "Game/Game.h"

#include "Networking/net/tcp.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char* argv[])
{
    // TEMP: Set spdlog level debug.
    spdlog::set_level(spdlog::level::debug);

    //net::tcp::server server;
    //server.open(1234);
    //while (true)
    //{
    //    server.poll();
    //}

    using namespace net::literals;

    net::tcp::client client;
    client.connect("127.0.0.1:1234"_ipv4, []() { spdlog::critical("CONNECTED!!!!!"); });
    while (true)
    {
        client.poll();
    }

    Bombmania::Game game;
    
    if (!game.Init())
        return 1;

    game.Run();

    return 0;
}