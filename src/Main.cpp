#include "Game/Game.h"

#include "Networking/Server.h"

int main(int argc, char* argv[])
{
    Bombmania::Game game;
    
    if (!game.Init())
        return 1;

    game.Run();

    return 0;
}